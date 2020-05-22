#include "pre.h"

#include <QtCore/QCoreApplication>

#include <QtConcurrent/qtconcurrentrun.h>

#include <QtCore/QFuture>
#include <QtCore/QThread>

#include "RequestMapper.h"

#include <FantomLibrary/FantomLibrary.h>

#include <iostream>

#include <Common/WebServerSettings.h>
#include <Common/StringConverters_Qt_Fantom.h>


#ifdef _MSC_VER
	#include <XRADConsoleUI/Sources/PlatformSpecific/MSVC/MSVC_XRADConsoleUILink.h>
	#include <XRADSystem/Sources/PlatformSpecific/MSVC/MSVC_XRADSystemLink.h >
	#include "FantomWebServer_QtLink.h"
	#include "FantomLibrary/MSVC_FantomLibraryLink.h"
#endif //_MSC_VER


//QString	web_server_path;
QString	data_store_path;

void f(HttpListener* listener)
{
	cout << "test" << endl;
}


 int xrad::xrad_main(int argc, char *argv[])
{
	using namespace std;

	QCoreApplication app(argc, argv);

	WebServerSettings wss;

	QString server_ini_file;

	if (argc == 3) 
	{
		data_store_path = string_to_qstring(argv[1]);
		server_ini_file = string_to_qstring(argv[2]);
	}
	else 
	{
		ImportSettngs(wss);

		data_store_path = wstring_to_qstring(wss.dicom_folder);
		server_ini_file = wstring_to_qstring(wss.server_ini_file);
	}


	QSettings* settings_webserver = new QSettings(server_ini_file, QSettings::IniFormat, &app);

	settings_webserver->beginGroup("listener");

//	RequestMapper* handler = new RequestMapper(&app);
	RequestMapper* mapper = new RequestMapper();
	HttpListener* listener = new HttpListener(settings_webserver, mapper, &app);

	QThread  thread;
	QObject::connect(&thread, SIGNAL(started()), mapper, SLOT(LoadFantom()));//Qt::QueuedConnection
    mapper->moveToThread(&thread);
	thread.start();


	//	QObject::connect(handler, SIGNAL(CloseApp()), &thread, SLOT(deleteLater()));//Qt::QueuedConnection

//	QObject::connect(handler, SIGNAL(CloseApp()), settings_webserver, SLOT(deleteLater()), Qt::QueuedConnection);

//	QObject::connect(handler, &RequestMapper::CloseApp, [listener]() {
//		QMetaObject::invokeMethod(listener, "myDestroy", Qt::QueuedConnection); 
//	});//Qt::QueuedConnection

//		QObject::connect(handler, &RequestMapper::CloseApp, [&thread]() {thread.exit(0);});

	QObject::connect(mapper, &RequestMapper::CloseApp, &thread, &QThread::quit);

	QObject::connect(&thread, &QThread::finished, listener, &HttpListener::ForcedDestroy);

//	QObject::connect(handler, &RequestMapper::CloseApp, listener, &HttpListener::myDestroy);

    QObject::connect(listener, SIGNAL(readyToClose()), &app, SLOT(quit()));//Qt::QueuedConnection

//	QObject::connect(handler, &RequestMapper::CloseApp, listener, &HttpListener::myDestroy);

//	QFuture<void> future = QtConcurrent::run(handler, &RequestMapper::LoadFantom);

//	QObject::connect(listener, SIGNAL(readyToClose()), &app, SLOT(quit()));//Qt::QueuedConnection

//	QObject::connect(listener, SIGNAL(readyToClose()), &thread, SLOT(terminate()));//Qt::QueuedConnection

//	QObject::connect(&thread, SIGNAL(finished()), &app, SLOT(quit()));

//	QMetaObject::invokeMethod(freeHandler, "handleConnection", Qt::QueuedConnection, Q_ARG(tSocketDescriptor, socketDescriptor))



	//	QFuture<void> future = QtConcurrent::run(handler, &RequestMapper::LoadFantom);



	return app.exec();
//	return 0;
}



