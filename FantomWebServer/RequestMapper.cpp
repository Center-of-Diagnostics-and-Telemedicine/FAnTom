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
#include <iostream>

#include <XRADBasic/Core.h>
#include <XRADBasic/ContainersAlgebra.h>
#include <XRADSystem/Sources/CFile/shared_cfile.h>
#include <XRADBasic/ThirdParty/nlohmann/json.hpp>
#include <QTGui/QImage.h>
#include <QTCore/QBuffer.h>
#include <QTCore/QThread>
#include <QTCore/QTextCodec>

#include <QtTest/QTest>

#include "ManageStrings.h"
#include "ManageBitmap.h"
#include "ManageTomogram.h"
#include "ManageWebPages.h"
#include "ManageServerCommands.h"

extern QString	web_server_path;
extern QString	data_store_path;




RequestMapper::RequestMapper(QObject* parent)
	:HttpRequestHandler(parent), isLoaded(false)
{
	qDebug() << "RequestMapper constructor finished";
}

void RequestMapper::LoadFantom()
{
	std::string s_buff = data_store_path.toStdString();

	const char* cbuff = s_buff.c_str();

	InitFantom_J(cbuff);

	char* accession_number;
	int acc_number_length;
	GetStudiesIDs_J(&accession_number, &acc_number_length);

	string str(accession_number);
	str.erase(acc_number_length - 1, 1);

	LoadCTbyAccession_J(str.c_str());

	isLoaded = true;
}


RequestMapper::~RequestMapper()
{
}

using namespace std;
using namespace xrad;


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
// Get a request parameters

	lock_guard<std::mutex> lck(m_RequestMapperMutex);

	if (!isLoaded)
	{
		nlohmann::json	j;

		j["response"] = nullptr;
		j["error"] = 22;

		response.setHeader("Content-Type", "text/html; charset=utf-8");

		response.write(QByteArray(j.dump('\t').c_str()));

		return;
	}

		qDebug() << "############";
		qDebug() << "SERVICE STARTED ID = " << QThread::currentThreadId();
		qDebug() << "############";

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

	QByteArray myBody = request.getBody();

	QString DataAsString(myBody);

//	wstring ws = DataAsString.toStdWString();


	switch (com_t)
	{
			case e_no_command:

					if (q_request_method == "GET")
					{
						if ( ws_path_name_no_slash == L"" )
						{
							GenerateLoginPage(q_params_map, message);
						}

						if (ws_path_name_no_slash == L"research/init")
						{
							nlohmann::json	j;

							j["response"] = "success";
							j["error"] = nullptr;

							response.setHeader("Content-Type", "text/html; charset=utf-8");
							response.write(QByteArray(j.dump('\t').c_str()));

							return;
						}

						else if ( ws_path_name_no_slash == L"favicon.ico" )
						{
							qDebug() << " favicon asked";
						}
						else if ( ws_path_name_no_slash == L"login_page.html" )
						{
							GenerateLoginPage(q_params_map, message);
						}
						else if ( ws_path_name_no_slash == L"DICOM_Viewer.html" )
						{
						//	LoadCTbyAccession(q_params_map, message);
							
							message << ReadDocument(L"DICOM_Viewer.html");
						}
				/*		else if ( is_filetype(ws_path_name_no_slash, L"txt") )
						{
							shared_cfile	opened_file;
							string document_path = text_file_path.toStdString() + "/" + convert_to_string(ws_path_name_no_slash);
							opened_file.open(document_path, "rb");
							DataArray<char>	document_data(opened_file.size() + 1, 0);
							opened_file.read_numbers(document_data, ioI8);
							wstring	ws_data = convert_to_wstring(ustring((const uchar_t*)document_data.data()));
							message << ws_data;
						}   */

						else if ( is_filetype(ws_path_name_no_slash, L"js") )
						{
							wstring	wjsdata = ReadDocument(ws_path_name_no_slash);
							message << wjsdata;
						}
					}

				/*	else if (q_request_method == "POST")
					{
						if ( is_filetype(ws_path_name_no_slash, L"txt") )
						{
							QByteArray text_saved = request.getBody();
							shared_cfile	file;
							string file_path = text_file_path.toStdString() + "/" + convert_to_string(ws_path_name_no_slash);
							file.open(file_path, "wb");
							file.write(text_saved.data(), text_saved.size(), 1);
						}
					} */

			case e_get_one_slice:
					if (q_params_map.value("img_format","") == "png" || q_params_map.value("img_format", "") == "bmp")
					{
						QByteArray bmp;
						bmp = GetSlice(q_params_map);
						response.setStatus(200, "OK");
						response.write(bmp);
						//@@@@@@@@@@prokudaylo
						qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@";
						qDebug() << "Some bmp QByteArray written ID = " <<  QThread::currentThreadId();
						qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@";
						//@@@@@@@@@@prokudaylo
						return;
					}
				break;
			case e_get_tomogram_coordinate:
				GetTomogramLocationFromScreenCoordinate(q_params_map, message);
				break;
			case e_get_scrn_coord_from_tmgm_lctn:
				GetScreenCoordinateFromTomogramLocation(q_params_map, message);
				break;
		//	case e_get_pixel_length:
		//		GeneratePixelLengthData(q_params_map, message);
		//		break;
			case e_get_screen_dimension:
				GetScreenDimension(q_params_map, message);

				qDebug() << "GetScreenDimension called ";
				qDebug() << u16tou8(message.str()).c_str();

				break;
			case e_get_tomogram_dimension:
				GetTomogramDimension(q_params_map, message);
				break;
			case e_get_point_HU:
				GetPointHU(q_params_map, message);

				break;
//			case e_get_coordinate_native:
//				GenerateNativeCoordData(q_params_map, message);
//				break;
//			case e_get_coordinate_interpolated:
//				GenerateInterpolatedCoordData(q_params_map, message);
//				break;
			case e_get_studies_ids:
				GetStudiesIDs(message);

				qDebug() << "GetStudiesIDs called ";
				qDebug() << u16tou8(message.str()).c_str();

				break;
			default:
				break;
			
		}
	
	// Set a response header
	response.setHeader("Content-Type", "text/html; charset=utf-8");


	// Generate the HTML document
	string	msgstr = u16tou8(message.str());

	//@@@@@@@@ prokudaylo
//	qDebug() << "RequestMapper::service msgstr.c_str()";
//	qDebug() << msgstr.c_str() ;
//	qDebug() << "End of msgstr.c_str()";
	//@@@@@@@@


	response.write(msgstr.c_str());

	//@@@@@@@@ prokudaylo
//		qDebug() << "";
//		qDebug() << "Some msgstr.c_str()is written to response" ;
//		qDebug() << "";
	//@@@@@@@@

		qDebug() << "############";
		qDebug() << "SERVICE COMPLETED ID = " << QThread::currentThreadId();
		qDebug() << "############";
}

