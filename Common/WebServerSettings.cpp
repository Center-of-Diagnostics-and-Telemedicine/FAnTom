#include "pre.h"
#include "WebServerSettings.h"

#include <QtCore/QSettings>
#include <XRADQt/QtStringConverters.h>

XRAD_BEGIN

const string OrganizationName = "XRAD";

const string ApplicationName = "FAnTom WebServer";

const string DicomFolder = "DicomFolder";

//const string HtmlSourceFolder = "HtmlSourceFolder";

const string ServerIniFile = "Server .ini file";





void	ExportSettings(WebServerSettings &wss)
{
	QSettings	settings(QSettings::NativeFormat, QSettings::UserScope, OrganizationName.c_str(), ApplicationName.c_str());

	string str = convert_to_string8(wss.dicom_folder);

	const char* ttt = convert_to_string8(wss.dicom_folder).c_str();

	settings.setValue(DicomFolder.c_str(), convert_to_string8(wss.dicom_folder).c_str());

	settings.setValue(ServerIniFile.c_str(), convert_to_string8(wss.server_ini_file).c_str());

}


void  ImportSettngs(WebServerSettings &wss)
{
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, OrganizationName.c_str(), ApplicationName.c_str());

	QVariant V1 = settings.value(DicomFolder.c_str(), "C:/temp");

	wss.dicom_folder = qstring_to_wstring(V1.toString());

	QVariant V2 = settings.value(ServerIniFile.c_str(), "C:/temp/webserver.ini");

	wss.server_ini_file = qstring_to_wstring(V2.toString());
};

XRAD_END