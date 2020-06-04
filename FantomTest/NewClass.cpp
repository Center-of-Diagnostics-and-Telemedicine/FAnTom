#include "pre.h"

#include "NewClass.h"

#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>
#include <XRADBasic/ThirdParty/nlohmann/json.hpp>


#include <iostream>
#include <fstream>


void SliceManager::InitHeap(const wstring& dicom_folder)
{
	Dicom::patients_loader patients_heap = GetDicomStudiesHeap(
		Dicom::datasource_folder(dicom_folder, true),
		MakeDicomInstanceFilters(),
		GUIProgressProxy());
		//			ConsoleProgressProxy());

	for (auto &patient : patients_heap)
	{
		for (auto &study : patient)
		{
		m_studies_heap.push_back(study);
		m_accession_numbers.push_back( study.complete_study_id().accession_number() );
		}
	}

//	Dicom::instance_ptr sample_instance = m_studies_heap.front().front().front().front().front();
//	wstring luf = sample_instance->dicom_container()->last_used_instancestorage()->print();
}


void CTomogram::ProcessAcqusionByAccessionNumber(const wstring accession_number) 
{


}