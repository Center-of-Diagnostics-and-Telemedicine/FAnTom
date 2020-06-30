//--------------------------------------------------------------
#ifndef SliceDefs_h__
#define SliceDefs_h__
//--------------------------------------------------------------

#include <XRADBasic/MathFunctionTypes2D.h>

XRAD_USING

//--------------------------------------------------------------


typedef RealFunction2D_F32 frame_t;


 struct modality_t
 {
	 static const string CT() { return "CT"; }
	 static const string DX() { return "DX"; }
	 static const string MG() { return "MG"; }
 };



struct image_t
{
	//tomographic
	static const string ct_axial()	{	return "ct_axial"; }
	static const string ct_frontal()	{	return "ct_frontal"; }
	static const string ct_sagittal()	{	return "ct_sagittal"; }

	//lung
	// выяснить правильное название проекций и их признаки в Dicom файлах

	static const string dx_generic()	{	return "dx_generic"; }
	static const string dx_postero_anterior()	{	return "dx_postero_anterior"; }
	static const string dx_left_lateral()	{	return "dx_left_lateral"; }
	static const string dx_right_lateral()	{	return "dx_right_lateral"; }


	//mammomgraphy
	// Для Fujifilm информация о типе проекции содержится в тэге
	// (0018,1400) Acquisition Device Processing Description LO
	//	RMLO                                              # 1, 4 
	//	для других аппаратов выяснять
	static const string mg_rcc()	{	return "mg_rcc"; }
	static const string mg_lcc()	{	return "mg_lcc"; }
	static const string mg_rmlo()	{	return "mg_rmlo"; }
	static const string mg_lmlo()	{	return "mg_lmlo"; }
};


struct mip_index_t
{
	const string mip_method;
	size_t       mip_value;
};

struct image_index_t
{
	const string	modality;
	const string	image_type;
	size_t	image_no;
	mip_index_t mip;
};

struct mip_method_t
{
	static const string average()	{	return "mip_average"; }
	static const string maxvalue()	{	return "mip_maxvalue";}
	static const string minvalue()	{	return "mip_minvalue"; }
	static const string none()	{	return "mip_no_mip"; }
};



struct brightness
{
	double white;
	double black;
	double gamma;
};


enum axis_t
{
	e_z = 0,
	e_y = 1,
	e_x = 2
};
#endif // SliceDefs_h__