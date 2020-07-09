//--------------------------------------------------------------
#ifndef Fantom__File_FantomDefs_h
#define Fantom__File_FantomDefs_h
//--------------------------------------------------------------

#include <XRADBasic/MathFunctionTypes2D.h>

XRAD_USING

//--------------------------------------------------------------

typedef RealFunction2D_F32 frame_t;

enum operation_result
{
	e_successful,
	e_empty_pointer,
	e_out_of_range,
	e_other,
	e_other1,
	e_other2,
	e_other3,
};

enum slice_type
{
	//tomographic
	e_axial = 0,
	e_frontal = 1,
	e_sagittal = 2,

	//lung
	// выяснить правильное название проекций и их признаки в Dicom файлах

	//mammomgraphy
	// Для Fujifilm информация о типе проекции содержится в тэге
	// (0018,1400) Acquisition Device Processing Description LO
	//	RMLO                                              # 1, 4 
	//	для других аппаратов выяснять
	e_Rcc,
	e_Lcc,
	e_Rmlo,
	e_Lmlo,
};

enum mip_method_type
{
	e_average,
 	e_maxvalue,
	e_minvalue,//TODO сделать
	e_no_mip
};

//--------------------------------------------------------------
#endif // Fantom__File_FantomDefs_h
