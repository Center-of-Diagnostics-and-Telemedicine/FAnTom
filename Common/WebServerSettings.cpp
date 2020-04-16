#include "pre.h"
#include <QtCore/QSettings.h>


#include "WebServerSettings.h"

XRAD_BEGIN

const string OrganizationName = "XRAD";

const string ApplicationName = "FAnTom WebServer";

const string DicomFolder = "DicomFolder";

const string DicomTextFolder = "DicomTextFolder";

const string HtmlSourceFolder = "HtmlSourceFolder";

const string ServerIniFile = "Server .ini file";


void	ExportSettings(WebServerSettings &wss)
{

	QSettings	settings(QSettings::NativeFormat, QSettings::UserScope, OrganizationName.c_str(), ApplicationName.c_str());

	settings.setValue(DicomFolder.c_str(), convert_to_string(wss.dicom_folder).c_str());

	settings.setValue(DicomTextFolder.c_str(), convert_to_string(wss.dicom_text_folder).c_str());

	settings.setValue(HtmlSourceFolder.c_str(), convert_to_string(wss.html_source_path).c_str());

	settings.setValue(ServerIniFile.c_str(), convert_to_string(wss.server_ini_file).c_str());

}


void  ImportSettngs(WebServerSettings &wss)
{

	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, OrganizationName.c_str(), ApplicationName.c_str());

	wss.dicom_folder = settings.value(DicomFolder.c_str(), "C:/temp").toString().toStdWString();

	wss.dicom_text_folder = settings.value(DicomTextFolder.c_str(), "C:/temp").toString().toStdWString();

	wss.html_source_path = settings.value(HtmlSourceFolder.c_str(), "C:/temp").toString().toStdWString();

	wss.server_ini_file = settings.value(ServerIniFile.c_str(), "C:/temp/webserver.ini").toString().toStdWString();

};

XRAD_END