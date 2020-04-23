#ifndef WebServerSettings_h__
#define WebServerSettings_h__

#include <XRADBasic/ContainersAlgebra.h>


XRAD_BEGIN

struct WebServerSettings
{

	wstring dicom_folder;

	wstring html_source_path;

	wstring server_ini_file;

};

void	ExportSettings(WebServerSettings &wss);

void	ImportSettngs(WebServerSettings &wss);

XRAD_END


#endif //  WebServerSettings_h__

