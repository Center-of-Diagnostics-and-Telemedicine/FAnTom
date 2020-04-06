#include "pre.h"
#include "FantomSettingsEdit.h"
#include <Sources/XRAD/GUI/DynamicDialog.h>
#include <QtCore/QSettings.h>
#include <iostream>
#include <Common/WebServerSettings.h>
//#include <QtCore/QCoreApplication.h>

XRAD_USING

using namespace std;


int xrad::xrad_main(int,char * * const)
{
	try
	{
		auto	dialog = DynamicDialog::OKCancelDialog::Create(L"FAnTom web server settings");

		WebServerSettings	wss;

		ImportSettngs(wss);
	
		//	dialog->CreateControl<DynamicDialog::ValueDirectoryReadEdit>(L"Каталог с исследованиями для разметки", SavedGUIValue(&wss.dicom_folder), DynamicDialog::Layout::Vertical);

		dialog->CreateControl<DynamicDialog::ValueDirectoryReadEdit>(L"Каталог с исследованиями для разметки", &wss.dicom_folder, DynamicDialog::Layout::Vertical);

		dialog->CreateControl<DynamicDialog::ValueDirectoryReadEdit>(L"Каталог с текстовых файлов", &wss.dicom_text_folder, DynamicDialog::Layout::Vertical);

		dialog->CreateControl<DynamicDialog::ValueDirectoryReadEdit>(L"Каталог html файлов сервера", &wss.html_source_path, DynamicDialog::Layout::Vertical);


		dialog->CreateControl<DynamicDialog::ValueFileLoadEdit>(L"Выбор файла внутренних настроек сервера", &wss.server_ini_file,  L"*.ini", DynamicDialog::Layout::Vertical);

		dialog->CreateControl<DynamicDialog::ValueFileLoadEdit>(L"Выбор файла списка докторов", &wss.doctor_ini_file, L"*.ini", DynamicDialog::Layout::Vertical);



	//	dialog->CreateControl<DynamicDialog::StringEdit>(L"какая-то строковая настройка", some_string);
	//	dialog->CreateControl<DynamicDialog::ValueNumberEdit<size_t>>(L"Port", SavedGUIValue(&wss.port), 1024, 65535);

	//!!!	dialog->CreateControl<DynamicDialog::ValueNumberEdit<size_t>>(L"Port", &wss.port, 1024, 65535);

   //		dialog->AddControl(FoldersLayout(settings.target_folder, settings.source_folder/*, source_names_list*/));

		dialog->Show();

		ExportSettings(wss);

	}
	catch(...)
	{

	}
	return 0;
}
