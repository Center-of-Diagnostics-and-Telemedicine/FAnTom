#include "pre.h"

#include <QtCore/QCoreApplication>

#include <QtConcurrent/qtconcurrentrun.h>

#include <QtCore/QFuture>
#include <QtCore/QThread>

#include "RequestMapper.h"

#include <FantomLibrary/FantomLibrary.h>

#include <iostream>

#include <Common/WebServerSettings.h>

#ifdef _MSC_VER
	#include <XRADConsoleUI/Sources/PlatformSpecific/MSVC/MSVC_XRADConsoleUILink.h>
	#include <XRADSystem/Sources/PlatformSpecific/MSVC/MSVC_XRADSystemLink.h >
	#include "FantomWebServer_QtLink.h"
	#include "FantomLibrary/MSVC_FantomLibraryLink.h"
#endif //_MSC_VER


QString	web_server_path;
QString	data_store_path;


void test()
{

}


 int xrad::xrad_main(int argc, char *argv[])
{
	using namespace std;

	QCoreApplication app(argc, argv);

		WebServerSettings wss;
		ImportSettngs(wss);

	web_server_path = QString::fromStdWString(wss.html_source_path) +"/";

	data_store_path = QString::fromStdWString(wss.dicom_folder);

	QString server_ini_file = QString::fromStdWString(wss.server_ini_file);
	QSettings* settings_webserver = new QSettings(server_ini_file, QSettings::IniFormat, &app);


	settings_webserver->beginGroup("listener");
	int port = settings_webserver->value("port").toInt();

	RequestMapper* handler = new RequestMapper(&app);

	HttpListener* listener = new HttpListener(settings_webserver, handler, &app);

	//QFuture<void> future = QtConcurrent::run(this, &RequestMapper::LoadFantom);

	QFuture<void> future = QtConcurrent::run(handler, &RequestMapper::LoadFantom);

	//QThread myThrd;

	//handler->moveToThread(&myThrd);

	//(&myThrd, &QThread::started, handler, &RequestMapper::LoadFantom);

	return app.exec();
}



