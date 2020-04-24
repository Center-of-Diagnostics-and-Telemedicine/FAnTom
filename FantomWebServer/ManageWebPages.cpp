﻿#include "pre.h"
#include <sstream>
#include <XRADBasic/Core.h>
#include <XRADSystem/CFile.h>
#include "ManageStrings.h"
#include "ManageBitmap.h"
#include "ManageTomogram.h"
#include "ManageWebPages.h"
#include <FantomLibrary/FantomLibrary.h>


using namespace std;
extern QString	web_server_path;


XRAD_BEGIN


void GenerateNFramesRealData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	size_t n_frames;
//	GetNFrames_real(n_frames, st);
	GetTomogramDimension_J(&n_frames, st);
	message << n_frames;
}


void GenerateNFramesInterpolatedData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	size_t n_frames;
//	GetNFrames_interpolated(n_frames, st);
	GetScreenDimension_J(&n_frames, st);
	message << n_frames;
}


void GenerateStudyAccessionNumberData(std::wstringstream &message)
{
	wstring id;
	GetStudyAccessionNumber(id);
	message << id; //(id, e_decode_literals);
}


void GenerateNativeCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	double coord_dbl;
	GetCoordinateNative(coord_dbl, st, _wtoi(interpret_url(q_params_map.value("slice_no", "")).c_str()));
	message << coord_dbl;
}


void GenerateInterpolatedCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	double coord_dbl;
	GetCoordinateInterpolated(coord_dbl, st, _wtoi(interpret_url(q_params_map.value("coord", "")).c_str()));
	message << coord_dbl;
}


void GenerateHUValueData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	double hu_value;
	GetPointHU_J(&hu_value, _wtoi(interpret_url(q_params_map.value("coords_z", "")).c_str()),
	_wtoi(interpret_url(q_params_map.value("coords_y", "")).c_str()),
		_wtoi(interpret_url(q_params_map.value("coords_x", "")).c_str()));
	message << hu_value;
}


void GetAccNamesData(std::wstringstream &message)
{
	vector<wstring> acc_names;
	GetAccessionNumbers_J(&acc_names);
	for (size_t i = 0; i < acc_names.size(); ++i)
	{
		message << convert_to_wstring(acc_names[i]) + L" ";
	}
}

void GenerateOriginalPixelCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	size_t pixel_coord(0);
	bool interpolate_z = false;
	if (interpret_url(q_params_map.value("coord_interpolation", "")) == L"true")
	{
		interpolate_z = true;
	}
	GetOriginalPixelCoordinate(pixel_coord, st, _wtoi(interpret_url(q_params_map.value("coord", "")).c_str()), interpolate_z);
	message << pixel_coord;
}


void GeneratePixelLengthData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	double pixel_length(0);
	GetPixelLengthCoefficient(pixel_length);
	message << pixel_length;
}

void GenerateInterpolatedPixelData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	size_t pixel_coord(0);
	GetInterpolatedPixel(pixel_coord, st, wcstod(interpret_url(q_params_map.value("slice_no", "")).c_str(), NULL));
	message << pixel_coord;
}

void GenerateDICOMPage(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	bool series_loaded(false);
	LoadCTbyAccession(interpret_url(q_params_map.value("accession_number", "")), series_loaded);
	if (series_loaded)
	{
		wstring	ws_dicom_page_data = ReadDocument(L"DICOM_Viewer.html");
		message << ws_dicom_page_data;

		char *buf;
		int len;
		GetDetailedStudyInfo_J(&buf, &len);

		printf("\nDetailed study info\n");
		printf(buf);
		printf("\n-----------------\n");

	}
	else
	{
		GenerateLoginPage(q_params_map, message);
	}
}


wstring ReadDocument(wstring file_name_no_slash)
{
	shared_cfile	opened_file;
	string document_path = web_server_path.toStdString() + "html/" + convert_to_string(file_name_no_slash);
	opened_file.open(document_path, "rb");
	DataArray<char>	document_data(opened_file.size() + 1, 0);
	opened_file.read_numbers(document_data, ioI8);
	wstring	ws_data = convert_to_wstring(ustring((const uchar_t*)document_data.data()));
	return ws_data;
}

void GenerateLoginPage(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	wstring	ws_login_page_data = ReadDocument(L"login_page.html");
	if (q_params_map.contains("accession_number"))
	{
		message << std::wstring(L"Введеный идентификатор не найден. Вы ввели: <b>") << interpret_url(q_params_map.value("accession_number", "")) << L"</b><br>";
	}
	message << ws_login_page_data;
}

//void CloseTomogram(QMultiMap<QByteArray, QByteArray> &q_params_map)
//{
//	CloseCTStudyAcession(interpret_url(q_params_map.value("accession_number", "")));
//}

void GenerateStartPage(std::wstringstream &message)
{
	wstring	wjsdata = ReadDocument(L"start_page.html");
	message << wjsdata;
}


XRAD_END
