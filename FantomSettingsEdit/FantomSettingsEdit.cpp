#include "pre.h"
#include "FantomSettingsEdit.h"
#include <Sources/XRAD/GUI/DynamicDialog.h>
#include <QtCore/QSettings.h>
//#include <QtCore/QCoreApplication.h>

XRAD_USING

struct WebServerSettings
{
	size_t	port = 5555;
	wstring dicom_folder;
};


void		ExportSettings(WebServerSettings &wss)
{
	int	dummy_n = 0;
	char	**argv = NULL;

	QString	app_name("FAnTom WebServer");
	QString	org_name("RPCMR");

	QSettings	settings(QSettings::NativeFormat, QSettings::UserScope, org_name, app_name);
	settings.setValue("Port", wss.port);
	settings.setValue("Dicom folder", convert_to_string(wss.dicom_folder).c_str());

	// все сохран€етс€ в ветку реестра
	//HKEY_CURRENT_USER\Software\RPCMR\FAnTom WebServer
	// Ќаладить чтение оттуда, пополнить все параметры
}

int xrad::xrad_main(int,char * * const)
{
	try
	{
		auto	dialog = DynamicDialog::OKCancelDialog::Create(L"FAnTom web server settings");
		WebServerSettings	wss;
	
//		wstring	some_string;

		dialog->CreateControl<DynamicDialog::ValueDirectoryReadEdit>(L" аталог с исследовани€ми дл€ разметки", SavedGUIValue(&wss.dicom_folder), DynamicDialog::Layout::Vertical);
//		dialog->CreateControl<DynamicDialog::StringEdit>(L"кака€-то строкова€ настройка", SavedGUIValue(some_string));
		dialog->CreateControl<DynamicDialog::ValueNumberEdit<size_t>>(L"Port", SavedGUIValue(&wss.port), 1024, 65535);

//		dialog->AddControl(FoldersLayout(settings.target_folder, settings.source_folder/*, source_names_list*/));

		dialog->Show();

		ExportSettings(wss);



	}
	catch(...)
	{

	}
	return 0;
}
