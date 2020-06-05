//--------------------------------------------------------------
#ifndef NewClassDefs_h__
#define NewClassDefs_h__
//--------------------------------------------------------------

#include <XRADBasic/MathFunctionTypes2D.h>

XRAD_USING

//--------------------------------------------------------------


typedef RealFunction2D_F32 frame_t;

enum modality_t
{
	CT,
	DX,
	MG
};

enum image_t
{
	//tomographic
	e_ct_axial = 0,
	e_ct_frontal = 1,
	e_ct_sagittal = 2,

	//lung
	// выяснить правильное название проекций и их признаки в Dicom файлах

	e_dx_generic,
	e_dx_postero_anterior,
	e_dx_left_lateral,
	e_dx_right_lateral,


	//mammomgraphy
	// Для Fujifilm информация о типе проекции содержится в тэге
	// (0018,1400) Acquisition Device Processing Description LO
	//	RMLO                                              # 1, 4 
	//	для других аппаратов выяснять
	e_mg_Rcc,
	e_mg_Lcc,
	e_mg_Rmlo,
	e_mg_Lmlo,
};

struct image_index_t
{
	modality_t	modality;
	image_t	image_type;
	size_t	image_no;
};


#endif // NewClassDefs_h__