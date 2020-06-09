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

	m_XR_slices = XrayAcquisition_ptr().slices();

	m_accession_number = accession_number;

	Dicom::instance	&sample_instance = *XrayAcquisition_ptr().loader().front();

	m_patient_id = sample_instance.get_wstring(Dicom::e_patient_id);
	m_patient_sex = sample_instance.get_wstring(Dicom::e_patient_sex);
	m_patient_age = sample_instance.get_wstring(Dicom::e_patient_age);
	m_study_id = sample_instance.get_wstring(Dicom::e_study_id);
	m_study_instance_uid = sample_instance.get_wstring(Dicom::e_study_instance_uid);

	return 0;
}

void XRay::GetImage(frame_t &img, image_index_t idx)
{
	XRAD_ASSERT_THROW(idx.modality == modality_t::DX);

	size_t no = range(idx.image_no, 0, XRSlices().size() - 1);

	img.MakeCopy(XRSlices()[no]);
}


void XRay::GetBrightness(double *value, image_index_t idx, size_t y, size_t x)
{ 
	XRAD_ASSERT_THROW(idx.modality == modality_t::DX);

	size_t no = range(idx.image_no, 0, XRSlices().size() - 1);

	x = range(x, 0, XRSlices()[no].sizes(1) - 1);
	y = range(y, 0, XRSlices()[no].sizes(0) - 1);

	*value = XRSlices()[no].at(y, x);
}