#include "pre.h"
/*

*/
#include <QtCore/QCoreApplication>

#include "RequestMapper.h"

#include <FantomLibrary.h>

#include <iostream>

#ifdef _MSC_VER
	#include <XRADConsoleUI/Sources/PlatformSpecific/MSVC/MSVC_XRADConsoleUILink.h>
	#include <XRADSystem/Sources/PlatformSpecific/MSVC/MSVC_XRADSystemLink.h >
//	#include <XRAD/PlatformSpecific/MSVC/PC_XRADConsoleUILink.h>
	#include "FantomWebServer_QtLink.h"
    #include "WebServerSettings.h"
	#pragma comment(lib, "FantomLibrary.lib")
#endif //_MSC_VER


//QString	ini_path = "q:/Projects/Fantom/FantomWebServer/webserver.ini";
QString	web_server_path;
QString	data_store_path;
QString	text_file_path;
QMultiMap<QByteArray, QByteArray> doctor_database_map;




int xrad::xrad_main(int argc, char *argv[])
{
	using namespace std;

	QCoreApplication app(argc, argv);

	WebServerSettings wss;

	ImportSettngs(wss);

//	int port = wss.port;

	data_store_path = QString::fromStdWString(wss.dicom_folder);

	text_file_path = QString::fromStdWString(wss.dicom_text_folder);

	web_server_path = QString::fromStdWString(wss.html_source_path);


	wcout << wss.server_ini_file << endl;

	QString server_ini_file = QString::fromStdWString(wss.server_ini_file);

	//QString server_ini_file = QString::fromStdString("Q:/Projects/FAnTom/WebServerSources/webserver.ini");

	QString doctor_ini_file = QString::fromStdWString(wss.doctor_ini_file);


	QSettings* settings_webserver=new QSettings(server_ini_file, QSettings::IniFormat, &app);

	settings_webserver->beginGroup("listener");
	cout << "port = " << settings_webserver->value("port").toInt() << endl;
	settings_webserver->endGroup();

	QSettings* settings_doctor_database = new QSettings(doctor_ini_file, QSettings::IniFormat, &app);

	for (size_t i = 1; i < 4 /*note Kovbas это кол-во записей о врачах в базе врачей*/; ++i)
	{
		QString doctor_ini_group = QString("doctor_%1").arg(i);
		settings_doctor_database->beginGroup(doctor_ini_group);
		QByteArray doctor_id_buff = settings_doctor_database->value("doctor_id").toByteArray();
		QByteArray doctor_pass_buff = settings_doctor_database->value("password").toByteArray();
		doctor_database_map.insert(doctor_id_buff, doctor_pass_buff);
		settings_doctor_database->endGroup();
	}

	settings_webserver->beginGroup("listener");
 	int port = settings_webserver->value("port").toInt();
	settings_webserver->endGroup();

	cout << port << endl;

	RequestMapper* handler=new RequestMapper(&app, port);


	HttpListener* listener=new HttpListener(settings_webserver, handler, &app);

	return app.exec();
}


//int xrad::xrad_main(int argc, char *argv[])
//{
//	using namespace std;
//
//	QCoreApplication app(argc, argv);
//	app.setApplicationName("DicomMarkerServer");
//	app.setOrganizationName("RPCMR");
//	QString	app_path = app.applicationDirPath();
//
//	cout << "app path = " << app_path.toLocal8Bit().constData() << endl;
//
//	//#error здесь идет обращение к файлу с настройками. Организовано очень плохо. С реорганизации настроек следует начать
//
//	QString	web_sources_ini_filename = app_path + "/websources.ini";
//	QSettings* web_sources_settings = new QSettings(web_sources_ini_filename, QSettings::IniFormat, &app);
//	web_sources_settings->beginGroup("websources");
//	QString web_sources_path = web_sources_settings->value("WebSourcesPath").toString();
//
//	QString	web_server_path_app = web_sources_path + "/WebServerData/";
//	web_server_path = web_server_path_app;
//
//	QString	server_ini_filename = web_sources_path + "/webserver.ini";
//	QSettings* settings_webserver = new QSettings(server_ini_filename, QSettings::IniFormat, &app);
//
//	QString	data_ini_filename = web_sources_path + "/datastore.ini";
//	QSettings* settings_datastore = new QSettings(data_ini_filename, QSettings::IniFormat, &app);
//
//	QString	doctor_ini_filename = web_sources_path + "/doctor_database.ini";
//	QSettings* settings_doctor_database = new QSettings(doctor_ini_filename, QSettings::IniFormat, &app);
//
//	for (size_t i = 1; i < 4 /*note Kovbas это кол-во записей о врачах в базе врачей*/; ++i)
//	{
//		QString doctor_ini_group = QString("doctor_%1").arg(i);
//		settings_doctor_database->beginGroup(doctor_ini_group);
//		QByteArray doctor_id_buff = settings_doctor_database->value("doctor_id").toByteArray();
//		QByteArray doctor_pass_buff = settings_doctor_database->value("password").toByteArray();
//		doctor_database_map.insert(doctor_id_buff, doctor_pass_buff);
//		settings_doctor_database->endGroup();
//	}
//
//	settings_webserver->beginGroup("listener");
//	int port = settings_webserver->value("port").toInt();
//
//	settings_datastore->beginGroup("datastore");
//	data_store_path = settings_datastore->value("dataStorePath").toString();
//	text_file_path = settings_datastore->value("textfilepath").toString();
//
//	RequestMapper* handler = new RequestMapper(&app, port);
//
//
//	HttpListener* listener = new HttpListener(settings_webserver, handler, &app);
//
//	return app.exec();
//}

