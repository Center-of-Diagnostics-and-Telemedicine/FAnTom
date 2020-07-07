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
#include <QtGui/QImage>
#include <QtCore/QBuffer>
#include <QtCore/QThread>
#include <QtCore/QTextCodec>



#include "ManageStrings.h"
#include "ManageBitmap.h"
#include "ManageTomogram.h"
#include "ManageWebPages.h"
#include "ManageServerCommands.h"

#include <XRADQt/QtStringConverters.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>

extern QString	data_store_path;

RequestMapper::RequestMapper(QObject* parent)
	:HttpRequestHandler(parent), isLoaded(false)
{
//	qDebug() << "RequestMapper constructor finished";
}

void RequestMapper::LoadFantom()
{
	wstring ws = qstring_to_wstring(data_store_path);

	InitFantom_J(convert_to_string8(ws).c_str());
//	InitFantom_J(convert_to_string8(ws).c_str());

	char* accession_number;
	int acc_number_length;
	GetStudiesIDs_J(&accession_number, &acc_number_length);

	string str(accession_number);
	str.erase(acc_number_length - 1, 1);

	cout << "Accession number = " << str << endl;

	LoadCTbyAccession_J(str.c_str());

	isLoaded = true;
}

auto &ibi = interpolators2D::ibicubic;

void RequestMapper::LoadFantom1()
{
	wstring ws = qstring_to_wstring(data_store_path);

	InitHeap_N(ws);

	wstring acc_no;
	GetAccNumber_N(0, acc_no);

	wcout << "Accession number = " << acc_no << endl;

	LoadByAccession_N(acc_no);

	isLoaded = true;
}


RequestMapper::~RequestMapper()
{
}

using namespace std;
using namespace xrad;


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
//	lock_guard<std::mutex> lck(m_RequestMapperMutex);

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


	if (!isLoaded)
	{
		if (q_request_method == "GET" && ws_path_name_no_slash == L"research/close")
		{
			nlohmann::json	j;

			j["response"] = nullptr;
			j["error"] = 22;

			response.setHeader("Content-Type", "application/json; charset=utf-8");

			response.write(QByteArray(j.dump('\t').c_str()));

			emit CloseApp();

			return;
		}

		else
		{
			nlohmann::json	j;

			j["response"] = nullptr;
			j["error"] = 22;

			response.setHeader("Content-Type", "application/json; charset=utf-8");

			response.write(QByteArray(j.dump('\t').c_str()));

			return;
		}
	}

	if (q_request_method == "GET")
	{
		if (ws_path_name_no_slash == L"research/close")
		{
			response.setStatus(200, "OK");

			emit CloseApp();

			return;
		}

		if (ws_path_name_no_slash == L"research/init")
		{
			nlohmann::json	j;

			vector<image_size_t> v;

			string modality;

			GetModality_N(modality);

			GetDimensions_N(j);

			for (auto &image : v)
			{
				nlohmann::json	node;

				node["n_images"] = image.n_images;
				node["screen_size_v"] = image.screen_sizes.y();
				node["screen_size_h"] = image.screen_sizes.x();

				node["dicom_size_v"] = image.dicom_sizes.y();
				node["dicom_size_h"] = image.dicom_sizes.x();

				node["dicom_step_v"] = image.steps.y();
				node["dicom_step_h"] = image.steps.x();

				j["response"][image.modality][image.image_type] = node;
			}

			bool isFlipped;

			GetZFlip_N(isFlipped);

			j["response"][modality]["reversed"] = isFlipped;

			j["error"] = nullptr;

			response.setHeader("Content-Type", "application/json; charset=utf-8");
			response.write(QByteArray(j.dump('\t').c_str()));

			return;
		}
	}

	else if (q_request_method == "POST")
	{

		if (ws_path_name_no_slash == L"research/brightness")
		{
			QByteArray myBody = request.getBody();
			if (myBody.isEmpty())
			{
				qDebug() << "The body of request is empty";
				return;
			}
			QString DataAsString(myBody);

			string str = convert_to_string8(qstring_to_wstring(DataAsString));

			nlohmann::json	j_req;

			j_req = nlohmann::json::parse(str);

			double brValue;
			GetBrightness_N(&brValue,
			{ j_req["image"]["modality"],
				j_req["image"]["type"],
				j_req["image"]["number"],
				{
					j_req["image"]["mip"]["mip_method"],
					j_req["image"]["mip"]["mip_value"]
				}
			},
				j_req["point"]["vertical"],
				j_req["point"]["horizontal"]
				);

			nlohmann::json	j_response;

			j_response["response"]["huValue"] = brValue;
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

			string str = convert_to_string8(qstring_to_wstring(DataAsString));

			nlohmann::json	j_req;

			j_req = nlohmann::json::parse(str);

			const unsigned char *img;
			int  length;

			//GetScreenImage_N(const unsigned char **img, int *length, image_index_t idx, brightness brightness)
			GetScreenImage_N(&img, &length,
			{ j_req["image"]["modality"],
				j_req["image"]["type"],
				j_req["image"]["number"],
				{
					j_req["image"]["mip"]["mip_method"],
					j_req["image"]["mip"]["mip_value"]
				}
			},
			{
				j_req["brightness"]["white"],
				j_req["brightness"]["black"],
				j_req["brightness"]["gamma"]
			}
			);

			QByteArray png = QByteArray();

			CreateQByteArrayPngFromChar(png, img, length);

			nlohmann::json	j_response;

			j_response["response"]["image"] = png;//.toBase64();
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


void RequestMapper::service_old(HttpRequest& request, HttpResponse& response)
{
	// Get a request parameters

	lock_guard<std::mutex> lck(m_RequestMapperMutex);

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


	if (!isLoaded)
	{
		if (q_request_method == "GET" && ws_path_name_no_slash == L"research/close")
		{
			nlohmann::json	j;

			j["response"] = nullptr;
			j["error"] = 22;

			response.setHeader("Content-Type", "application/json; charset=utf-8");

			response.write(QByteArray(j.dump('\t').c_str()));

//			qApp->quit();

			emit CloseApp();

//			quit_application q("Exception during LoadFantom", 0);
//			throw q;

			return;
		}

		else
		{
			nlohmann::json	j;

			j["response"] = nullptr;
			j["error"] = 22;

			response.setHeader("Content-Type", "application/json; charset=utf-8");

			response.write(QByteArray(j.dump('\t').c_str()));

			return;
		}
	}

					if (q_request_method == "GET")
					{
						if (ws_path_name_no_slash == L"research/close")
						{
							response.setStatus(200, "OK");

						//	qApp->quit();

							emit CloseApp();

//							quit_application q("Exception after LoadFantom", 0);
//							throw q;

							return;
						}

						if (ws_path_name_no_slash == L"research/init")
						{
							nlohmann::json	j;

							size_t dimension;

							point3_ST dimensions;

							string str;


						GetTomogramDimension_J(&dimension, slice_type::e_axial);
							j["response"]["axialTomogram"] = dimension;

							GetTomogramDimension_J(&dimension, slice_type::e_frontal);
							j["response"]["frontalTomogram"] = dimension;

							GetTomogramDimension_J(&dimension, slice_type::e_sagittal);
							j["response"]["sagittalTomogram"] = dimension;			

							GetScreenDimensions_N(dimensions);

							j["response"]["axialScreen"] = dimensions.z();
							j["response"]["frontalScreen"] = dimensions.y();
							j["response"]["sagittalScreen"] = dimensions.x();


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

							string str = convert_to_string8(qstring_to_wstring(DataAsString));

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

							string str = convert_to_string8(qstring_to_wstring(DataAsString));

							nlohmann::json	j_request;

							j_request = nlohmann::json::parse(str);

							const unsigned char *img;
							int  length;

							GetSlice_J(&img, &length,
								GetImageTypeFromInt(j_request["sliceType"]),
								j_request["sliceNumber"],
								j_request["black"],
								j_request["white"],
								j_request["gamma"],
								j_request["mipValue"],
								GetMIPMethodFromInt(j_request["mipMethod"])
							);

							QByteArray png = QByteArray();

							CreateQByteArrayPngFromChar(png, img, length);

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
