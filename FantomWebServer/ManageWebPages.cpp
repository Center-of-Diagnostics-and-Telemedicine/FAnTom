#include "pre.h"
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


void GetTomogramDimension(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	size_t n_frames;
//	GetNFrames_real(n_frames, st);
	GetTomogramDimension_J(&n_frames, st);
	message << n_frames;
}


void GetScreenDimension(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	size_t n_frames;
//	GetNFrames_interpolated(n_frames, st);
	GetScreenDimension_J(&n_frames, st);
	message << n_frames;
}




//void GenerateNativeCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
//{
//	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
//	double coord_dbl;
//	GetCoordinateNative(coord_dbl, st, _wtoi(interpret_url(q_params_map.value("slice_no", "")).c_str()));
//	message << coord_dbl;
//}


//void GenerateInterpolatedCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
//{
//	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
//	double coord_dbl;
//	GetCoordinateInterpolated(coord_dbl, st, _wtoi(interpret_url(q_params_map.value("coord", "")).c_str()));
//	message << coord_dbl;
//}


void GenerateHUValueData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	double hu_value;
	GetPointHU_J(&hu_value, _wtoi(interpret_url(q_params_map.value("coords_z", "")).c_str()),
	_wtoi(interpret_url(q_params_map.value("coords_y", "")).c_str()),
		_wtoi(interpret_url(q_params_map.value("coords_x", "")).c_str()));
	message << hu_value;
}


void GetStudiesIDs(std::wstringstream &message)
{
//	vector<wstring> acc_names;
//	GetNumbersOfAccessions(&acc_names);
	char* acc_n;
	int length;
	GetStudiesIDs_J(&acc_n, &length);

	message << string8_to_wstring(acc_n);

}

void GetTomogramLocationFromScreenCoordinate(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	size_t pixel_coord(0);
	bool interpolate_z = false;
	if (interpret_url(q_params_map.value("coord_interpolation", "")) == L"true")
	{
		interpolate_z = true;
	}
	GetTomogramLocationFromScreenCoordinate_J(&pixel_coord, st, _wtoi(interpret_url(q_params_map.value("coord", "")).c_str()), interpolate_z);
	message << pixel_coord;
}


//void GeneratePixelLengthData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
//{
//	double pixel_length(0);
//	GetPixelLengthCoefficient(pixel_length);
//	message << pixel_length;
//}

void GetScreenCoordinateFromTomogramLocation(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	slice_type st = GetImageType(interpret_url(q_params_map.value("img_type", "")));
	size_t pixel_coord(0);
	GetScreenCoordinateFromTomogramLocation_J(&pixel_coord, st, wcstod(interpret_url(q_params_map.value("slice_no", "")).c_str(), NULL));
	message << pixel_coord;
}

void LoadCTbyAccession(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message)
{
	bool series_loaded(false);

	std::string s_buff = convert_to_string( interpret_url(q_params_map.value("accession_number", "")) );
	
	const char* cbuff = s_buff.c_str();

	LoadCTbyAccession_J(cbuff, &series_loaded);

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
