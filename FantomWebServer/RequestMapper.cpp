﻿#include "pre.h"
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

		response.setHeader("Content-Type", "application/json; charset=utf-8");

		response.write(QByteArray(j.dump('\t').c_str()));

		return;
	}

		qDebug() << "############";
		qDebug() << "SERVICE STARTED ID = " << QThread::currentThreadId();
		qDebug() << "############";


//Определение чистого адреса без параметров

	wstring	ws_path_name = interpret_url(request.getRawPath());
	wstring	ws_path_name_no_slash = DeleteSlash(ws_path_name);

//Определение имени сайта (хоста)

	QByteArray q_host_name = request.getHeader("host");

//Type = POST, SEND, GET ...
	QByteArray q_request_method = request.getMethod();

//Определение параметров запроса

					if (q_request_method == "GET")
					{


						if (ws_path_name_no_slash == L"research/init")
						{
							nlohmann::json	j;

							size_t dimension;

							GetTomogramDimension_J(&dimension, slice_type::e_axial);
							j["response"]["axialTomogram"] = dimension;

							GetTomogramDimension_J(&dimension, slice_type::e_frontal);
							j["response"]["frontalTomogram"] = dimension;

							GetTomogramDimension_J(&dimension, slice_type::e_sagittal);
							j["response"]["sagittalTomogram"] = dimension;


							GetScreenDimension_J(&dimension, slice_type::e_axial);
							j["response"]["axialScreen"] = dimension;

							GetScreenDimension_J(&dimension, slice_type::e_frontal);
							j["response"]["frontalScreen"] = dimension;

							GetScreenDimension_J(&dimension, slice_type::e_sagittal);
							j["response"]["sagittalScreen"] = dimension;


							double length_pixel_coef;
							GetPixelLengthCoefficient_J(&length_pixel_coef);

							j["response"]["pixelLength"] = length_pixel_coef;

							size_t pixel_coord;
							GetTomogramLocationFromScreenCoordinate_J(&pixel_coord, slice_type::e_axial, 0, true);
							
							if (pixel_coord > 0)
							{
								j["response"]["reversed"] = true;
							}
							else
							{
								j["response"]["reversed"] = false;
							}
			
							j["error"] = nullptr;

							response.setHeader("Content-Type", "application/json; charset=utf-8");
							response.write(QByteArray(j.dump('\t').c_str()));

							return;
						}
					}

					else if (q_request_method == "POST")
					{
					
						if(ws_path_name_no_slash == L"research/hounsfield")
						{ 
							QByteArray myBody = request.getBody();
							if (myBody.isEmpty())
							{
								qDebug() << "The body of request is empty";
								return;
							}
							QString DataAsString(myBody);

							string str = convert_to_string8(qs_to_ws(DataAsString));

							nlohmann::json	j_request;

							j_request = nlohmann::json::parse(str);
						
							double huValue;
							//	GetPointHU_J(double *value, size_t axial_coord, size_t frontal_coord, size_t sagittal_coord)
							GetPointHU_J(&huValue, j_request["axialCoord"], j_request["frontalCoord"], j_request["sagittalCoord"]);

							nlohmann::json	j_response;

							j_response["response"]["huValue"] = huValue;
							j_response["error"] = nullptr;

							response.setHeader("Content-Type", "application/json; charset=utf-8");
							response.write(QByteArray(j_response.dump('\t').c_str()));

							return;
						}

						if (ws_path_name_no_slash == L"research/slice")
						{
							QByteArray myBody = request.getBody();
							if (myBody.isEmpty())
							{
								qDebug() << "The body of request is empty";
								return;
							}
							QString DataAsString(myBody);

							string str = convert_to_string8(qs_to_ws(DataAsString));

							nlohmann::json	j_request;

							j_request = nlohmann::json::parse(str);

							string tmp = j_request["sliceType"];

							slice_type st = GetImageType(convert_to_wstring(tmp));

							string tmp1 = j_request["mipMethod"];

							mip_method_type mip_method = GetMIPMethod(convert_to_wstring(tmp1));

							int tmp2 = j_request["sliceNumber"];

							size_t slice_no = GetSliceNo(tmp2, st);

							const unsigned char *img;
							int  length;

							GetSlice_J(&img, &length,
								st,
								slice_no,
								j_request["black"],
								j_request["white"],
								j_request["gamma"],
								j_request["mipValue"],
								mip_method
							);

							QByteArray png = QByteArray();

							CreateQByteArrayPngFromChar(png, img, length, L"png");

							nlohmann::json	j_response;

							j_response["response"]["image"] = png.toBase64();
							j_response["error"] = nullptr;

							response.setHeader("Content-Type", "application/json; charset=utf-8");
							response.write(QByteArray(j_response.dump('\t').c_str()));

							return;
						}
						
					} 


		qDebug() << "############";
		qDebug() << "SERVICE COMPLETED ID = " << QThread::currentThreadId();
		qDebug() << "############";
}

