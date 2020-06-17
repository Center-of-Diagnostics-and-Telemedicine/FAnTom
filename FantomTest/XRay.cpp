#include "pre.h"

#include "XRay.h"
#include <sstream>

#include <XRADBasic/Sources/Utils/ConsoleProgress.h>


/*
void XRay::CreateQByteArrayPngFromChar(QByteArray &png, const unsigned char *img, int length)
{
	QImage q_image;
	
	if (q_image.loadFromData(img, length, ".bmp"))
	{
		QBuffer buffer;
		buffer.open(QIODevice::ReadWrite);
		//q_image.save(&buffer, "bmp"); // writes pixmap into bytes in BMP format
		q_image.save(&buffer, "png");	// writes pixmap into bytes in PNG format

		//q_image.save("C:/temp/bbb.png", "png");
	
		png = buffer.buffer().toBase64();
	}
}
*/

int XRay::LoadByAccession(const wstring accession_number)
{
	bool acc_loaded = false;

	if (m_accession_number == accession_number && m_proc_acquisition_ptr != nullptr)
	{
		acc_loaded = true;
		return 0;// e_successful;
	}

	size_t chosen_position = GetAccessionHeapPosition(accession_number, acc_loaded);

	if (!acc_loaded) return -1;

	m_proc_acquisition_ptr = CreateProcessAcquisition(GetInstancesOfStudy(chosen_position), ConsoleProgressProxy());

	ProcessAcquisitionOpenClose prcAcq(*m_proc_acquisition_ptr);
	//	proc_acquisition_work_ptr->open_instancestorages();

//	m_XR_images = XrayAcquisition_ptr().slices();

	m_accession_number = accession_number;

	Dicom::instance	&sample_instance = *XrayAcquisition_ptr().loader().front();

	m_patient_id = sample_instance.get_wstring(Dicom::e_patient_id);
	m_patient_sex = sample_instance.get_wstring(Dicom::e_patient_sex);
	m_patient_age = sample_instance.get_wstring(Dicom::e_patient_age);
	m_study_id = sample_instance.get_wstring(Dicom::e_study_id);
	m_study_instance_uid = sample_instance.get_wstring(Dicom::e_study_instance_uid);

//	size_t i = 0;

	for (Dicom::instance_ptr &inst_ptr : XrayAcquisition_ptr().loader())
	{
		Dicom::image &slice_container = dynamic_cast<Dicom::image&>(*inst_ptr);

		RealFunction2D_F32 data_slice(slice_container.vsize(), slice_container.hsize());

		slice_container.get_image(data_slice);

		m_XR_images.push_back(move(data_slice));

		wstring str = inst_ptr->get_wstring(Dicom::e_acquisition_device_processing_description);

		vector<wstring> var1;
		vector<wstring> var2;

//		if (i == 0)
//		{
//		    var2 = { L"0.2",L"0.1" };
//		}
//		else
//		{
			var1 = inst_ptr->get_wstring_values(Dicom::e_imager_pixel_spacing);
			var2 = inst_ptr->get_wstring_values(Dicom::e_pixel_spacing);
//		}
//		i++;

		this->AddToStepsVector(var1, var2);
		//if (this->AddToStepsVector(var1, var2) < 0)   return -1;
	}

	m_bmp.resize(m_XR_images.size());

	return 0;
}


void XRay::GetScreenImage(const unsigned char **img, int *length, image_index_t idx, double black, double white, double gamma, mip_index_t mip)
{
	frame_t img_screen;

	size_t N = idx.image_no;

	if(m_EqualSteps[N])
	{
		this->GetImage(img_screen, idx);
	}
	else
	{
		frame_t	buffer;

		this->GetImage(buffer, idx);

		m_ScreenSize.first = (size_t) m_XR_Images()[N].sizes(0)*m_Steps[N].first / min(m_Steps[N].first, m_Steps[N].second);

		m_ScreenSize.second = (size_t) m_XR_Images()[N].sizes(1)*m_Steps[N].second / min(m_Steps[N].first, m_Steps[N].second);

		img_screen.realloc(m_ScreenSize.first, m_ScreenSize.second);

		RescaleImageToScreenCoordinates(img_screen, buffer, idx);
	
	}

	ApplyFunction(img_screen, [black, white](float x) {return x<black ? 0 : x>white ? 255 : 255.*(x - black) / (white - black); });

	ApplyFunction(img_screen, [gamma](float x) {return 255.*pow(x / 255., gamma); });


	m_bmp[N].SetSizes(img_screen.vsize(), img_screen.hsize());

	m_bmp[N].palette.realloc(256);

	for (size_t i = 0; i < 256; ++i)
	{
		m_bmp[N].palette[i] = static_cast<uint8_t>(i);
	}
	m_bmp[N].CopyData(img_screen);

//	DisplayMathFunction2D(img_screen, L"Выбраный срез");

	*length = static_cast<int>(m_bmp[N].GetBitmapFileSize());

	*img = reinterpret_cast<const unsigned char*>(m_bmp[N].GetBitmapFile());//bitmap_buffer.get();

}



void XRay::RescaleImageToScreenCoordinates(frame_t &img_screen,const frame_t &buffer, image_index_t idx)
{
	size_t N = idx.image_no;

	for (size_t i = 0; i < img_screen.vsize(); ++i)
	{
	//	double y = ScreenToDicomCoordinate(i, v);
		double y = (double) i * m_XR_Images()[N].sizes(0)/ m_ScreenSize.first;

		for (size_t j = 0; j < img_screen.hsize(); ++j)
		{
		//	double x = ScreenToDicomCoordinate(j, h);
			double x = (double) j * m_XR_Images()[N].sizes(1) / m_ScreenSize.second;

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
			m_Steps.push_back(make_pair(
				wcstod(var1[0].c_str(), NULL), 
				wcstod(var1[1].c_str(), NULL)
			));
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

		m_Steps.push_back(make_pair(
			wcstod(var2[0].c_str(), NULL),
			wcstod(var2[1].c_str(), NULL)
		));
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

		m_Steps.push_back(make_pair(
			wcstod(var1[0].c_str(), NULL),
			wcstod(var1[1].c_str(), NULL)
		));
	}
	else
	{
		m_EqualSteps.push_back(true);
	}

	return 0;
}



void XRay::GetImage(frame_t &img, image_index_t idx)
{
	XRAD_ASSERT_THROW(idx.modality == modality_t::DX());

	size_t no = range(idx.image_no, 0, m_XR_Images().size() - 1);

	img.MakeCopy(m_XR_Images()[no]);
}


void XRay::GetBrightness(double *value, image_index_t idx, size_t y, size_t x)
{ 
	XRAD_ASSERT_THROW(idx.modality == modality_t::DX());

	size_t no = range(idx.image_no, 0, m_XR_Images().size() - 1);

	x = range(x, 0, m_XR_Images()[no].sizes(1) - 1);
	y = range(y, 0, m_XR_Images()[no].sizes(0) - 1);

	*value = m_XR_Images()[no].at(y, x);
}