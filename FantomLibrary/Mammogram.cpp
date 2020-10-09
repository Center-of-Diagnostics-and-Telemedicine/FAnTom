#include "pre.h"

#include "Mammogram.h"
#include <FantomLogger.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>

vector<wstring> mg_rcc_dict =
{
	L"RCC",
	L"R MAMMOGRAPHY, CC",
	L"R CC"
};

bool	is_rcc(const wstring &str)
{
	return (std::find(mg_rcc_dict.begin(), mg_rcc_dict.end(), str) != mg_rcc_dict.end());
}


vector<wstring> mg_lcc_dict = 
{
	L"LCC",
	L"L MAMMOGRAPHY, CC",
	L"L CC"
};

bool	is_lcc(const wstring &str)
{
	return (std::find(mg_lcc_dict.begin(), mg_lcc_dict.end(), str) != mg_lcc_dict.end());
}


vector<wstring> mg_rmlo_dict =
{
	L"RMLO",
	L"R MAMMOGRAPHY, MLO",
	L"R MLO"
};

bool	is_rmlo(const wstring &str)
{
	return (std::find(mg_rmlo_dict.begin(), mg_rmlo_dict.end(), str) != mg_rmlo_dict.end());
}

vector<wstring> mg_lmlo_dict =
{
	L"LMLO",
	L"L MAMMOGRAPHY, MLO",
	L"L MLO"
};

bool	is_lmlo(const wstring &str)
{
	return (std::find(mg_lmlo_dict.begin(), mg_lmlo_dict.end(), str) != mg_lmlo_dict.end());
}


operation_result Mammogram::LoadByAccession()
{
	START_LOG;

	bool acc_loaded = false;

	if (m_proc_acquisition_ptr != nullptr)
	{
		acc_loaded = true;
		return e_successful;
	}

	m_proc_acquisition_ptr = CreateProcessAcquisition(GetInstancesOfStudy(), ConsoleProgressProxy());

	ProcessAcquisitionOpenClose prcAcq(*m_proc_acquisition_ptr);
	//	proc_acquisition_work_ptr->open_instancestorages();

	Dicom::instance	&sample_instance = *MMAcquisition_ptr().loader().front();

	m_patient_id = sample_instance.get_wstring(Dicom::e_patient_id);
	m_patient_sex = sample_instance.get_wstring(Dicom::e_patient_sex);
	m_patient_age = sample_instance.get_wstring(Dicom::e_patient_age);
	m_study_id = sample_instance.get_wstring(Dicom::e_study_id);
	m_study_instance_uid = sample_instance.get_wstring(Dicom::e_study_instance_uid);

	for (Dicom::instance_ptr &inst_ptr : MMAcquisition_ptr().loader())
	{
		Dicom::image &slice_container = dynamic_cast<Dicom::image&>(*inst_ptr);

		RealFunction2D_F32 data_slice(slice_container.vsize(), slice_container.hsize());

		slice_container.get_image(data_slice);

		bool inverse = inst_ptr->get_wstring(Dicom::e_photometric_interpretation) == L"MONOCHROME1";
		if (inverse)
		{
			double	mx = MaxValue(data_slice);
//			double	mn = MinValue(data_slice);

			ApplyFunction(data_slice, [mx](float &x) {return x = (mx-x); });
		}

		string image_type;

		wstring str = inst_ptr->get_wstring(Dicom::e_acquisition_device_processing_description);

		wcout << L"The value of (0018,1400) tag is " << str << endl;

		size_t GE_MMG_TAG = 4526107;

		if ( !is_rcc(str) && !is_lcc(str) && !is_rmlo(str) && !is_lmlo(str)) str = inst_ptr->get_wstring(reinterpret_cast<Dicom::tag_e&>(GE_MMG_TAG));

		wcout << L"The value of (0045,101B) tag is " << str << endl;

		if( is_rcc(str))	image_type = image_t::mg_rcc();

		else if (is_lcc(str))	image_type = image_t::mg_lcc();

		else if (is_rmlo(str))	image_type = image_t::mg_rmlo();

		else if (is_lmlo(str))	image_type = image_t::mg_lmlo();

		m_MM_images[image_type] = std::move(data_slice);

		vector<wstring> var1;
		vector<wstring> var2;

/*		if (image_type == image_t::e_mg_lcc())
		{
		var2 = { L"0.2",L"0.1" };
		}
		else
		{*/
		var1 = inst_ptr->get_wstring_values(Dicom::e_imager_pixel_spacing);
		var2 = inst_ptr->get_wstring_values(Dicom::e_pixel_spacing);
//		}

		this->AddToStepsMap(image_type, var1, var2);

		m_ScreenSize[image_type].y() = (size_t)m_MM_Images()[image_type].sizes(0)*m_Steps[image_type].y() / min(m_Steps[image_type].y(), m_Steps[image_type].x());

		m_ScreenSize[image_type].x() = (size_t)m_MM_Images()[image_type].sizes(1)*m_Steps[image_type].x() / min(m_Steps[image_type].y(), m_Steps[image_type].x());
}
	END_LOG;
	return e_successful;
}

operation_result  Mammogram::GetModality(string &modality)
{
	modality = modality_t::MG();
	return e_successful;
}

int Mammogram::AddToStepsMap(const string image_type, vector<wstring> var1, vector <wstring> var2)
{
	if (var1.size() == 2 && var2.size() == 2)
	{
		if (var1[0] != var1[1])
		{
			m_EqualSteps[image_type]=false;
		}
		else
		{
			m_EqualSteps[image_type] = true;
		}
		m_Steps[image_type].y() = wcstod(var2[0].c_str(), NULL);
		m_Steps[image_type].x() = wcstod(var2[1].c_str(), NULL);
	}
	else if (var1.size() == 0 && var2.size() == 2)
	{
		if (var2[0] != var2[1])
		{
			m_EqualSteps[image_type] = false;
		}
		else
		{
			m_EqualSteps[image_type] = true;
		}
		m_Steps[image_type].y() = wcstod(var2[0].c_str(), NULL);
		m_Steps[image_type].x() = wcstod(var2[1].c_str(), NULL);
	}
	else if (var1.size() == 2 && var2.size() == 0)
	{
		if (var1[0] != var1[1])
		{
			m_EqualSteps[image_type] = false;
		}
		else
		{
			m_EqualSteps[image_type] = true;
		}

		m_Steps[image_type].y() = wcstod(var1[0].c_str(), NULL);
		m_Steps[image_type].x() = wcstod(var1[1].c_str(), NULL);
	
	}
	else
	{
		throw std::invalid_argument("Pixel sizes (or pixel steps) are not given in current Dicom");

//		m_EqualSteps[image_type] = true;
//		m_Steps[image_type].y() = wcstod(var1[0].c_str(), NULL);
//		m_Steps[image_type].x() = wcstod(var1[1].c_str(), NULL);

	}

	return 0;
}

operation_result Mammogram::GetDimensions(nlohmann::json &j)
{
	for (auto &image : m_MM_Images())
	{
		nlohmann::json node;
		node["n_images"] = 1;

		node["screen_size_v"] = m_ScreenSize[image.first].y();
		node["screen_size_h"] = m_ScreenSize[image.first].x();

		node["dicom_size_v"] = image.second.sizes(0);// m_MM_images[image.first].sizes(0);
		node["dicom_size_h"] = image.second.sizes(1);//m_MM_images[image.first].sizes(1);

		node["dicom_step_v"] = m_Steps[image.first].y();
		node["dicom_step_h"] = m_Steps[image.first].x();

		j["response"][modality_t::MG()][image.first] = node;
	}
	
	return e_successful; 
}

operation_result Mammogram::GetDictionary(nlohmann::json &j)
{
	nlohmann::json j0;  j1, j2, j3, j4, j5;


	j0["#0_M"]["RU"] = convert_to_string8("Прочие находки");
	j0["#0_M"]["EN"] = "miscellaneous";

	j1["#1_MG"]["RU"] = convert_to_string8("ЗНО");
	j1["#1_MG"]["EN"] = "Malignant growth";

	j2["#2_B"]["RU"] = convert_to_string8("Доброкачественное");
	j2["#2_B"]["EN"] = "Benign";

	j3["#3_SC"]["RU"] = convert_to_string8("Подозрительные кальцинаты");
	j3["#3_SC"]["EN"] = "Suspicious calcification";

	j4["#4_PA"]["RU"] = convert_to_string8("Патологически измененные лимфоузлы");
	j4["#4_PA"]["EN"] = "Pathologically altered lymph nodes";

	j5["#5_TS"]["RU"] = convert_to_string8("Утолщение кожи");
	j5["#5_TS"]["EN"] = "Thickening of the skin";



	j = {
		j0, j1, j2, j3, j4 , j5
	};
	return e_successful;
}

operation_result Mammogram::GetScreenImage(const unsigned char **img, int *length, image_index_t idx, brightness brightness)
{
//	XRAD_ASSERT_THROW(idx.modality == modality_t::MG());

	if (idx.modality != modality_t::MG())
	{
		throw modality_error("modality is MG");
	}

	frame_t img_screen;

	const string &image = idx.image_type;

	if (image != image_t::mg_rcc() && image != image_t::mg_lcc() && image != image_t::mg_rmlo() && image != image_t::mg_lmlo())
	{
		throw image_error("unknown image type");
	}

	if (idx.width && idx.height)
	{
		throw image_error("both size of image are given in input json");
	}

	if (!idx.width && !idx.height)
	{
		throw image_error("both size of image are given in input json are zero");
	}

	if (m_EqualSteps[image] && !idx.width && !idx.height)
	{
		this->GetImage(img_screen, idx);
	}
	else
	{
		frame_t	buffer;

		this->GetImage(buffer, idx);

		if (idx.width)  
		{
			m_ScreenSize[image].y() = (size_t)(idx.width * m_ScreenSize[image].y()/ m_ScreenSize[image].x());
			m_ScreenSize[image].x() = idx.width;
		}

		else if (idx.height)
		{
			m_ScreenSize[image].x() = (size_t)(idx.height * m_ScreenSize[image].x() / m_ScreenSize[image].y());
			m_ScreenSize[image].y() = idx.height;
		}

//		m_ScreenSize[image].y() = (size_t)m_MM_Images()[image].sizes(0)*m_Steps[image].y() / min(m_Steps[image].y(), m_Steps[image].x());

//		m_ScreenSize[image].x() = (size_t)m_MM_Images()[image].sizes(1)*m_Steps[image].x() / min(m_Steps[image].y(), m_Steps[image].x());

		img_screen.realloc(m_ScreenSize[image].y(), m_ScreenSize[image].x());

		RescaleImageToScreenCoordinates(img_screen, buffer, idx);
	}

	double white = brightness.white;
	double black = brightness.black;
	double gamma = brightness.gamma;

	ApplyFunction(img_screen, [black, white](float x) {return x<black ? 0 : x>white ? 255 : 255.*(x - black) / (white - black); });

	ApplyFunction(img_screen, [gamma](float x) {return 255.*pow(x / 255., gamma); });


	m_bmp[image].SetSizes(img_screen.vsize(), img_screen.hsize());

	m_bmp[image].palette.realloc(256);

	for (size_t i = 0; i < 256; ++i)
	{
		m_bmp[image].palette[i] = static_cast<uint8_t>(i);
	}

	for (size_t i = 0; i < img_screen.vsize(); ++i)
	{
		for (size_t j = 0; j < img_screen.hsize(); ++j)
		{
			m_bmp[image].at(i, j) = img_screen.at(img_screen.vsize() - i - 1, j);
		}
	}

	*length = static_cast<int>(m_bmp[image].GetBitmapFileSize());

	*img = reinterpret_cast<const unsigned char*>(m_bmp[image].GetBitmapFile());

	return e_successful;
}

void Mammogram::RescaleImageToScreenCoordinates(frame_t &img_screen, const frame_t &buffer, image_index_t idx)
{
	string image = idx.image_type;

	for (size_t i = 0; i < img_screen.vsize(); ++i)
	{
		//	double y = ScreenToDicomCoordinate(i, v);
		double y = (double)i * m_MM_Images()[image].sizes(0) / m_ScreenSize[image].y();//y = i * step_screen_y / step_dicom = i * step_dicom * N_dicom/(N_screen * step_dicom) = i * N_dicom/N_screen

		for (size_t j = 0; j < img_screen.hsize(); ++j)
		{
			//	double x = ScreenToDicomCoordinate(j, h);
			double x = (double)j * m_MM_Images()[image].sizes(1)  / m_ScreenSize[image].x();

			img_screen.at(i, j) = buffer.in(y, x, &interpolators2D::ibicubic);
		}
	}
}


operation_result Mammogram::GetImage(frame_t &img, const image_index_t idx)
{
	XRAD_ASSERT_THROW(idx.modality == modality_t::MG());

		img.MakeCopy(m_MM_Images()[idx.image_type]);
		return e_successful;
}





operation_result  Mammogram::GetBrightness(double *value, image_index_t idx, int y, int x)
{
//	XRAD_ASSERT_THROW(idx.modality == modality_t::MG());

	if (idx.modality != modality_t::MG())
	{
		throw modality_error("modality is MG");
	}

	frame_t img_screen;

	const string &image = idx.image_type;

	if (image != image_t::mg_rcc() && image != image_t::mg_lcc() && image != image_t::mg_rmlo() && image != image_t::mg_lmlo())
	{
		throw image_error("unknown image type");
	}


		x = range(x, 0, m_MM_Images()[image].sizes(1) - 1);
		y = range(y, 0, m_MM_Images()[image].sizes(0) - 1);

		*value = m_MM_Images()[image].at(y, x);

		return e_successful;
}