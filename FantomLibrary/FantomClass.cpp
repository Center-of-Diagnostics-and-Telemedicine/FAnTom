#include "pre.h"
#include "FantomClass.h"

#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>
#include <XRADBasic/ThirdParty/nlohmann/json.hpp>

#include <cstring>
#include <iostream>
#include <sstream>
#include "FantomLogger.h"


//void GetDicomStudiesVector(std::vector<Dicom::study_loader> &m_studies_heap, const wstring &root_folder_name, bool analyze_subfolders, ProgressProxy progress_proxy);
//TODO эта функция используется единственный раз в проекте Fantom. Уместно ли ради единственного случая ее держать? (Kovbas) я думаю, что её можно перенести в Fantom, когда будем активно продолжать с ним работы.

point2_ST	slice_manager::slice_sizes(slice_type st, size_t no) const
{
	switch(st)
	{
		case	e_axial:
			return	point2_ST(m_slices.sizes(1), m_slices.sizes(2));
		case	e_frontal:
			return	point2_ST(m_slices.sizes(0), m_slices.sizes(2));
		case	e_sagittal:
			return	point2_ST(m_slices.sizes(0), m_slices.sizes(1));
		default:
			throw std::invalid_argument("unknown slice type");
	}
}

size_t	slice_manager::n_slices(slice_type st) const
{
	switch(st)
	{
		case	e_axial:
			return	m_slices.sizes(0);
		case	e_frontal:
			return	m_slices.sizes(1);
		case	e_sagittal:
			return	m_slices.sizes(2);
		default:
			throw std::invalid_argument("unknown slice type");
	}
}

frame_t	slice_manager::slice(slice_type st, size_t no)
{
	switch(st)
	{
		case	e_axial:
			return	m_slices.GetSlice({no, slice_mask(0), slice_mask(1)});
		case	e_frontal:
			return	m_slices.GetSlice({slice_mask(0), no, slice_mask(1)});
		case	e_sagittal:
			return	m_slices.GetSlice({slice_mask(0), slice_mask(1), no});
		default:
			throw std::invalid_argument("unknown slice type");
	}
}


void GetDicomStudiesVector(std::vector<Dicom::study_loader> &studies_heap, const wstring &src_folder_name, bool analyze_subfolders, ProgressProxy progress_proxy)
{
	Dicom::patients_loader patients_heap = GetDicomStudiesHeap(
			Dicom::datasource_folder(src_folder_name, analyze_subfolders),
			MakeDicomInstanceFilters(),
			progress_proxy);

	for (auto &patient : patients_heap)
	{
		for (auto &study : patient)
		{
			studies_heap.push_back(study);
		}
	}
}


operation_result Fantom::InitFantom(const wstring &data_store_path)
{
	//bool	subfolders = true;
//	sp = ConsoleProgressProxy();
	GetDicomStudiesVector(m_studies_heap, data_store_path, true, ConsoleProgressProxy());
	Init2DInterpolators(ConsoleProgressProxy());

	return e_successful;
}

operation_result Fantom::GetAccessionNumbers(vector<wstring> &accession_numbers)
{
	if (m_studies_heap.size() == 0) return e_other;
	for (auto &study: m_studies_heap)
	{
		accession_numbers.push_back(study.complete_study_id().accession_number());
	}
	return e_successful;
}

operation_result Fantom::GetStudiesIDs(vector<Dicom::complete_study_id_t> &accession_numbers)
{
	if (m_studies_heap.size() == 0) return e_other;
	for (auto &study: m_studies_heap)
	{
		accession_numbers.push_back(study.complete_study_id());
	}
	return e_successful;
}




int Fantom::GetStudiesNumber()
{
	return int(m_studies_heap.size());
}

size_t slice_manager::GetAccessionHeapPosition(const wstring &accession_number, bool &series_found)
{
	series_found = false;
	size_t series_position = 0;
	for (size_t i = 0; i < m_studies_heap.size(); ++i)
	{
		if ((accession_number == m_studies_heap[i].complete_study_id().accession_number()) || (accession_number == m_studies_heap[i].complete_study_id().study_id()))
		{
			series_found = true;
			series_position = i;
		}
	}
	if (!series_found)
	{
		return 0;
	}
	else
	{
		return series_position;
	}
//#error see comment below
	//TODO Здесь (если убрать строку выше) компилятор дает предупреждение, и неспроста! логика не до конца продумана.
	//	1.	Что в переменной series_found до начала поиска? Надо инициализировать или написать комментарий,
	//		что эта переменная инициализируется до вызова функции
	//	2.	Если она проинициализирована до вызова в true, а потом ничего найти не удалось,
	//		функция ничего не возвращает, хотя должна. Нельзя игнорировать такие предупреждения!
}


// получение acquisition с самым большим размером
Dicom::acquisition_loader &GetLargestAcquisition(Dicom::study_loader &study)
{
	Dicom::acquisition_loader *result = &study.front().front().front();
	size_t sz{ result->size() };

	for (auto &ser : study)
	{
		for (auto &stack : ser)
		{
			for (auto &acq : stack)
			{
				if (acq.size() > sz)
				{
					result = &acq;
					sz = acq.size();
				}
			}
		}
	}
	return *result;
}

//Загрузка данных выбранного КТ в свойства текущего объекта
operation_result slice_manager::LoadCTbyAccession(const wstring &accession_number, bool &series_loaded)
{
//	std::lock_guard<std::mutex>  lg(m_slice_manager_mutex);

	if (m_accession_number == accession_number && proc_acquisition_work_ptr != nullptr)
	{
		series_loaded = true;
		return e_successful;
	}
	
	size_t chosen_accession_number = GetAccessionHeapPosition(accession_number, series_loaded);

	if (!series_loaded) return e_out_of_range;

	proc_acquisition_work_ptr = CreateProcessAcquisition(GetLargestAcquisition(m_studies_heap[chosen_accession_number]), ConsoleProgressProxy());
	proc_acquisition_work_ptr->open_instancestorages();

	m_slices = ct_acquisition_ptr().slices();
	m_scales = ct_acquisition_ptr().scales();
	m_image_positions_patient = ct_acquisition_ptr().image_positions_patient();

	CalculateInterpolationScales();

	m_accession_number = accession_number;

	auto	&sample_instance = *ct_acquisition_ptr().loader().front();

	m_patient_id = sample_instance.get_wstring(Dicom::e_patient_id);
	m_patient_sex = sample_instance.get_wstring(Dicom::e_patient_sex);
	m_patient_age = sample_instance.get_wstring(Dicom::e_patient_age);
	m_study_id = sample_instance.get_wstring(Dicom::e_study_id);
	m_study_instance_uid = sample_instance.get_wstring(Dicom::e_study_instance_uid);

	cout << "\n\nPatient info:\n";
	cout << convert_to_string8(m_patient_id) << endl;
	cout << convert_to_string8(m_patient_sex) << endl;
	cout << convert_to_string8(m_patient_age) << endl;

	cout << "\nstudy info:\n";
	cout << convert_to_string8(m_study_id) << endl;
	cout << convert_to_string8(m_accession_number) << endl;
	cout << convert_to_string8(m_study_instance_uid) << endl;

	return e_successful;
}

// ось, перпендикулярная данному срезу
slice_manager::axis_t	slice_manager::normal_axis(slice_type st) const
{
	switch(st)
	{
		case e_axial:	return e_z;
		case e_frontal:	return e_y;
		case e_sagittal: return e_x;

		default:
			XRAD_ASSERT_THROW_M(false, invalid_argument, "Unknown axis type");
	}
}

// срез, перпендикулярный данной оси
slice_type	slice_manager::normal_slice(slice_manager::axis_t at) const
{
	switch(at)
	{
		case e_z: return e_axial;
		case e_y: return e_frontal;
		case e_x: return e_sagittal;

		default:
			XRAD_ASSERT_THROW_M(false, invalid_argument, "Unknown axis type");
	}
}




operation_result Fantom::GetScreenCoordinateFromDicomLocation(size_t &tomogram_location, slice_type st, size_t original_slice_no)
{
	tomogram_location = dicom_to_screen_coordinate(original_slice_no, normal_axis(st));
	return e_successful;
}

//TODO Нужно перевести все z координаты из номеров среза в миллиметры
operation_result Fantom::GetDicomLocationFromScreenCoordinate(size_t &dicom_coord, slice_type st, size_t rescaled_slice_no, bool interpolate)
{
	//Получение координат точки на томограмме по точке на растре
	if(interpolate)
	{
		dicom_coord = screen_to_dicom_coordinate(rescaled_slice_no, normal_axis(st));
	}
	else
	{
		dicom_coord = rescaled_slice_no;
	}

	return e_successful;
}

double	slice_manager::dicom_to_screen_coordinate(double t, axis_t axis)
{
	switch(axis)
	{
		case e_z:
			return double(b_flip_z ? m_slices.sizes(0) - t-1 : t) * m_interpolation_factor.z();

		case e_y:
			return double(t) * m_interpolation_factor.y();

		case e_x:
			return double(t) * m_interpolation_factor.x();
	}
	XRAD_ASSERT_THROW_M(false, invalid_argument, "Unknown axis index");
}

double	slice_manager::screen_to_dicom_coordinate(double t, axis_t axis)
{
	switch(axis)
	{
		case e_z:
		{
			double	u = t / m_interpolation_factor.z();
			return double(b_flip_z ? m_slices.sizes(0) - u-1 : u);
		}

		case e_y:
			return double(t) / m_interpolation_factor.y();

		case e_x:
			return double(t) / m_interpolation_factor.x();
	}
	XRAD_ASSERT_THROW_M(false, invalid_argument, "Unknown axis index");
}


operation_result Fantom::RescaleImageFromTomogramToScreenCoordinates(frame_t &rescaled_image, const frame_t &tomogram_slice, slice_type st)
{
	axis_t	v, h;

	switch (st)
	{
	case e_frontal:
		v = e_z;
		h = e_x;
		break;

	case e_sagittal:
		v = e_z;
		h = e_y;
		break;

	case e_axial:
	default:
		v = e_y;
		h = e_x;
		break;
	}
	for (size_t i = 0; i < rescaled_image.vsize(); ++i)
	{
		double y = screen_to_dicom_coordinate(i, v);
		for (size_t j = 0; j < rescaled_image.hsize(); ++j)
		{
			double x = screen_to_dicom_coordinate(j, h);
			rescaled_image.at(i,j) = tomogram_slice.in(y, x, &interpolators2D::ibicubic);
		}
	}

	return e_successful;

}

operation_result  slice_manager::CalculateMIPFrame(frame_t &frame, double dicom_slice_position, slice_type st, size_t mip_size, mip_method_type mmt)
{
	auto	mip_function = [&mmt](const RealFunctionF32	 &row)->double
	{
		switch(mmt)
		{
			case e_average:
				return AverageValue(row);
			case e_maxvalue:
				return MaxValue(row);
			case e_minvalue:
				return MinValue(row);
			default:
			case e_no_mip:
				return row[row.size()/2];
		}
	};

	// используется двухступенчатая процедура извлечения подмножества, поэтому два разных буфера. неуклюже, можно подумать об улучшении
	RealFunctionMD_F32	acquisition_buffer;
	RealFunctionMD_F32	b1;

	auto	frame_sizes = slice_sizes(st, dicom_slice_position);
	frame.realloc(frame_sizes.y(), frame_sizes.x(), 0);

	size_t	p0 = range(dicom_slice_position-mip_size, 0, n_slices(st)-1);
	size_t	p1 = range(dicom_slice_position+mip_size, 0, n_slices(st)-1);


	switch (st)
	{
	case e_axial:
		b1.UseDataFragment(m_slices, {p0, 0, 0}, {p1, m_slices.sizes(1), m_slices.sizes(2)});
 		b1.GetSubset(acquisition_buffer, {slice_mask(0), slice_mask(1), slice_mask(2)});
		break;

	case e_frontal:
		b1.UseDataFragment(m_slices, {0, p0, 0}, {m_slices.sizes(0), p1, m_slices.sizes(2)});
		b1.GetSubset(acquisition_buffer, {slice_mask(1), slice_mask(0), slice_mask(2)});
		break;

	case e_sagittal:
		b1.UseDataFragment(m_slices, {0, 0, p0}, {m_slices.sizes(0), m_slices.sizes(1), p1});
		b1.GetSubset(acquisition_buffer, {slice_mask(1), slice_mask(2), slice_mask(0)});
		break;
	}


	for(size_t i = 0; i < frame.vsize(); ++i)
	{
		for(size_t j = 0; j < frame.hsize(); ++j)
		{
			frame.at(i, j) = mip_function(acquisition_buffer.GetRow({slice_mask(0),i, j}));
		}
	}

	return e_successful;
}

//auto	get_iv(const )

operation_result Fantom::GetTomogramSlice(frame_t &frame,
	double tomogram_slice_position,
	slice_type st,
	size_t aprox_size,
	mip_method_type mip_method)
{
	size_t	slice_no = size_t(tomogram_slice_position);
	//TODO заменить на пересчет из z?

	if (aprox_size)
	{
		CalculateMIPFrame(frame, tomogram_slice_position, st, aprox_size, mip_method);
	}
	else
	{
		frame.MakeCopy(slice(st, slice_no));
	}
	return e_successful;
}


operation_result Fantom::GetScreenSlice(frame_t &screen_image_buffer,
			slice_type st,
			size_t dicom_slice_position,
			double black,
			double white,
			double gamma,
			size_t slice_aprox,
			mip_method_type mip_method)
{
	switch (st)
	{
	case e_frontal:
		screen_image_buffer.realloc(interpolation_sizes.z(), interpolation_sizes.x());
		break;

	case e_sagittal:
		screen_image_buffer.realloc(interpolation_sizes.z(), interpolation_sizes.y());
		break;

	case e_axial:
	default:
		screen_image_buffer.realloc(interpolation_sizes.y(), interpolation_sizes.x());
		break;
	}
	frame_t	intermediate_buffer;
	GetTomogramSlice(intermediate_buffer, dicom_slice_position, st, slice_aprox, mip_method);
	RescaleImageFromTomogramToScreenCoordinates(screen_image_buffer, intermediate_buffer, st);
	ApplyFunction(screen_image_buffer, [black, white](float x) {return x<black? 0: x>white? 255 : 255.*(x - black) / (white - black); });
	ApplyFunction(screen_image_buffer, [gamma](float x) {return 255.*pow(x / 255., gamma); });
	return e_successful;
}

operation_result Fantom::GetScreenDimension(size_t &frames_no, slice_type st)
{
	switch (st)
	{
	case e_frontal:
		frames_no = interpolation_sizes.y();
		break;
	case e_sagittal:
		frames_no = interpolation_sizes.x();
		break;
	case e_axial:
		frames_no = interpolation_sizes.z();
		break;
	default:
		XRAD_ASSERT_THROW_M(false, invalid_argument, "unknown slice type");
	}
	return e_successful;
}

operation_result Fantom::GetTomogramDimension(size_t &frames_no, slice_type st)
{
	switch (st)
	{
	/*case e_axial:
		frames_no = m_slices.sizes(0);
		break;*/
	case e_frontal:
		frames_no = slices().sizes(1);
		break;
	case e_sagittal:
		frames_no = slices().sizes(2);
		break;
	case e_axial:
	default:
		frames_no = slices().sizes(0);
		break;
	}
	return e_successful;
}

operation_result Fantom::GetTomogramSampleHU(double &value, point3_ST tomogram_sample_position)
{
	tomogram_sample_position.x() = range(tomogram_sample_position.x(), 0, slices().sizes(2)-1);
	tomogram_sample_position.y() = range(tomogram_sample_position.y(), 0, slices().sizes(1)-1);
	tomogram_sample_position.z() = range(tomogram_sample_position.z(), 0, slices().sizes(0)-1);


	value = slices().at({tomogram_sample_position.z(), tomogram_sample_position.y(), tomogram_sample_position.x()});

	return e_successful;
}

operation_result Fantom::GetPixelLengthCoefficient(double &length_pixel)
{
	length_pixel = m_scales.y();
	return e_successful;
}

operation_result Fantom::GetMillimeterCoordinateFromTomogramPosition(double &coord, slice_type st, size_t tomogram_sample_position)
{
	tomogram_sample_position -= 1;
	switch (st)
	{
	case e_axial:
		coord = m_image_positions_patient[tomogram_sample_position].z();
		break;
	case e_frontal:
		coord = tomogram_sample_position*m_scales.y();
		break;
	case e_sagittal:
		coord = tomogram_sample_position*m_scales.x();
		break;
	default:
		coord = m_image_positions_patient[tomogram_sample_position].z();
		break;
	}
	return e_successful;
}

operation_result Fantom::GetDatabaseCoordinateFromScreenPosition(double &coord, slice_type st, size_t screen_coordinate)
{
	double	tomogram_coordinate = screen_to_dicom_coordinate(screen_coordinate, normal_axis(st));

	if (st == e_axial)
	{
		coord = m_image_positions_patient[size_t(tomogram_coordinate)].z();
	}
	else
	{
		coord = tomogram_coordinate;
	}
	return e_successful;
}


operation_result Fantom::GetStudyAccessionNumber(wstring &accession_number)
{
	if ((ct_acquisition_ptr().accession_number() != L""))
	{
		accession_number = ct_acquisition_ptr().accession_number(), e_encode_literals;
		return e_successful;
	}
	else return e_other;
}

operation_result slice_manager::CalculateInterpolationScales()
{
	m_interpolation_factor.CopyData(m_scales / min(m_scales.x(), m_scales.y()));

	interpolation_sizes =
	{
		size_t(m_slices.sizes(0) * m_interpolation_factor.z()),
		size_t(m_slices.sizes(1) * m_interpolation_factor.y()),
		size_t(m_slices.sizes(2) * m_interpolation_factor.x())
	};

	// учет укладки пациента
	double	position_0 = m_image_positions_patient[0].z();
	double	position_1 = m_image_positions_patient[m_image_positions_patient.size()-1].z();
	b_flip_z = position_0 < position_1;

	return e_successful;
}



// Java ========================================================================================================
operation_result  Fantom::InitFantom_J(const char *data_store_path)
{
	START_LOG;
	InitFantom(convert_to_wstring(data_store_path));
	END_LOG;
	return e_successful;
}


std::string slice_manager::DetailedStudyInfo()
{
	nlohmann::json	j;
	nlohmann::json	tube_current;
	stringstream	str;
	//make indents in json
	str.width(1);
	str.fill('\t');
	
	auto	currents = ct_acquisition_ptr().currents();

	tube_current["min_tube_current"] = MinValue(currents);
	tube_current["max_tube_current"] = MaxValue(currents);
	tube_current["average_tube_current"] = AverageValue(currents);
	std::sort(currents.begin(), currents.end());
	tube_current["median_tube_current"] = currents[currents.size()/2];

	j["accession_number"] = convert_to_string8(m_accession_number);
	j["study_id"] = convert_to_string8(m_study_id);
	j["study_instance_uid"] = convert_to_string8(m_study_instance_uid);
	j["patient_id"] = convert_to_string8(m_patient_id);
	j["patient_sex"] = convert_to_string8(m_patient_sex);
	j["patient_age"] = convert_to_string8(m_patient_age);
	j["tube_current"] = tube_current;
	str << j;

	return str.str();
}



operation_result Fantom::GetDetailedStudyInfo_J(char **info_json_p, int &length)
{
	START_LOG;

	string string_buffer = DetailedStudyInfo();

	length = int(string_buffer.size()+1);
	
	buffer_detailed_study_info = make_unique<char[]>(length);
	memcpy(buffer_detailed_study_info.get(), string_buffer.c_str(), length);
	*info_json_p = buffer_detailed_study_info.get();

	END_LOG;

	return e_successful;
}


operation_result Fantom::GetStudiesIDs_J(char **studies_ids_p, int &length)
{
	START_LOG;

	vector<Dicom::complete_study_id_t> study_ids;
	GetStudiesIDs(study_ids);
	string string_buffer;
	for (auto &study_id : study_ids)
	{
//		string_buffer += convert_to_string8(study_id.study_instance_uid()) + '\t' + convert_to_string8(study_id.study_id()) + '\t' + convert_to_string8(study_id.accession_number()) + '\n';
		//TODO следующая строчка временно. Нужно вернуть то, что выше, предварительно наладив разбор на стороне котлина
		string_buffer += convert_to_string8(study_id.accession_number()) + '\t';
	}

	length = int(string_buffer.size());
	buf_ct_accession_numbers = make_unique<char[]>(length);
	memcpy(buf_ct_accession_numbers.get(), string_buffer.c_str(), length);
	*studies_ids_p = buf_ct_accession_numbers.get();

	END_LOG;
	return e_successful;
}

operation_result  Fantom::LoadCTbyAccession_J(const char *accession_number)
{
	START_LOG;
	bool res;
	LoadCTbyAccession(convert_to_wstring(accession_number), res);

	END_LOG;
	return e_successful;
}

operation_result Fantom::GetSlice_J(
	const unsigned char **imgData,
	int &length,
	slice_type st,
	size_t dicom_slice_no,
	double black,
	double white,
	double gamma,
	size_t slice_aprox,
	mip_method_type mip_method)
{
	START_LOG;
	frame_t screen_image;
	if (GetScreenSlice(screen_image, st, dicom_slice_no, black, white, gamma, slice_aprox, mip_method) != e_successful)
	{
		return e_other;
	}

	BitmapContainerIndexed	bmp;
	bmp.SetSizes(screen_image.vsize(), screen_image.hsize());
	bmp.palette.realloc(256);
	for (size_t i = 0; i < 256; ++i)
	{
		bmp.palette[i] = static_cast<uint8_t>(i);
	}
	bmp.CopyData(screen_image);

	length = static_cast<int>(bmp.GetBitmapFileSize());
	bitmap_buffers[st] = make_unique<unsigned char[]>(length);
	memcpy(bitmap_buffers[st].get(), bmp.GetBitmapFile(), length);
	*imgData = bitmap_buffers[st].get();
	END_LOG;
	return e_successful;
}

