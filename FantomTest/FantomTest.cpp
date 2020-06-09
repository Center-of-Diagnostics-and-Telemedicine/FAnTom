#include "pre.h"
#include "TestFunctions.h"

#ifdef _MSC_VER
#include <FantomLibrary/MSVC_FantomLibraryLink.h>
#endif

XRAD_USING


int	xrad::xrad_main(int n, char *names[])
{
	try
	{
		XRAD_ASSERT_THROW_M(n <= 2, invalid_argument, "Укажите только одну папку с данными формата DICOM");
		wstring	foldername = (n == 2) ? convert_to_wstring(names[1]) : L"";
		if (foldername.size() == 0)
		{
			//foldername = L"D:/websrv_marktomogram/data_store/2015_08_KT_subsets/subset";
			//foldername = L"D:/websrv_marktomogram/data_store/multiple_accession_test";
			foldername = L"C:/xray/tuberculosis";//L"c:/Dicom";
	
		}
		//auto m_studies_heap = GetDicomStudiesHeap(Dicom::datasource_folder(foldername, true), MakeDicomStudiesFilters(), VoidProgressProxy());
		//Dicom::acquisition_loader ct = SelectSeriesInteractive(m_studies_heap);
	//	size_t continue_option = 0;
	//	do
	//	{
			
			TestTomogram();
			//TestXRAYImage();
			Pause();
	//		continue_option = Decide("Choose option", {"Select another slice?", "Exit" });
	//	}
	//	while (continue_option == 0);
		return 0;
	}

	catch(canceled_operation &)
	{
		return 0;
	}
	catch(quit_application &)
	{
		return 0;
	}
	catch(exception &ex)
	{
		ShowString("An exception occured:", ex.what());
	}
	catch(...)
	{
		Error("Unknown exception");
	}

	return 1;
}
