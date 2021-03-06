﻿#include "pre.h"
/*!
* \file ManageBitmap.cpp
* \date 2017/10/05 15:39
*
* \author kulberg
*
* \brief
*
* TODO: long description
*
* \note
*/
//#include "pre.h"
#include <sstream>
#include "ManageTomogram.h"
#include "ManageBitmap.h"
#include "ManageStrings.h"
#include <QtCore/QBuffer>

#include "ManageWebPages.h"

#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <QtGui/QPixmap>


XRAD_BEGIN



QByteArray GetSlice(QMultiMap<QByteArray, QByteArray> &q_params_map)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));

	mip_method_type mip_method = GetMIPMethod(interpret_url(q_params_map.value("mip_method", "")));

	size_t slice_no = GetSliceNo(interpret_url(q_params_map.value("slice_no", "")), st);

	const unsigned char *img;
	int  length;

	GetSlice_J(&img, &length,
		st,
		slice_no,
		stod(interpret_url(q_params_map.value("black_var", ""))),
		stod(interpret_url(q_params_map.value("white_var", ""))),
		stod(interpret_url(q_params_map.value("gamma_var", ""))),
		string_to_int(interpret_url(q_params_map.value("aprox_value", ""))),
		mip_method
	);

	QByteArray bmp = QByteArray();

//	CreateQByteArrayPngFromChar(bmp, img, length, interpret_url(q_params_map.value("img_format", "")));

	return bmp;
}

//void GetSliceTomogramWeb(RealFunction2D_UI8 &img, wstring accession_number, slice_type st, size_t slice_no,  double black, double white, double gamma, size_t aprox_size, mip_method_type mip_method)
//{
//	RealFunction2D_F32 img_buf;
//	GetSlice(img_buf, st, slice_no, black, white, gamma, aprox_size, mip_method);
////	size_t vs(img_buf.vsize()), hs(img_buf.hsize());
//	img.MakeCopy(img_buf);
//}
slice_type GetImageTypeFromInt(int img_type)
{
	if (img_type == 0)
	{
		return e_axial;
	}
	else if (img_type == 1)
	{
		return e_frontal;
	}
	else if (img_type == 2)
	{
		return e_sagittal;
	}
	else return e_axial;
}

mip_method_type GetMIPMethod(wstring mip_method_wstring)
{
	if (mip_method_wstring == L"average")
	{
		return e_average;
	}
	else if (mip_method_wstring == L"maxvalue")
	{
		return e_maxvalue;
	}
	else
	{
		return e_no_mip;
	}
}

slice_type GetImageType(wstring st_wstring)
{
	if (st_wstring == L"axial")
	{
		return e_axial;
	}
	else if (st_wstring == L"sagittal")
	{
		return e_sagittal;
	}
	else if (st_wstring == L"frontal")
	{
		return e_frontal;
	}
	else return e_axial;
}



mip_method_type GetMIPMethodFromInt(int mip_method)
{
	if (mip_method == 0)
	{
		return e_average;
	}
	else if (mip_method == 1)
	{
		return e_maxvalue;
	}
	else if (mip_method == 2)
	{
		return e_minvalue;
	}
	else
	{
		return e_no_mip;
	}
}


size_t GetSliceNo(wstring slice_no_wstring, slice_type st)
{
	size_t frames_no;
	GetTomogramDimension_J(&frames_no, st);
	size_t slice_no = string_to_int(slice_no_wstring);

	if (slice_no > frames_no)
	{
		slice_no = frames_no;
	}
	return slice_no;
}

size_t GetSliceNo(size_t slice_no, slice_type st)
{
	size_t frames_no;
	GetTomogramDimension_J(&frames_no, st);

	if (slice_no > frames_no)
	{
		slice_no = frames_no;
	}
	return slice_no;
}

XRAD_END