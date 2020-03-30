#include "pre.h"
#include "FantomSettingsEdit.h"
#include <Sources/XRAD/GUI/DynamicDialog.h>
#include <QtCore/QSettings.h>
#include <iostream>
//#include <QtCore/QCoreApplication.h>

XRAD_USING

using namespace std;

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

	// все сохраняется в ветку реестра
	//HKEY_CURRENT_USER\Software\RPCMR\FAnTom WebServer
	// Наладить чтение оттуда, пополнить все параметры
}


void  ImportSettngs(WebServerSettings &wss)
{
	QSettings settings("HKEY_CURRENT_USER\\Software\\RPCMR\\FAnTom WebServer",
		QSettings::NativeFormat);

	size_t tmpPort;
	tmpPort = settings.value("Port",     0).toInt();

	QString tmpFolder = settings.value("Dicom folder", "C:").toString();

	cout << tmpFolder.toLocal8Bit().constData() << endl;

	wss.port = tmpPort;
}

int xrad::xrad_main(int,char * * const)
{
	try
	{
		auto	dialog = DynamicDialog::OKCancelDialog::Create(L"FAnTom web server settings");
		WebServerSettings	wss;

		ImportSettngs(wss);
	
		wstring	some_string;// = L"aaa";

		dialog->CreateControl<DynamicDialog::ValueDirectoryReadEdit>(L"Каталог с исследованиями для разметки", SavedGUIValue(&wss.dicom_folder), DynamicDialog::Layout::Vertical);
	//	dialog->CreateControl<DynamicDialog::ValueDirectoryReadEdit>(L"Каталог с исследованиями для разметки", SavedGUIValue(&some_string), DynamicDialog::Layout::Vertical);
	//	dialog->CreateControl<DynamicDialog::ValueDirectoryReadEdit>(L"Каталог с исследованиями для разметки", &some_string, DynamicDialog::Layout::Vertical);
	//	dialog->CreateControl<DynamicDialog::StringEdit>(L"какая-то строковая настройка", some_string);

		cout << wss.port << endl;
		cout << &some_string << endl;

		dialog->CreateControl<DynamicDialog::ValueNumberEdit<size_t>>(L"Port", SavedGUIValue(&wss.port), 1024, 65535);



//		dialog->AddControl(FoldersLayout(settings.target_folder, settings.source_folder/*, source_names_list*/));

		dialog->Show();

		wcout << wss.dicom_folder << endl;

		ExportSettings(wss);

		Pause();



	}
	catch(...)
	{

	}
	return 0;
}
