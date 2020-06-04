#include "pre.h"
#include "FantomLibrary.h"

#include "FantomClass.h"

#include <sstream>
#include <iostream>
//#include <XRADBasic/Sources/Utils/BitmapContainer.h>

//TODO list<MarketTomogram> sessions;
Fantom	tomogram;


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

operation_result FANTOM_DLL_EI LoadXRbyAccession_J(const char *accession_number, bool *flag)
{
	return tomogram.LoadXRbyAccession(string8_to_wstring(accession_number), *flag);
}


operation_result FANTOM_DLL_EI GetXRSlice(RealFunction2D_F32 &img, size_t slice)
{
	img = tomogram.XRslice(slice);
	return e_successful;
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


