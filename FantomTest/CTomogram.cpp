#include "pre.h"

#include "CTomogram.h"

//#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>
//#include <XRADBasic/ThirdParty/nlohmann/json.hpp>


#include <iostream>

void CTomogram::CalculateInterpolationScales()
{
	m_interpolation_factor.CopyData(m_CTscales / min(m_CTscales.x(), m_CTscales.y()));

	m_interpolation_sizes =
	{
		size_t(m_CTslices.sizes(0) * m_interpolation_factor.z()),
		size_t(m_CTslices.sizes(1) * m_interpolation_factor.y()),
		size_t(m_CTslices.sizes(2) * m_interpolation_factor.x())
	};

	// учет укладки пациента
	double	position_0 = m_image_positions_patient[0].z();
	double	position_1 = m_image_positions_patient[m_image_positions_patient.size() - 1].z();
	m_flip_z = position_0 < position_1;

	return;// e_successful;
}

int CTomogram::LoadByAccession(const wstring accession_number)
{
	bool acc_loaded = false;

	if (m_accession_number == accession_number && m_proc_acquisition_ptr != nullptr)
	{
		acc_loaded = true;
		return 0;// e_successful;
	}

	size_t chosen_position = GetAccessionHeapPosition(accession_number, acc_loaded);

	if (!acc_loaded) return -1;

	m_proc_acquisition_ptr = CreateProcessAcquisition(GetLargestAcquisition(chosen_position), ConsoleProgressProxy());

	ProcessAcquisitionOpenClose prcAcq(*m_proc_acquisition_ptr);
	//	proc_acquisition_work_ptr->open_instancestorages();

	m_CTslices = CTAcquisition_ptr().slices();
	m_CTscales = CTAcquisition_ptr().scales();
	m_image_positions_patient = CTAcquisition_ptr().image_positions_patient();

	CalculateInterpolationScales();

	m_accession_number = accession_number;

	Dicom::instance	&sample_instance = *CTAcquisition_ptr().loader().front();

	m_patient_id = sample_instance.get_wstring(Dicom::e_patient_id);
	m_patient_sex = sample_instance.get_wstring(Dicom::e_patient_sex);
	m_patient_age = sample_instance.get_wstring(Dicom::e_patient_age);
	m_study_id = sample_instance.get_wstring(Dicom::e_study_id);
	m_study_instance_uid = sample_instance.get_wstring(Dicom::e_study_instance_uid);

	return 0;
}

void CTomogram::GetBrightness(double &value, image_index_t idx, size_t y, size_t x)
{
	point3_ST tomogram_sample_position{ idx.image_no, y, x };

	tomogram_sample_position.x() = range(tomogram_sample_position.x(), 0, CTSlices().sizes(2) - 1);
	tomogram_sample_position.y() = range(tomogram_sample_position.y(), 0, CTSlices().sizes(1) - 1);
	tomogram_sample_position.z() = range(tomogram_sample_position.z(), 0, CTSlices().sizes(0) - 1);


	value = CTSlices().at({ tomogram_sample_position.z(), tomogram_sample_position.y(), tomogram_sample_position.x() });

	return;// e_successful;

}

void CTomogram::GetImage(frame_t &img, image_index_t idx)
{
	switch (idx.image_type)
	{
	case	e_ct_axial:
		img.MakeCopy(
			m_CTslices.GetSlice({ idx.image_no, slice_mask(0), slice_mask(1) })
		);
		return;
	case	e_ct_frontal:
		img.MakeCopy(
			m_CTslices.GetSlice({ slice_mask(0), idx.image_no, slice_mask(1) })
		);
		return;
	case	e_ct_sagittal:
		img.MakeCopy(
			m_CTslices.GetSlice({ slice_mask(0), slice_mask(1), idx.image_no })
		);
		return;
	default:
		throw std::invalid_argument("unknown slice type");
	}
}