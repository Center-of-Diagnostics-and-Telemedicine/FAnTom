#ifndef FantomLibrary_h__
#define FantomLibrary_h__
/*!
	\file
	\date 2017/09/28 12:29
	\author kulberg
*/
#if 1

#include "FantomDefs.h"
#include "SliceDefs.h"

#ifndef FANTOM_DLL_EI
#if defined(XRAD_COMPILER_MSC)
#ifdef Fantom_DLL
	#define FANTOM_DLL_EI __declspec(dllexport)
#else
	#define FANTOM_DLL_EI __declspec(dllimport)
#endif
#elif defined(XRAD_COMPILER_GNUC)
#ifdef Fantom_DLL
	#define FANTOM_DLL_EI __attribute__((visibility("default")))
#else
	#define FANTOM_DLL_EI
#endif
#else
#define FANTOM_DLL_EI
#endif //_MSC_VER
#endif

XRAD_USING

extern "C"
{
//инициализация библиотеки: загрузка всех исследований по указанному пути
//operation_result FANTOM_DLL_EI InitFantom(const wstring &data_store_path);

//operation_result FANTOM_DLL_EI GetAccessionNumbers(vector<wstring> &accession_numbers);
// Если функция понадобится, переделать прототип. Не должно быть привязки к типам из Dicom,
// вернуть составные части структуры complete_study_id_t.
//operation_result FANTOM_DLL_EI GetStudiesIDs(vector<Dicom::complete_study_id_t> &accession_numbers);
//operation_result FANTOM_DLL_EI GetStudiesIDs(string &accession_numbers);
//	operation_result FANTOM_DLL_EI GetNumbersOfAccessions(vector<wstring> *accession_numbers);
//operation_result FANTOM_DLL_EI LoadCTbyAccession(const wstring &accession_number, bool &series_loaded);
//operation_result FANTOM_DLL_EI CloseCTStudyAcession(const wstring &accession_number);



// кол-во фреймов в соответствующем срезе
//operation_result FANTOM_DLL_EI GetNFrames_real(size_t &frames_no, slice_type st);
//operation_result FANTOM_DLL_EI GetNFrames_interpolated(size_t &frames_no, slice_type st);

//operation_result FANTOM_DLL_EI GetSlice(frame_t &interpolated_image, slice_type st, size_t rescaled_slice_no, double black, double white, double gamma, size_t aprox_size, mip_method_type mip_method);

//operation_result FANTOM_DLL_EI GetCoordinateNative(double &coord, slice_type st, size_t native_slice_position);
//operation_result FANTOM_DLL_EI GetCoordinateInterpolated(double &coord, slice_type st, size_t rescaled_slice_no);
//operation_result FANTOM_DLL_EI GetOriginalPixelCoordinate(size_t &pixel_coord, slice_type st, size_t rescaled_slice_no, bool interpolate_z);

//
//operation_result FANTOM_DLL_EI GetPointHU(double &value, size_t axial_coord, size_t frontal_coord, size_t sagittal_coord);
//operation_result FANTOM_DLL_EI GetInterpolatedPixel(size_t &rescaled_pixel_coord, slice_type st, size_t original_slice_no);
//operation_result FANTOM_DLL_EI GetPixelLengthCoefficient(double &length_pixel_coef);

// New implementation with 2D mode support ======================================================
operation_result FANTOM_DLL_EI InitHeap_N(const wstring& dicom_folder);//(const char *data_store_path)

operation_result FANTOM_DLL_EI HeapDump_N(const wstring& dump_file);

operation_result FANTOM_DLL_EI GetImage_N(frame_t &img, const image_index_t idx);

operation_result FANTOM_DLL_EI GetScreenImage_N(const unsigned char **img, int *length, image_index_t idx, double black, double white, double gamma, mip_index_t mip);

operation_result FANTOM_DLL_EI GetBrightness_N(double *value, image_index_t idx, size_t y, size_t x);

//Java Реализации
//note (Kovbas) для работы c Java сделан суффикс _J
//todo (Kovbas) выводить в консоль инфу о выполнении методов
operation_result FANTOM_DLL_EI InitFantom_J(const char *data_store_path); //todo (Kovbas) баблиотека должна получить путь к папке с исследованиями
operation_result FANTOM_DLL_EI GetStudiesIDs_J(char **accession_numbers, int *length); //todo (Kovbas) сделать, чтобы возвращался указатель на значения, разделённые \t


//! \brief Функция помещает в info_json_ptr указатель на строку, содержащую json c подробной информацией об исследовании
//!	для записи в БД разметки.
//!	Записываются ключи patient_id, patient_age, patient_sex, study_id, study_instance_uid, accession_number
operation_result FANTOM_DLL_EI GetDetailedStudyInfo_J(char **info_json_ptr, int *length);

operation_result FANTOM_DLL_EI LoadCTbyAccession_J(const char *assesion_number);

operation_result FANTOM_DLL_EI GetTomogramDimension_J(size_t *frames_no, slice_type st);

operation_result FANTOM_DLL_EI GetScreenDimension_J(size_t *frames_no, slice_type st);

operation_result FANTOM_DLL_EI GetSlice_J(const unsigned char **imgData, int *length, slice_type st, size_t rescaled_slice_no, double black, double white, double gamma, size_t aprox_size, mip_method_type mip_method);

operation_result FANTOM_DLL_EI GetPointHU_J(double *value, size_t axial_coord, size_t frontal_coord, size_t sagittal_coord);

operation_result FANTOM_DLL_EI GetMillimeterCoordinateFromTomogramPosition_J(double *coord, slice_type st, size_t native_slice_position);
operation_result FANTOM_DLL_EI GetDatabaseCoordinateFromScreenPosition_J(double *coord, slice_type st, size_t rescaled_slice_no);
operation_result FANTOM_DLL_EI GetTomogramLocationFromScreenCoordinate_J(size_t *pixel_coord, slice_type st, size_t rescaled_slice_no, bool interpolate_z);

//
//operation_result FANTOM_DLL_EI GetPointHU_J(double *value, size_t axial_coord, size_t frontal_coord, size_t sagittal_coord);
operation_result FANTOM_DLL_EI GetScreenCoordinateFromTomogramLocation_J(size_t *rescaled_pixel_coord, slice_type st, size_t original_slice_no);
operation_result FANTOM_DLL_EI GetPixelLengthCoefficient_J(double *length_pixel_coef);

//--------------------------------------------------------------------------------------------------



}

//test connecting with Java ================================================================================

struct tst_java_struct_t
{
	int i;
	double d;
};

extern "C"
{

// general numbers
void FANTOM_DLL_EI set_int(int i);
int FANTOM_DLL_EI get_int();
void FANTOM_DLL_EI set_double(double d);
double FANTOM_DLL_EI get_double();

//strings
void FANTOM_DLL_EI set_string(const char *val);
const FANTOM_DLL_EI char *get_string_ret();
void FANTOM_DLL_EI get_string_ptr(const char **result);

//wstring
void FANTOM_DLL_EI set_wstr(const char *val);
const char FANTOM_DLL_EI *get_tst_wstr_ptr();
void FANTOM_DLL_EI get_tst_wstr(const char **result);

//ustr
void FANTOM_DLL_EI set_ustr(const char *val);
const char FANTOM_DLL_EI *get_tst_ustr_ptr();
void FANTOM_DLL_EI get_tst_ustr(const char **result);

//u16
void FANTOM_DLL_EI set_u16(const char *val);

//pixdata
void FANTOM_DLL_EI init_tomogram();
//void FANTOM_DLL_EI get_pixeldata(unsigned char **pixelData, int &length, int &hs, int &vs, int &ncomp, int &bpp, int &sign);
//void FANTOM_DLL_EI get_pixeldata(unique_ptr<uint8_t> &pixelData, int &length, int &hs, int &vs, int &ncomp, int &bpp, int &sign);

}

#else

#define FANTOM_DLL_EI

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class Test */

#ifndef _Included_Test
#define _Included_Test
#ifdef __cplusplus
extern "C" {
#endif
/*
* Class:     Test
* Method:    GetNumNames
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_Test_GetNumNames
(JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif

#endif

#endif // FantomLibrary_h__
