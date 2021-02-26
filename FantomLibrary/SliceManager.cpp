#include "pre.h"

#include "SliceManager.h"

//#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>
//#include <XRADBasic/ThirdParty/nlohmann/json.hpp>

//#include <iostream>


operation_result SliceManager::InitHeap(const wstring& dicom_folder)
{
	Dicom::patients_loader patients_heap = GetDicomStudiesHeap(
		Dicom::datasource_folder(dicom_folder, true),
		MakeDicomInstanceFilters(),
		ConsoleProgressProxy());

// 	for (auto &patient : patients_heap)
// 	{
// 		for (auto &study : patient)
// 		{
// 			GetStudy().push_back(study);
// 			GetStudy().push_back( study.complete_study_id().accession_number() );
// 		}
// 	}
	GetStudy() = patients_heap.front().front();

	return e_successful;
}

// size_t SliceManager::GetAccessionHeapPosition(const wstring &accession_number, bool &acc_found)
// {
// 	acc_found = false;
// 	size_t series_position = 0;
// 	for (size_t i = 0; i < m_studies_heap.size(); ++i)
// 	{
// 		if ((accession_number == m_studies_heap[i].complete_study_id().accession_number()) || (accession_number == m_studies_heap[i].complete_study_id().study_id()))
// 		{
// 			acc_found = true;
// 			series_position = i;
// 		}
// 	}
// 	if (!acc_found)
// 	{
// 		return 0;
// 	}
// 	else
// 	{
// 		return series_position;
// 	}
// }


Dicom::acquisition_loader& SliceManager::GetLargestAcquisition()
{
	Dicom::acquisition_loader *result = &GetStudy().front().front().front();
	size_t sz{ result->size() };

	for (auto &ser :  m_Study)
	{
		for (auto &stack : ser)
		{
			for (auto &acq : stack)
			{
				if (acq.size() > sz)
				{
					result = &acq;
					sz = acq.size();
				}
			}
		}
	}
	return *result;
}

Dicom::acquisition_loader SliceManager::GetInstancesOfStudy()
{
	vector<Dicom::instance_ptr> result;

	for (auto &ser : m_Study)
	{
		for (auto &stack : ser)
		{
			for (auto &acq : stack)
			{
				for (auto &instance : acq)
				{
					result.push_back(instance);
				}
			}
		}
	}

	Dicom::acquisition_loader all(result.size(), L"");
	//	size_t sz{ result.size() };

	for (size_t i = 0; i < result.size(); i++)
	{
		all.push_back(result[i]);
	}

	return all;
}