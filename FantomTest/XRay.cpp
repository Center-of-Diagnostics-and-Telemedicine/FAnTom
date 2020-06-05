#include "pre.h"

#include "XRay.h"

#include <XRADBasic/Sources/Utils/ConsoleProgress.h>

int XRay::LoadByAccession(const wstring accession_number)
{
	bool acc_loaded = false;

	if (m_accession_number == accession_number && m_proc_acquisition_ptr != nullptr)
	{
		acc_loaded = true;
		return 0;// e_successful;
	}

	size_t chosen_position = GetAccessionHeapPosition(accession_number, acc_loaded);

	if (!acc_loaded) return -1;

	m_proc_acquisition_ptr = CreateProcessAcquisition(GetInstancesOfStudy(chosen_position), ConsoleProgressProxy());

	ProcessAcquisitionOpenClose prcAcq(*m_proc_acquisition_ptr);
	//	proc_acquisition_work_ptr->open_instancestorages();

	m_XRslices = XrayAcquisition_ptr().slices();

	m_accession_number = accession_number;

	Dicom::instance	&sample_instance = *XrayAcquisition_ptr().loader().front();

	m_patient_id = sample_instance.get_wstring(Dicom::e_patient_id);
	m_patient_sex = sample_instance.get_wstring(Dicom::e_patient_sex);
	m_patient_age = sample_instance.get_wstring(Dicom::e_patient_age);
	m_study_id = sample_instance.get_wstring(Dicom::e_study_id);
	m_study_instance_uid = sample_instance.get_wstring(Dicom::e_study_instance_uid);

	size_t number = 1577984U;
	for (Dicom::instance_ptr inst_ptr : XrayAcquisition_ptr().loader())
	{
		wstring str = inst_ptr->get_wstring(reinterpret_cast<Dicom::tag_e&>(number));
	}

	return 0;
}


void XRay::GetImage(frame_t &img, image_index_t idx)
{
	img.MakeCopy(XRSlices()[idx.image_no]);
}