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
//#include "FantomDefs.h"

#include <XRADQt/QtStringConverters.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>

extern QString	data_store_path;

RequestMapper::RequestMapper(QObject* parent)
	:HttpRequestHandler(parent), isLoaded(false)
{
//	qDebug() << "RequestMapper constructor finished";
}



void RequestMapper::LoadFantom1()
{
	wstring ws = qstring_to_wstring(data_store_path);

	try
	{
		InitHeapFiltered_N(ws);

		LoadByAccession_N();
	}
	catch(invalid_argument &e)
	{
		cout << "Invalid argument" << e.what() << endl;

		return;// e_other;
	}

	catch (operation_result opr)
	{
		cout << "Operation result error " << opr << endl;
		fflush(stdout);

		return;// e_other;
	}
	catch (...)
	{
		cout << "Some exception thrown" << endl;
		fflush(stdout);
		return;// e_other;
	}

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

						string modality;
						GetModality_N(modality);

						GetDimensions_N(j);



						bool isFlipped;

						GetZFlip_N(isFlipped);

						j["response"][modality]["reversed"] = isFlipped;

						j["error"] = nullptr;

						response.setHeader("Content-Type", "application/json; charset=utf-8");
						response.write(QByteArray(j.dump('\t').c_str()));

					//	return;
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

						nlohmann::json	j_req, j_response;

						j_req = nlohmann::json::parse(str);

						double brValue;
					try
					{

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
					}
					catch (modality_error &e)
					{
						std::cout << e.what() << std::endl;

						j_response["response"]["brightness"] = nullptr;
						j_response["error"] = 31;

						response.setHeader("Content-Type", "application/json; charset=utf-8");
						response.write(QByteArray(j_response.dump('\t').c_str()));

						return;
					}
					catch (image_error &e)
					{
						std::cout << e.what() << std::endl;

						j_response["response"]["brightness"] = nullptr;
						j_response["error"] = 32;

						response.setHeader("Content-Type", "application/json; charset=utf-8");
						response.write(QByteArray(j_response.dump('\t').c_str()));

						return;
					}
					catch (mip_error &e)
					{
						std::cout << e.what() << std::endl;

						j_response["response"]["brightness"] = nullptr;
						j_response["error"] = 33;

						response.setHeader("Content-Type", "application/json; charset=utf-8");
						response.write(QByteArray(j_response.dump('\t').c_str()));

						return;
					}

						j_response["response"]["brightness"] = brValue;
						j_response["error"] = nullptr;

						response.setHeader("Content-Type", "application/json; charset=utf-8");
						response.write(QByteArray(j_response.dump('\t').c_str()));

					//	return;
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

						nlohmann::json	j_req, j_response;;

						j_req = nlohmann::json::parse(str);

						const unsigned char *img;
						int  length;




						//GetScreenImage_N(const unsigned char **img, int *length, image_index_t idx, brightness brightness)
					try
					{

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
					}

					catch (modality_error &e)
					{
						std::cout << e.what() << std::endl;

						j_response["response"]["image"] = nullptr;
						j_response["error"] = 31;

						response.setHeader("Content-Type", "application/json; charset=utf-8");
						response.write(QByteArray(j_response.dump('\t').c_str()));

						return;
					}
					catch (image_error &e)
					{
						std::cout << e.what() << std::endl;

						j_response["response"]["image"] = nullptr;
						j_response["error"] = 32;

						response.setHeader("Content-Type", "application/json; charset=utf-8");
						response.write(QByteArray(j_response.dump('\t').c_str()));

						return;
					}
					catch (mip_error &e)
					{
						std::cout << e.what() << std::endl;

						j_response["response"]["image"] = nullptr;
						j_response["error"] = 33;

						response.setHeader("Content-Type", "application/json; charset=utf-8");
						response.write(QByteArray(j_response.dump('\t').c_str()));

						return;
					}

						QByteArray png = QByteArray();

						CreateQByteArrayPngFromChar(png, img, length);

					
						j_response["response"]["image"] = png;//.toBase64();
						j_response["error"] = nullptr;

						response.setHeader("Content-Type", "application/json; charset=utf-8");
						response.write(QByteArray(j_response.dump('\t').c_str()));

						//return;
					}

				}




	qDebug() << "############";
	qDebug() << "SERVICE COMPLETED ID = " << QThread::currentThreadId();
	qDebug() << "############";
}


