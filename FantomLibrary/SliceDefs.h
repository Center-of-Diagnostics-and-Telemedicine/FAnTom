//--------------------------------------------------------------
#ifndef SliceDefs_h__
#define SliceDefs_h__
//--------------------------------------------------------------

#include <XRADBasic/MathFunctionTypes2D.h>

XRAD_USING

//--------------------------------------------------------------


typedef RealFunction2D_F32 frame_t;
/*
enum modality_t
{
	CT,
	DX,
	MG
};
*/
/*
 struct mod_t
{
	static char* CT = "CT" ;
	static char* DX = "DX";
	static char* MG = "MG";
};
*/


 struct modality_t
 {
	 static const wstring CT() { return L"CT"; }
	 static const wstring DX() { return L"DX"; }
	 static const wstring MG() { return L"MG"; }
 };



struct image_t
{
	//tomographic
	static const wstring e_ct_axial()	{ return L"e_ct_axial"; }
	static const wstring e_ct_frontal()	{ return L"e_ct_frontal"; }
	static const wstring e_ct_sagittal() { return L"e_ct_sagittal"; }

	//lung
	// выяснить правильное название проекций и их признаки в Dicom файлах

	static const wstring e_dx_generic() { return L"e_dx_generic"; }
	static const wstring e_dx_postero_anterior() { return L"e_dx_postero_anterior"; }
	static const wstring e_dx_left_lateral() { return L"e_dx_left_lateral"; }
	static const wstring e_dx_right_lateral() { return L"e_dx_right_lateral"; }


	//mammomgraphy
	// Для Fujifilm информация о типе проекции содержится в тэге
	// (0018,1400) Acquisition Device Processing Description LO
	//	RMLO                                              # 1, 4 
	//	для других аппаратов выяснять
	static const wstring e_mg_rcc() { return L"e_mg_rcc"; }
	static const wstring e_mg_lcc() { return L"e_mg_lcc"; }
	static const wstring e_mg_rmlo() { return L"e_mg_rmlo"; }
	static const wstring e_mg_lmlo() { return L"e_mg_lmlo"; }
};

struct image_index_t
{
	const wstring	modality;
	const wstring	image_type;
	size_t	image_no;
};

enum mip_method_t
{
	e_mip_average,
	e_mip_maxvalue,
	e_mip_minvalue,//TODO сделать
	e_mip_no_mip
};

struct mip_index_t
{
	mip_method_t mip_method;
	size_t       mip_half_size;
};



enum axis_t
{
	e_z = 0,
	e_y = 1,
	e_x = 2
};
#endif // SliceDefs_h__