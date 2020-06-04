//--------------------------------------------------------------
#ifndef NewClassDef_h__
#define NewClassDef_h__
//--------------------------------------------------------------

#include <XRADBasic/MathFunctionTypes2D.h>

XRAD_USING

//--------------------------------------------------------------


typedef RealFunction2D_F32 frame_t;

enum modality
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
	// вы€снить правильное название проекций и их признаки в Dicom файлах

	e_dx_generic,
	e_dx_postero_anterior,
	e_dx_left_lateral,
	e_dx_right_lateral,


	//mammomgraphy
	// ƒл€ Fujifilm информаци€ о типе проекции содержитс€ в тэге
	// (0018,1400) Acquisition Device Processing Description LO
	//	RMLO                                              # 1, 4 
	//	дл€ других аппаратов вы€сн€ть
	e_mg_Rcc,
	e_mg_Lcc,
	e_mg_Rmlo,
	e_mg_Lmlo,
};

struct image_index_t
{
	modality	modality;
	image_t	image_type;
	size_t	image_no;
};


#endif // NewClassDef_h__