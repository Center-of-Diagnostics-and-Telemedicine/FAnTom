#include "pre.h"

#include "XRay.h"
//#include <sstream>
#include <FantomLogger.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>


operation_result XRay::LoadByAccession()
{
	START_LOG;
	bool acc_loaded = false;

	if ( m_proc_acquisition_ptr != nullptr)
	{
		acc_loaded = true;
		return e_successful;
	}

	

	m_proc_acquisition_ptr = CreateProcessAcquisition(GetInstancesOfStudy(), ConsoleProgressProxy());

	ProcessAcquisitionOpenClose prcAcq(*m_proc_acquisition_ptr);
	//	proc_acquisition_work_ptr->open_instancestorages();

//	m_XR_images = XrayAcquisition_ptr().slices();

	Dicom::instance	&sample_instance = *XrayAcquisition_ptr().loader().front();

	m_patient_id = sample_instance.get_wstring(Dicom::e_patient_id);
	m_patient_sex = sample_instance.get_wstring(Dicom::e_patient_sex);
	m_patient_age = sample_instance.get_wstring(Dicom::e_patient_age);
	m_study_id = sample_instance.get_wstring(Dicom::e_study_id);
	m_study_instance_uid = sample_instance.get_wstring(Dicom::e_study_instance_uid);

	size_t i = 0;

	for (Dicom::instance_ptr &inst_ptr : XrayAcquisition_ptr().loader())
	{
		Dicom::image &slice_container = dynamic_cast<Dicom::image&>(*inst_ptr);

		RealFunction2D_F32 data_slice(slice_container.vsize(), slice_container.hsize());

		slice_container.get_image(data_slice);

		bool inverse = inst_ptr->get_wstring(Dicom::e_photometric_interpretation) == L"MONOCHROME1";
		if (inverse)
		{
			double	mx = MaxValue(data_slice);
			//			double	mn = MinValue(data_slice);

			ApplyFunction(data_slice, [mx](float &x) {return x = (mx - x); });
		}

		m_XR_images.push_back(move(data_slice));

		wstring str = inst_ptr->get_wstring(Dicom::e_acquisition_device_processing_description);

		vector<wstring> var1;
		vector<wstring> var2;

/*		if (i == 0)		
		{
		    var2 = { L"0.2",L"0.1" };
		}
		else if (i == 1)
		{
			var1 = { L"0.1",L"0.2" };
			var2 = { L"0.2",L"0.1" };
		}
		else
		{*/
			var1 = inst_ptr->get_wstring_values(Dicom::e_imager_pixel_spacing);
			var2 = inst_ptr->get_wstring_values(Dicom::e_pixel_spacing);
	//	}
	//	i++;
			this->AddToStepsVector(var1, var2);

			m_ScreenSize.push_back({
				static_cast<size_t> (m_XR_Images()[i].sizes(0)*m_Steps[i].y() / min(m_Steps[i].y(), m_Steps[i].x())), 
				static_cast<size_t> (m_XR_Images()[i].sizes(1)*m_Steps[i].x() / min(m_Steps[i].y(), m_Steps[i].x()))
			});

			i++;
	}

	m_bmp.resize(m_XR_Images().size());
//	m_ScreenSize.resize(m_XR_Images().size());

	END_LOG;
	return e_successful;
}

operation_result XRay::GetDimensions(nlohmann::json &j)
{
	int i = 0;

	for (auto &image : m_XR_Images())
	{
		nlohmann::json node;
		node["n_images"] = 1;

		node["screen_size_v"] = m_ScreenSize[i].y();
		node["screen_size_h"] = m_ScreenSize[i].x();

		node["dicom_size_v"] = image.sizes(0); //m_XR_Images()[i].sizes(0);
		node["dicom_size_h"] = image.sizes(1); //m_XR_Images()[i].sizes(1);

		node["dicom_step_v"] = m_Steps[i].y();
		node["dicom_step_h"] = m_Steps[i].x();

		string str = "dx" + to_string(i);

		j["response"][modality_t::DX()][str] = node;

		i++;
	}

	return e_successful;
}

operation_result XRay::GetDictionary(nlohmann::json &j)
{
	j["#0_H"]["RU"] = convert_to_string8("гидроторокс");
	j["#0_H"]["EN"] = "hydrothorax";

	j["#1_PC"]["RU"] = convert_to_string8("застой по МКК");
//	j["#1_MG"]["EN"] = "todo";//??????????

	return e_successful;
}

operation_result  XRay::GetModality(string &modality)
{
	modality = modality_t::DX();
	return e_successful;
}

operation_result XRay::GetScreenImage(const unsigned char **img, int *length, image_index_t idx, brightness brightness)
{
//	XRAD_ASSERT_THROW(idx.modality == modality_t::DX());

	if (idx.modality != modality_t::DX())
	{
		throw modality_error("modality is DX");
	}

	frame_t img_screen;

	int &N = idx.image_no;

//	N = range(N, 0, m_XR_Images(). size() - 1);

	if (N < 0 || N >(m_XR_Images().size() - 1))
	{
		throw image_error("unknown image type");
	}

	if(m_EqualSteps[N])
	{
		this->GetImage(img_screen, idx);
	}
	else
	{
		frame_t	buffer;

		this->GetImage(buffer, idx);

		img_screen.realloc(m_ScreenSize[N].y(), m_ScreenSize[N].x());

		RescaleImageToScreenCoordinates(img_screen, buffer, idx);
	}

	double white = brightness.white;
	double black = brightness.black;
	double gamma = brightness.gamma;

	ApplyFunction(img_screen, [black, white](float x) {return x<black ? 0 : x>white ? 255 : 255.*(x - black) / (white - black); });

	ApplyFunction(img_screen, [gamma](float x) {return 255.*pow(x / 255., gamma); });


	m_bmp[N].SetSizes(img_screen.vsize(), img_screen.hsize());

	m_bmp[N].palette.realloc(256);

	for (size_t i = 0; i < 256; ++i)
	{
		m_bmp[N].palette[i] = static_cast<uint8_t>(i);
	}

	for (size_t i = 0; i < img_screen.vsize(); ++i)
	{
		for (size_t j = 0; j < img_screen.hsize(); ++j)
		{
			m_bmp[N].at(i, j) = img_screen.at(img_screen.vsize() - i - 1, j);
		}
	}

	*length = static_cast<int>(m_bmp[N].GetBitmapFileSize());

	*img = reinterpret_cast<const unsigned char*>(m_bmp[N].GetBitmapFile());

	return e_successful;
}



void XRay::RescaleImageToScreenCoordinates(frame_t &img_screen,const frame_t &buffer, image_index_t idx)
{
	size_t N = idx.image_no;

	for (size_t i = 0; i < img_screen.vsize(); ++i)
	{
	//	double y = ScreenToDicomCoordinate(i, v);
		double y = (double) i * m_XR_Images()[N].sizes(0)/ m_ScreenSize[N].y();

		for (size_t j = 0; j < img_screen.hsize(); ++j)
		{
		//	double x = ScreenToDicomCoordinate(j, h);
			double x = (double) j * m_XR_Images()[N].sizes(1) / m_ScreenSize[N].x();

			img_screen.at(i, j) = buffer.in(y, x, &interpolators2D::ibicubic);
		}
	}

}

int XRay::AddToStepsVector(vector<wstring> var1, vector <wstring> var2)
{
	if (var1.size() == 2 && var2.size() == 2)
	{
		if (var1[0] != var1[1])
		{
			m_EqualSteps.push_back(false) ;
		}
		else
		{
			m_EqualSteps.push_back(true);
		}
		m_Steps.push_back({
			wcstod(var2[0].c_str(), NULL),
			wcstod(var2[1].c_str(), NULL)
		});
	}
	else if (var1.size() == 0 && var2.size() == 2)
	{
		if (var2[0] != var2[1])
		{
			m_EqualSteps.push_back(false);
		}
		else
		{
			m_EqualSteps.push_back(true);
		}

		m_Steps.push_back({
			wcstod(var2[0].c_str(), NULL),
			wcstod(var2[1].c_str(), NULL)
		});
	}
	else if (var1.size() == 2 && var2.size() == 0)
	{
		if (var1[0] != var1[1])
		{
			m_EqualSteps.push_back(false);
		}
		else
		{
			m_EqualSteps.push_back(true);
		}

		m_Steps.push_back({
			wcstod(var1[0].c_str(), NULL),
			wcstod(var1[1].c_str(), NULL)
		});
	}
	else
	{
		throw std::invalid_argument("Pixel sizes (or pixel steps) are not given in current Dicom");

// 		m_EqualSteps.push_back(true);
// 
// 		m_Steps.push_back(make_pair(
// 			wcstod(var1[0].c_str(), NULL),
// 			wcstod(var1[1].c_str(), NULL)
// 		));
	}

	return 0;
}



operation_result XRay::GetImage(frame_t &img, const image_index_t idx)
{
	XRAD_ASSERT_THROW(idx.modality == modality_t::DX());

	size_t no = range(idx.image_no, 0, m_XR_Images().size() - 1);

	img.MakeCopy(m_XR_Images()[no]);

	return e_successful;
}


operation_result XRay::GetBrightness(double *value, image_index_t idx, int y, int x)
{ 
//	XRAD_ASSERT_THROW(idx.modality == modality_t::DX());

	if (idx.modality != modality_t::DX())
	{
		throw modality_error("modality is DX");
	}

	int &no = idx.image_no;
//	size_t no = range(idx.image_no, 0, m_XR_Images().size() - 1);

	if (no < 0 || no > (m_XR_Images().size() - 1))
	{
		throw image_error("unknown image type");
	}

	x = range(x, 0, m_XR_Images()[no].sizes(1) - 1);
	y = range(y, 0, m_XR_Images()[no].sizes(0) - 1);

	*value = m_XR_Images()[no].at(y, x);

	return e_successful;
}