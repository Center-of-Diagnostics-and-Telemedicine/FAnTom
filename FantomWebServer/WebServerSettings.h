#ifndef WebServerSettings_h__
#define WebServerSettings_h__

#include <XRADBasic/ContainersAlgebra.h>


XRAD_BEGIN

struct WebServerSettings
{
//!!!	int	port = 5555;
	wstring dicom_folder;
	wstring dicom_text_folder;
	wstring html_source_path;
	wstring server_ini_file;
	wstring doctor_ini_file;

};

void  ImportSettngs(WebServerSettings &wss)
{
	QSettings settings("HKEY_CURRENT_USER\\Software\\RPCMR\\FAnTom WebServer",
		QSettings::NativeFormat);

	//!!!	wss.port = settings.value("Port", 0).toInt();

	wss.dicom_folder = settings.value("DicomFolder", "C:/temp").toString().toStdWString();

	wss.dicom_text_folder = settings.value("DicomTextFolder", "C:/temp").toString().toStdWString();

	wss.html_source_path = settings.value("HtmlSourceFolder", "C:/temp").toString().toStdWString();

	wss.server_ini_file = settings.value("Server .ini file", "C:/temp/webserver.ini").toString().toStdWString();

	wss.doctor_ini_file = settings.value("Doctors .ini file", "C:/temp/doctor_database.ini").toString().toStdWString();

};

XRAD_END


#endif //  WebServerSettings_h__

