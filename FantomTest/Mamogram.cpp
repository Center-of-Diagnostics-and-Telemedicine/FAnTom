#include "pre.h"

#include "Mamogram.h"

#include <XRADBasic/Sources/Utils/ConsoleProgress.h>

int Mamogram::LoadByAccession(const wstring accession_number)
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

	m_accession_number = accession_number;

	Dicom::instance	&sample_instance = *MMAcquisition_ptr().loader().front();

	m_patient_id = sample_instance.get_wstring(Dicom::e_patient_id);
	m_patient_sex = sample_instance.get_wstring(Dicom::e_patient_sex);
	m_patient_age = sample_instance.get_wstring(Dicom::e_patient_age);
	m_study_id = sample_instance.get_wstring(Dicom::e_study_id);
	m_study_instance_uid = sample_instance.get_wstring(Dicom::e_study_instance_uid);

	size_t number = 1577984U;

	for (Dicom::instance_ptr inst_ptr : MMAcquisition_ptr().loader())
	{
		Dicom::image &slice_container = dynamic_cast<Dicom::image&>(*inst_ptr);

		RealFunction2D_F32 data_slice(slice_container.vsize(), slice_container.hsize());

		slice_container.get_image(data_slice);

		wstring str = inst_ptr->get_wstring(reinterpret_cast<Dicom::tag_e&>(number));

		m_MM_slices[str] = std::move(data_slice);
		//	slices.push_back(std::move(data_slice));
	}

	return 0;
}



void Mamogram::GetImage(frame_t &img, image_index_t idx)
{
	XRAD_ASSERT_THROW(idx.modality == modality_t::MG);

	switch (idx.image_type)
	{
	case e_mg_lcc:
		img.MakeCopy(m_MM_slices[L"LCC"]);
		return;

	case e_mg_rcc:
		img.MakeCopy(m_MM_slices[L"RCC"]);
		return;

	case e_mg_lmlo:
		img.MakeCopy(m_MM_slices[L"LMLO"]);
		return;

	case e_mg_rmlo:
		img.MakeCopy(m_MM_slices[L"RMLO"]);
		return;
	default:
		throw std::invalid_argument("unknown slice type");
	}
}





void  Mamogram::GetBrightness(double *value, image_index_t idx, size_t y, size_t x)
{
	XRAD_ASSERT_THROW(idx.modality == modality_t::MG);


	switch (idx.image_type)
	{
	case e_mg_lcc:

		x = range(x, 0, MMSlices()[L"LCC"].sizes(1) - 1);
		y = range(y, 0, MMSlices()[L"LCC"].sizes(0) - 1);

		*value = MMSlices()[L"LCC"].at(y, x);
		return;

	case e_mg_rcc:

		x = range(x, 0, MMSlices()[L"RCC"].sizes(1) - 1);
		y = range(y, 0, MMSlices()[L"RCC"].sizes(0) - 1);

		*value = MMSlices()[L"RCC"].at(y, x);
		return;

	case e_mg_lmlo:

		x = range(x, 0, MMSlices()[L"LMLO"].sizes(1) - 1);
		y = range(y, 0, MMSlices()[L"LMLO"].sizes(0) - 1);

		*value = MMSlices()[L"LMLO"].at(y, x);
		return;

	case e_mg_rmlo:

		x = range(x, 0, MMSlices()[L"RMLO"].sizes(1) - 1);
		y = range(y, 0, MMSlices()[L"RMLO"].sizes(0) - 1);

		*value = MMSlices()[L"RMLO"].at(y, x);
		return;
	default:
				throw std::invalid_argument("unknown slice type");
	}
}