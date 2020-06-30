#include "pre.h"
#include "FantomLibrary.h"
#include "FantomClass.h"

#include "CTomogram.h"
#include "XRay.h"
#include "Mamogram.h"

#include <sstream>
#include <iostream>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>

//#include <XRADBasic/Sources/Utils/BitmapContainer.h>


Fantom	tomogram;
unique_ptr<CTomogram> Study;

//operation_result FANTOM_DLL_EI GetSlice(frame_t &interpolated_image, 
//									   slice_type st, 
//									   size_t rescaled_slice_no, 
//									   double black, 
//									   double white, 
//									   double gamma, 
//									   size_t aprox_size, 
//									   mip_method_type mip_method)
//{
//	return tomogram.GetScreenSlice(interpolated_image, 
//							 st, 
//							 rescaled_slice_no, 
//							 black, 
//							 white, 
//							 gamma, 
//							 aprox_size, 
//							 mip_method);
//}


//operation_result FANTOM_DLL_EI GetStudyAccessionNumber(wstring &accession_number)
//{
//	return tomogram.GetStudyAccessionNumber(accession_number);
//}

//operation_result FANTOM_DLL_EI GetAccessionNumbers(vector<wstring> &accession_numbers)
//{
//	return tomogram.GetAccessionNumbers(accession_numbers);
//}

//operation_result FANTOM_DLL_EI GetStudiesIDs(vector<Dicom::complete_study_id_t> &study_ids)
//{
//	return tomogram.GetStudiesIDs(study_ids);
//}



//operation_result FANTOM_DLL_EI InitFantom(const wstring &data_store_path)
//{
//	return tomogram.InitFantom(data_store_path);
//}


//operation_result FANTOM_DLL_EI GetInterpolatedPixel(size_t &rescaled_pixel_coord, slice_type st, size_t original_slice_no)
//{
//	return tomogram.GetScreenCoordinateFromDicomLocation(rescaled_pixel_coord, st, original_slice_no);
//}


//operation_result FANTOM_DLL_EI LoadCTbyAccession(const wstring &accession_number, bool &series_loaded)
//{
//	return tomogram.LoadCTbyAccession(accession_number, series_loaded);
//}


//operation_result  FANTOM_DLL_EI GetNFrames_interpolated(size_t &frames_no, slice_type st)
//{
//	return tomogram.GetScreenDimension(frames_no, st);
//}


//operation_result  FANTOM_DLL_EI GetNFrames_real(size_t &frames_no, slice_type st)
//{
//	return tomogram.GetTomogramDimension(frames_no, st);
//}


//operation_result FANTOM_DLL_EI GetCoordinateNative(double &coord, slice_type st, size_t native_slice_position)
//{
//	return tomogram.GetMillimeterCoordinateFromTomogramPosition(coord, st, native_slice_position);
//}
////operation_result FANTOM_DLL_EI GetNumbersOfAccessions(vector<wstring> *accession_numbers)
//{
//	return tomogram.GetNumbersOfAccessions(*accession_numbers);
//}

//operation_result FANTOM_DLL_EI GetPixelLengthCoefficient(double &length_pixel_coef)
//{
//	return tomogram.GetPixelLengthCoefficient(length_pixel_coef);
//}


//operation_result FANTOM_DLL_EI GetCoordinateInterpolated(double &coord, slice_type st, size_t rescaled_slice_no)
//{
//	return tomogram.GetDatabaseCoordinateFromScreenPosition(coord, st, rescaled_slice_no);
//}


//operation_result FANTOM_DLL_EI GetOriginalPixelCoordinate(size_t &pixel_coord, slice_type st, size_t rescaled_slice_no, bool interpolate_z)
//{
//	return tomogram.GetDicomLocationFromScreenCoordinate(pixel_coord, st, rescaled_slice_no, interpolate_z);
//}
// New implementation with 2D mode support ======================================================


//auxiliary_tomogram_acquisition_condition;


/*
class	auxiliary_instance_condition : public dicom_instance_condition
{
public:
	bool	check(const Dicom::instance_ptr &a) const override
	{
		{
			// Выявление не томограмм.
			// Любой instance внутри acquisition, не являющийся срезом томограммы,
			// делает весь acquisition "вспомогательным".
			auto *slice = dynamic_cast<Dicom::tomogram_slice*>(a.get());
//			if (!slice)
//				return true;

			// Выявление локализаторов
			auto image_type = slice->dicom_container()->get_wstring_values(Dicom::e_image_type);
			if (image_type.size() < 3) return true;
			if (image_type[2] == L"LOCALIZER") return true;

			// Выявление вспомогательных изображений вроде dose report
			if (slice->dicom_container()->get_double_values(Dicom::e_image_position_patient).size() != 3)
				return true;//вектор положения отсутствует
			if (slice->dicom_container()->get_double_values(Dicom::e_image_orientation_patient).size() != 6)
				return true;//вектор ориентации отсутствует.
		}


	}
	dicom_instance_condition	*clone() const override { return new auxiliary_instance_condition(); }
};
*/

class	allowed_modalities : public dicom_instance_condition
{
public:
	bool	check(const Dicom::instance_ptr &a) const override
	{
		wstring modality = a->modality();
		return modality == L"CT" || modality == L"DX" || modality == L"MG";
	}
	allowed_modalities	*clone() const override { return new allowed_modalities(); }
};


inline dicom_instance_predicate	fantom_allowed_modality(bool in_direct = true)
{
	return predicate::checker<Dicom::instance_ptr>(allowed_modalities(), in_direct);
}


inline auto	RemoveAux() 
{ 
	return	make_shared<AcquisitionFilter>(tomogram_acquisition_is_auxiliary());
}

inline DicomInstanceFilters_t RemoveNonFantomModalities()
{
	return make_tuple<>(
		Dicom::filter_t(),
		fantom_allowed_modality() & !instance_is_multiframe()//,
						);
}

/*inline DicomInstanceFilters_t MakeDicomInstanceFilters()
{
	return make_tuple<>(
		Dicom::filter_t(),
		Dicom::dicom_instance_predicate::true_predicate()//,
														 //Dicom::dicom_acquisition_predicate::true_predicate()
		);
}*/

operation_result FANTOM_DLL_EI InitStudy_N(const wstring& dicom_folder)//(const char *data_store_path)
{
	Dicom::patients_loader patients_heap = GetDicomStudiesHeap(
		Dicom::datasource_folder(dicom_folder, true),
		RemoveNonFantomModalities(),
			//  RemoveNonFantomModalities(),
			//	MakeDicomInstanceFilters(), // вместо пустого фильтр, который оставит только наши рабочие модальности
		ConsoleProgressProxy());

     //FilterDicoms(patients_heap, RemoveAux);
	FilterDicoms(patients_heap, RemoveAux());
	/*
	1. 
	2.	список пациентов и исследований. Должно быть 1, 1. Иначе ошибка
	3.	Подсчет модальностей. Должна быть одна.

	if(modality == "CT") Study = make_unique(CTomogram<>())
	else if("MG")... (MG//)
	*/

	Study = make_unique<CTomogram>();
	return Study->InitHeap(dicom_folder);
}


operation_result FANTOM_DLL_EI InitHeap_N(const wstring& dicom_folder)//(const char *data_store_path)
{
	Study = make_unique<CTomogram>();

	Init2DInterpolators(ConsoleProgressProxy()); //вызывать только здесь т.к. результат должен относиться к глобальным переменным dll
	
	return Study->InitHeap(dicom_folder);
}



operation_result FANTOM_DLL_EI HeapDump_N(const wstring& dump_file)
{
	if (!Study) return e_empty_pointer;

	return Study->HeapDump(dump_file);
}

operation_result FANTOM_DLL_EI GetAccNumber_N(size_t no, wstring &acc_no)
{
	if (!Study) return e_empty_pointer;

	return Study->GetAccNumber(no, acc_no);
}

operation_result FANTOM_DLL_EI LoadByAccession_N(const wstring accession_number)
{
	if (!Study) return e_empty_pointer;

	return Study->LoadByAccession(accession_number);
}

operation_result FANTOM_DLL_EI GetModality_N(string &modality)
{
	if (!Study) return e_empty_pointer;

	return Study->GetModality(modality);
}

operation_result FANTOM_DLL_EI GetImage_N(frame_t &img, const image_index_t idx)
{
	if (!Study) return e_empty_pointer;

	return Study->GetImage(img, idx);
}

operation_result FANTOM_DLL_EI GetScreenImage_N(const unsigned char **img, int *length, image_index_t idx, brightness brightness)
{
	if (!Study) return e_empty_pointer;

	return Study->GetScreenImage(img, length, idx, brightness);
}

operation_result FANTOM_DLL_EI GetBrightness_N(double *value, image_index_t idx, size_t y, size_t x)
{
	if (!Study) return e_empty_pointer;

	return Study->GetBrightness(value, idx, y, x);
}

operation_result FANTOM_DLL_EI GetTomogramDimensions_N(point3_ST &dimensions)
{
	if (!Study) return e_empty_pointer;

	return Study->GetTomogramDimensions(dimensions);
}

operation_result FANTOM_DLL_EI GetScreenDimensions_N(point3_ST &dimensions)
{
	if (!Study) return e_empty_pointer;

	return Study->GetScreenDimensions(dimensions);
}

operation_result FANTOM_DLL_EI GetPixelLengthCoefficient_N(double &length_pixel)
{
	if (!Study) return e_empty_pointer;

	return Study->GetPixelLengthCoefficient(length_pixel);
}

operation_result FANTOM_DLL_EI  GetZFlip_N(bool & flip)
{
	if (!Study) return e_empty_pointer;

	return Study->GetZFlip(flip);
}


// Java implementation ==========================================================================
operation_result FANTOM_DLL_EI InitFantom_J(const char *data_store_path)
{
	return tomogram.InitFantom_J(data_store_path);
}

operation_result FANTOM_DLL_EI GetStudiesIDs_J(char **accession_numbers, int *length)
{
	return tomogram.GetStudiesIDs_J(accession_numbers,*length);
}

operation_result FANTOM_DLL_EI GetDetailedStudyInfo_J(char **info_json_ptr, int *length)
{
	return tomogram.GetDetailedStudyInfo_J(info_json_ptr, *length);
}

operation_result FANTOM_DLL_EI LoadCTbyAccession_J(const char *accession_number)
{
	bool tmp;
	return tomogram.LoadCTbyAccession(string8_to_wstring(accession_number),tmp);
}

operation_result FANTOM_DLL_EI GetSlice_J(const unsigned char **imgData, int *length, slice_type st, size_t rescaled_slice_no, double black, double white, double gamma, size_t aprox_size, mip_method_type mip_method)
{
	return tomogram.GetSlice_J(imgData, *length, st, rescaled_slice_no, black, white, gamma, aprox_size, mip_method);
}

operation_result  FANTOM_DLL_EI GetTomogramDimension_J(size_t *frames_no, slice_type st)
{
	return tomogram.GetTomogramDimension(*frames_no, st);
}

operation_result  FANTOM_DLL_EI GetScreenDimension_J(size_t *frames_no, slice_type st)
{
	return tomogram.GetScreenDimension(*frames_no, st);
}

operation_result FANTOM_DLL_EI GetPointHU_J(double *value, size_t axial_coord, size_t frontal_coord, size_t sagittal_coord)
{
	//point3_ST coord(axial_coord, frontal_coord, sagittal_coord);
	return tomogram.GetTomogramSampleHU(*value, point3_ST(axial_coord, frontal_coord, sagittal_coord));
}

operation_result FANTOM_DLL_EI GetMillimeterCoordinateFromTomogramPosition_J(double *coord, slice_type st, size_t native_slice_position)
{
	return tomogram.GetMillimeterCoordinateFromTomogramPosition(*coord, st, native_slice_position);
}

operation_result FANTOM_DLL_EI GetDatabaseCoordinateFromScreenPosition_J(double *coord, slice_type st, size_t rescaled_slice_no)
{
	return tomogram.GetDatabaseCoordinateFromScreenPosition(*coord, st, rescaled_slice_no);
}

operation_result FANTOM_DLL_EI GetTomogramLocationFromScreenCoordinate_J(size_t *pixel_coord, slice_type st, size_t rescaled_slice_no, bool interpolate_z)
{
	return tomogram.GetDicomLocationFromScreenCoordinate(*pixel_coord, st, rescaled_slice_no, interpolate_z);
}

operation_result FANTOM_DLL_EI GetScreenCoordinateFromTomogramLocation_J(size_t *rescaled_pixel_coord, slice_type st, size_t original_slice_no)
{
	return tomogram.GetScreenCoordinateFromDicomLocation(*rescaled_pixel_coord, st, original_slice_no);
}

operation_result FANTOM_DLL_EI GetPixelLengthCoefficient_J(double *length_pixel_coef)
{
	return tomogram.GetPixelLengthCoefficient(*length_pixel_coef);
}















//test connecting with Java ================================================================================
#if 0
shared_cfile dump_file(L"Fantom_dump", L"wb");
shared_cfile tst_file(L"Fantom_tst_file.txt", L"w");

template<class T>
void to_file(T val)
{
	wstring tmpStr = to_wstring(val) + L"\n";
	tst_file.write(tmpStr.c_str(), tmpStr.size(), sizeof(*tmpStr.c_str()));
}
tst_java_struct_t tst_struct;
string tst_string;

// general numbers
void FANTOM_DLL_EI set_int(int i)
{
	tst_struct.i = i;
	to_file(tst_struct.i);
}
int FANTOM_DLL_EI get_int()
{
	return tst_struct.i;
}
void FANTOM_DLL_EI set_double(double d)
{
	tst_struct.d = d;
	to_file(tst_struct.d);
}
double FANTOM_DLL_EI get_double()
{
	return tst_struct.d;
}

//strings
void FANTOM_DLL_EI set_string(const char *val)
{
	tst_string = val;
	tst_file.write((tst_string + "\n").c_str(), tst_string.size() + 1, sizeof(*tst_string.c_str()));
}

const FANTOM_DLL_EI char *get_string_ret()
{
	return tst_string.c_str();
}

void FANTOM_DLL_EI get_string_ptr(const char **result)
{
	*result = tst_string.c_str();
}


//wstring
wstring tst_wstr(L"что-то");
void FANTOM_DLL_EI set_wstr(const char *val)
{
	dump_file.write(val, 100, sizeof(val));
	tst_wstr = convert_to_wstring(val);
}

const char FANTOM_DLL_EI *get_tst_wstr_ptr()
{
	return convert_to_string(tst_wstr).c_str();
}

void FANTOM_DLL_EI get_tst_wstr(const char **result)
{
	*result = convert_to_string(tst_wstr).c_str();
}

ustring tst_ustr;
void FANTOM_DLL_EI set_ustr(const char *val)
{
	tst_ustr = convert_to_ustring(val);
}

const char FANTOM_DLL_EI *get_tst_ustr_ptr()
{
	return convert_to_string(tst_ustr).c_str();
}

void FANTOM_DLL_EI get_tst_ustr(const char **result)
{
	*result = convert_to_string(tst_ustr).c_str();
}

void FANTOM_DLL_EI set_u16(const char *val)
{
	shared_cfile dumpu8(L"dumpu08", L"wb"), dumpu16(L"dumpu16", L"wb");
	dumpu8.write(val, 100, sizeof(val));
	/*wstring u16str;
	u16str = string8_to_wstring(string8(val));
	dumpu16.write(u16str.c_str(), 100, sizeof(*u16str.c_str()));*/

	wstring wstr = (L"wstr tst");
	u16string u16str_;
	u16str_ = u"ttt";
	char16_t c16 = L't';
	u16str_ += c16;

}
#endif

void FANTOM_DLL_EI init_tomogram()
{
	tomogram.InitFantom(L"D:/websrv_Fantom/data_store");
	bool loaded;
	tomogram.LoadCTbyAccession(L"AGFA000000691600", loaded);
}


