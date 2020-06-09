#ifndef SliceManager_h__
#define SliceManager_h__

#include <iostream>

#include <XRADBasic/MathFunctionTypesMD.h>
#include <XRADDicom/XRADDicom.h>
#include "SliceDefs.h"

//#include "XRADDicom\Sources\DicomClasses\ProcessContainers\XRAYAcquisition.h"

XRAD_USING

class SliceManager
{
protected:

	vector<Dicom::study_loader> m_studies_heap;
	shared_ptr<ProcessAcquisition> m_proc_acquisition_ptr;

	wstring m_patient_id;
	wstring m_patient_sex;
	wstring m_patient_age;

	wstring m_accession_number;
	wstring m_study_id;
	wstring m_study_instance_uid;

	vector<wstring> m_accession_numbers;

	void InitHeap(const wstring& dicom_folder);
	void HeapDump(const wstring& dump_file);

	wstring GetAccNumber(size_t no) { if (no < m_accession_numbers.size()) return m_accession_numbers[no]; else return L"no num"; }

	size_t GetAccessionHeapPosition(const wstring &accession_number, bool &acc_found);

	Dicom::acquisition_loader& GetLargestAcquisition(size_t chosen_position);
	Dicom::acquisition_loader GetInstancesOfStudy(size_t chosen_position);

private:

	virtual int LoadByAccession(const wstring accession_number) = 0;

	virtual void GetImage(frame_t &img, image_index_t idx) = 0;

	virtual void GetScreenImage(const unsigned char **img, int *length, image_index_t idx, double black, double white, double gamma, mip_index_t mip) = 0;

	virtual void GetBrightness(double *value, image_index_t idx, size_t y, size_t x) = 0;

};



#endif // SliceManager_h__
