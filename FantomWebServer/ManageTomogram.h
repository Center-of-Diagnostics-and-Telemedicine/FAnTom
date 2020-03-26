﻿#ifndef ManageTomogram_h__
#define ManageTomogram_h__

#include <XRADBasic/ContainersAlgebra.h>
#include <QTCore/QByteArray.h>
#include <QTCore/QMap.h>
#include <FantomLibrary.h>
#include <vector>


XRAD_BEGIN
using namespace std;


slice_type GetImageType(wstring st_wstring);
mip_method_type GetMIPMethod(wstring mip_method_wstring);
size_t GetSliceNo(wstring accession_number_ws, wstring slice_no_wstring, slice_type st);
void GetSliceTomogramWeb(RealFunction2D_UI8 &img, wstring accession_number, slice_type st, size_t slice_no, double black, double white, double gamma, size_t aprox_size, mip_method_type mip_method);
QByteArray ParseSliceBMP(QMultiMap<QByteArray, QByteArray> &q_params_map);


XRAD_END


#endif // ManageTomogram_h__