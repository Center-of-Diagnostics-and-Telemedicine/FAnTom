#include "pre.h"
/*!
* \file RequestMapper.cpp
* \date 2017/10/04 16:06
*
* \author kulberg
*
* \brief
*
* TODO: long description
*
* \note
*/
#include "RequestMapper.h"

#include <sstream>

#include <XRADBasic/Core.h>
#include <XRADBasic/ContainersAlgebra.h>
#include <XRADSystem/Sources/CFile/shared_cfile.h>
#include <QTGui/QImage.h>
#include <QTCore/QBuffer.h>
#include "ManageStrings.h"
#include "ManageBitmap.h"
#include "ManageTomogram.h"
#include "ManageWebPages.h"
#include "ManageServerCommands.h"


extern QString	web_server_path;
extern QString	data_store_path;
extern QString	text_file_path;
extern QMultiMap<QByteArray, QByteArray> doctor_database_map;


RequestMapper::RequestMapper(QObject* parent, int in_port)
	:HttpRequestHandler(parent), port(in_port)
{
	//wstring data_store_path_ws = convert_to_wstring(data_store_path.toStdString());
	InitFantom(convert_to_wstring(data_store_path.toStdString()));
}


RequestMapper::~RequestMapper()
{
}

using namespace std;
using namespace xrad;


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
// Get a request parameters
	std::wstringstream	message;

//Определение чистого адреса без параметров

	wstring	ws_path_name = interpret_url(request.getRawPath());
	wstring	ws_path_name_no_slash = DeleteSlash(ws_path_name);

//Определение имени сайта (хоста)

	QByteArray q_host_name = request.getHeader("host");

//Type = POST, SEND, GET ...
	QByteArray q_request_method = request.getMethod();

//Определение параметров запроса

	QMultiMap<QByteArray, QByteArray> q_params_map = request.getParameterMap();
	command_type com_t = ParseCommand(q_params_map);

//@@@	if (is_filetype(ws_path_name_no_slash, L"html")&&(com_t == e_load_web_page))
//	{
	     if (q_request_method == "GET" && com_t == e_no_command && ws_path_name_no_slash == L"" )//ws_path_name_no_slash.isEmthy()
	    {
			 GenerateStartPage(message);
		}
		 else if (q_request_method == "GET" && com_t == e_no_command && ws_path_name_no_slash == L"favicon.ico")
		 {
			 qDebug() << " favicon asked";
		 }
		 else if (q_request_method == "GET" && com_t == e_no_command &&ws_path_name_no_slash == L"login_page.html")
		 {
			 GenerateLoginPage(q_params_map, message);
		 }
		else if (q_request_method == "GET" && com_t == e_no_command &&ws_path_name_no_slash == L"DICOM_Viewer.html")
		{
			GenerateDICOMPage(q_params_map, message);
		}

		else if (is_filetype(ws_path_name_no_slash, L"txt") && q_request_method == "POST")
		{
			QByteArray text_saved = request.getBody();
			shared_cfile	file;
			string file_path = text_file_path.toStdString() + "/" + convert_to_string(ws_path_name_no_slash);
			file.open(file_path, "wb");
			file.write(text_saved.data(), text_saved.size(), 1);
		}
		else if (is_filetype(ws_path_name_no_slash, L"txt") && q_request_method == "GET")
		{
			shared_cfile	opened_file;
			string document_path = text_file_path.toStdString() +"/" + convert_to_string(ws_path_name_no_slash);
			opened_file.open(document_path, "rb");
			DataArray<char>	document_data(opened_file.size() + 1, 0);
			opened_file.read_numbers(document_data, ioI8);
			wstring	ws_data = convert_to_wstring(ustring((const uchar_t*)document_data.data()));
			message << ws_data;
		}
		else if (is_filetype(ws_path_name_no_slash, L"js"))
		{
			wstring	wjsdata = ReadDocument(ws_path_name_no_slash);
			message << wjsdata;
		}
		//else if (filetype_is(ws_path_name_no_slash, L".bmp"))
		//else if (is_filetype(ws_path_name_no_slash, L"png"))
		else if (q_params_map.value("img_format","") == "png" || q_params_map.value("img_format", "") == "bmp")
		{
			QByteArray bmp;
			bmp = ParseSliceBMP(q_params_map);
			response.setStatus(200, "OK");
			response.write(bmp);
            //@@@@@@@@@@prokudaylo
			qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@";
			qDebug() << "Some bmp QByteArray written";
			qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@";
			//@@@@@@@@@@prokudaylo
			return;
		}
		else
		{
			switch (com_t)
			{
			case e_get_original_coordinate:
				GenerateOriginalPixelCoordData(q_params_map, message);
				break;
			case e_get_pixel_interpolated:
				GenerateInterpolatedPixelData(q_params_map, message);
				break;
			case e_get_pixel_length:
				GeneratePixelLengthData(q_params_map, message);
				break;
			case e_get_n_frames_interpolated:
				GenerateNFramesInterpolatedData(q_params_map, message);
				break;
			case e_get_n_frames_real:
				GenerateNFramesRealData(q_params_map, message);
				break;
			case e_get_point_HU:
				GenerateHUValueData(q_params_map, message);
				break;
			case e_get_coordinate_native:
				GenerateNativeCoordData(q_params_map, message);
				break;
			case e_get_coordinate_interpolated:
				GenerateInterpolatedCoordData(q_params_map, message);
				break;
			//case e_load_start_page:
			//	GenerateStartPage(message);
			//	break;
			case e_check_doctor_login:
				CheckDoctorLogin(q_params_map, message);
				break;
			case e_get_accession_numbers:
				GetAccNamesData(message);

				qDebug() << "GetAccNamesData called ";
				qDebug() << u16tou8(message.str()).c_str();

				break;
			case e_delete_ct:
				//CloseTomogram(q_params_map);
				break;
			case e_get_study_accession:
				GenerateStudyAccessionNumberData(message);
				break;
			case e_no_command:
				break;
			case e_load_web_page:
				break;
			default:
				break;
			}
		}
	
	// Set a response header
	response.setHeader("Content-Type", "text/html; charset=utf-8");


	// Generate the HTML document
	string	msgstr = u16tou8(message.str());

	//@@@@@@@@ prokudaylo
	qDebug() << "RequestMapper::service msgstr.c_str()";
	qDebug() << msgstr.c_str() ;
	qDebug() << "End of msgstr.c_str()";
	//@@@@@@@@


	response.write(msgstr.c_str());
}

