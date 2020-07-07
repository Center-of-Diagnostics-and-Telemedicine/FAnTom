﻿#include "pre.h"

#include "CTomogram.h"

#include <XRADBasic/Sources/Utils/ConsoleProgress.h>
#include <FantomLogger.h>

void CTomogram::CalculateInterpolationScales()
{
	m_interpolation_factor.CopyData(m_CTscales / vmin(m_CTscales.z(), m_CTscales.y(), m_CTscales.x()));

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

operation_result CTomogram::LoadByAccession(const wstring accession_number)
{
	START_LOG;
	
	bool acc_loaded = false;

	if (m_accession_number == accession_number && m_proc_acquisition_ptr != nullptr)
	{
		acc_loaded = true;
		return e_successful;
	}

	size_t chosen_position = GetAccessionHeapPosition(accession_number, acc_loaded);

	if (!acc_loaded) return e_out_of_range;

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

	END_LOG;
	return e_successful;
}

operation_result CTomogram::GetModality(string &modality)
{
	modality = modality_t::CT();
	return e_successful;
}

/*
struct image_size_t
{
const string	modality;
const string	image_type;
size_t n_images;
point2_ST	screen_sizes;
point2_ST	dicom_sizes;
};


*/


operation_result CTomogram::GetDimensions(nlohmann::json &j)
{

	nlohmann::json	axial_node, frontal_node, sagittal_node;

	axial_node["n_images"] = CTSlices().sizes()[0];

	axial_node["screen_size_v"] = m_interpolation_sizes.y();
	axial_node["screen_size_h"] = m_interpolation_sizes.x();

	axial_node["dicom_size_v"] = CTSlices().sizes()[1];
	axial_node["dicom_size_h"] = CTSlices().sizes()[2];

	axial_node["dicom_step_v"] = m_CTscales.y();
	axial_node["dicom_step_h"] = m_CTscales.x();



	frontal_node["n_images"] = CTSlices().sizes()[1];

	frontal_node["screen_size_v"] = m_interpolation_sizes.z();
	frontal_node["screen_size_h"] = m_interpolation_sizes.x();

	frontal_node["dicom_size_v"] = CTSlices().sizes()[0];
	frontal_node["dicom_size_h"] = CTSlices().sizes()[2];

	frontal_node["dicom_step_v"] = m_CTscales.z();
	frontal_node["dicom_step_h"] = m_CTscales.x();


	sagittal_node["n_images"] = CTSlices().sizes()[2];

	sagittal_node["screen_size_v"] = m_interpolation_sizes.z();
	sagittal_node["screen_size_h"] = m_interpolation_sizes.y();

	sagittal_node["dicom_size_v"] = CTSlices().sizes()[0];
	sagittal_node["dicom_size_h"] = CTSlices().sizes()[1];

	sagittal_node["dicom_step_v"] = m_CTscales.z();
	sagittal_node["dicom_step_h"] = m_CTscales.y();



	j["response"][modality_t::CT()][image_t::ct_axial()] = axial_node;

	j["response"][modality_t::CT()][image_t::ct_frontal()] = frontal_node;

	j["response"][modality_t::CT()][image_t::ct_sagittal()] = sagittal_node;



/*	v.push_back(
	{ modality_t::CT(),image_t::ct_axial(), CTSlices().sizes()[0],
	{ m_interpolation_sizes.y(),  m_interpolation_sizes.x() },
	{ CTSlices().sizes()[1], CTSlices().sizes()[2] },
	{m_CTscales.y(), m_CTscales.x()} }
	);

	v.push_back(
	{ modality_t::CT(),image_t::ct_frontal(), CTSlices().sizes()[1],
	{ m_interpolation_sizes.z(),  m_interpolation_sizes.x() },
	{ CTSlices().sizes()[0], CTSlices().sizes()[2] },
	{m_CTscales.z(), m_CTscales.x() }  }
	);

	v.push_back(
	{ modality_t::CT(),image_t::ct_sagittal(), CTSlices().sizes()[2],
	{ m_interpolation_sizes.z(),  m_interpolation_sizes.y() },
	{ CTSlices().sizes()[0], CTSlices().sizes()[1] } ,
	{ m_CTscales.z(), m_CTscales.y() } }
	);
*/
	return e_successful;
}

operation_result CTomogram::GetBrightness(double *value, image_index_t idx, size_t y, size_t x)
{
	XRAD_ASSERT_THROW(idx.modality == modality_t::CT());

	size_t  x1 = 0, y1 = 0, z1 = 0;

		if(idx.image_type == image_t::ct_axial())
		{
			x1 = range(x, 0, CTSlices().sizes(2) - 1);
			y1 = range(y, 0, CTSlices().sizes(1) - 1);
			z1 = range(idx.image_no, 0, CTSlices().sizes(0) - 1);
		}
		else if (idx.image_type == image_t::ct_frontal())
		{
			x1 = range(x, 0, CTSlices().sizes(2) - 1);
			y1 = range(idx.image_no, 0, CTSlices().sizes(1) - 1);
			z1 = range(y, 0, CTSlices().sizes(0) - 1);
		}
		else if (idx.image_type == image_t::ct_sagittal())
		{
			x1 = range(idx.image_no, 0, CTSlices().sizes(2) - 1);
			y1 = range(x, 0, CTSlices().sizes(1) - 1);
			z1 = range(y, 0, CTSlices().sizes(0) - 1);
		}
		else throw std::invalid_argument("unknown slice type");
	
	*value = CTSlices().at({ z1, y1, x1 });

	return e_successful;
}

operation_result CTomogram::GetImage(frame_t &img, const image_index_t idx)
{
	XRAD_ASSERT_THROW(idx.modality == modality_t::CT());

		if (idx.image_type == image_t::ct_axial())
		{
			img.MakeCopy(
				m_CTslices.GetSlice({ idx.image_no, slice_mask(0), slice_mask(1) })
						);
			return e_successful;
		}
		else if (idx.image_type == image_t::ct_frontal())
		{
			img.MakeCopy(
				m_CTslices.GetSlice({ slice_mask(0), idx.image_no, slice_mask(1) })
						);
			return e_successful;
		}
		else if (idx.image_type == image_t::ct_sagittal())
		{
			img.MakeCopy(
				m_CTslices.GetSlice({ slice_mask(0), slice_mask(1), idx.image_no })
						);
			return e_successful;
		}
		else throw std::invalid_argument("unknown slice type");
	
}

operation_result CTomogram::GetScreenImage(const unsigned char **img, int *length, image_index_t idx, brightness brightness)
{
	frame_t img_screen;

	if (idx.image_type == image_t::ct_frontal())
	{
		img_screen.realloc(m_interpolation_sizes.z(), m_interpolation_sizes.x());
	}

	else if (idx.image_type == image_t::ct_sagittal())
	{
		img_screen.realloc(m_interpolation_sizes.z(), m_interpolation_sizes.y());
	}

	else if (idx.image_type == image_t::ct_axial())
	{
		img_screen.realloc(m_interpolation_sizes.y(), m_interpolation_sizes.x());
	}

	frame_t	buffer;

	this->GetTomogramSlice(buffer, idx);
	//this->GetImage(buffer, idx);


	RescaleImageFromTomogramToScreenCoordinates(img_screen, buffer, idx.image_type);

	double white = brightness.white;
	double black = brightness.black;
	double gamma = brightness.gamma;

	ApplyFunction(img_screen, [black, white](float x) {return x<black ? 0 : x>white ? 255 : 255.*(x - black) / (white - black); });

	ApplyFunction(img_screen, [gamma](float x) {return 255.*pow(x / 255., gamma); });



	m_bmp[idx.image_type].SetSizes(img_screen.vsize(), img_screen.hsize());

	m_bmp[idx.image_type].palette.realloc(256);

	for (size_t i = 0; i < 256; ++i)
	{
		m_bmp[idx.image_type].palette[i] = static_cast<uint8_t>(i);
	}
	for (size_t i = 0; i < img_screen.vsize(); ++i)
	{
		for (size_t j = 0; j < img_screen.hsize(); ++j)
		{
			m_bmp[idx.image_type].at(i, j) = img_screen.at(img_screen.vsize() - i - 1, j);
		}
	}

	*length = static_cast<int>(m_bmp[idx.image_type].GetBitmapFileSize());

	*img = reinterpret_cast<const unsigned char*>(m_bmp[idx.image_type].GetBitmapFile());

	return e_successful;
}

//operation_result Fantom::RescaleImageFromTomogramToScreenCoordinates(frame_t &rescaled_image, const frame_t &tomogram_slice, slice_type st)
void CTomogram::RescaleImageFromTomogramToScreenCoordinates(frame_t &rescaled_image, const frame_t &tomogram_slice, const string slice_type)
{
	axis_t	v, h;

	if (slice_type == image_t::ct_frontal())
	{
		v = e_z;
		h = e_x;
	}

	else if (slice_type == image_t::ct_sagittal())
	{
		v = e_z;
		h = e_y;
	}

	else if (slice_type == image_t::ct_axial())
	{
		v = e_y;
		h = e_x;
	}

	for (size_t i = 0; i < rescaled_image.vsize(); ++i)
	{
		double y = ScreenToDicomCoordinate(i, v);

		for (size_t j = 0; j < rescaled_image.hsize(); ++j)
		{
			double x = ScreenToDicomCoordinate(j, h);
			rescaled_image.at(i, j) = tomogram_slice.in(y, x, &interpolators2D::ibicubic);
		}
	}

}



double	CTomogram::DicomToScreenCoordinate(double t, axis_t axis)
{
	switch (axis)
	{
	case e_z:
		return double(m_flip_z ? CTSlices().sizes(0) - t - 1 : t) * m_interpolation_factor.z();

	case e_y:
		return double(t) * m_interpolation_factor.y();

	case e_x:
		return double(t) * m_interpolation_factor.x();
	}
	XRAD_ASSERT_THROW_M(false, invalid_argument, "Unknown axis index");
}


double	CTomogram::ScreenToDicomCoordinate(double t, axis_t axis)
{
	switch (axis)
	{
		case e_z:
		{
		double	u = t / m_interpolation_factor.z();
		return double(m_flip_z ? CTSlices().sizes(0) - u - 1 : u);
		}

		case e_y:
		return double(t) / m_interpolation_factor.y();

		case e_x:
		return double(t) / m_interpolation_factor.x();
	}
	XRAD_ASSERT_THROW_M(false, invalid_argument, "Unknown axis index");
}



void CTomogram::CalculateMIP(frame_t &img, image_index_t idx)
{
	auto	mip_function = [&idx](const RealFunctionF32	 &row)->double
	{
		if(idx.mip.mip_method == mip_method_t::average())	return AverageValue(row);

		else if (idx.mip.mip_method == mip_method_t::maxvalue())	return MaxValue(row);
		
		else if (idx.mip.mip_method == mip_method_t::minvalue())	return MinValue(row);
		
		else /*if (mip.mip_method == mip_method_t::mip_no_mip())*/		return row[row.size() / 2];
	};

	// используется двухступенчатая процедура извлечения подмножества, поэтому два разных буфера. неуклюже, можно подумать об улучшении
	RealFunctionMD_F32	acquisition_buffer;
	RealFunctionMD_F32	b1;

	auto	frame_sizes = SliceSizes(idx.image_type);

	img.realloc(frame_sizes.y(), frame_sizes.x(), 0);

	size_t	p0 = range(idx.image_no - idx.mip.mip_value, 0, CTSlicesSize(idx.image_type) - 1);
	size_t	p1 = range(idx.image_no + idx.mip.mip_value, 0, CTSlicesSize(idx.image_type) - 1);

		if (idx.image_type == image_t::ct_axial())
		{
			b1.UseDataFragment(m_CTslices, { p0, 0, 0 }, { p1, m_CTslices.sizes(1), m_CTslices.sizes(2) });
			b1.GetSubset(acquisition_buffer, { slice_mask(0), slice_mask(1), slice_mask(2) });
		}

		else if (idx.image_type == image_t::ct_frontal())
		{
			b1.UseDataFragment(m_CTslices, { 0, p0, 0 }, { m_CTslices.sizes(0), p1, m_CTslices.sizes(2) });
			b1.GetSubset(acquisition_buffer, { slice_mask(1), slice_mask(0), slice_mask(2) });
		}

		else if (idx.image_type == image_t::ct_sagittal())
		{
		b1.UseDataFragment(m_CTslices, { 0, 0, p0 }, { m_CTslices.sizes(0), m_CTslices.sizes(1), p1 });
		b1.GetSubset(acquisition_buffer, { slice_mask(1), slice_mask(2), slice_mask(0) });
		}


	for (size_t i = 0; i < img.vsize(); ++i)
	{
		for (size_t j = 0; j < img.hsize(); ++j)
		{
			img.at(i, j) = mip_function(acquisition_buffer.GetRow({ slice_mask(0),i, j }));
		}
	}
}



void CTomogram::GetTomogramSlice(frame_t &img, image_index_t idx)
{
	//size_t	slice_no = size_t(tomogram_slice_position);
	//TODO заменить на пересчет из z?

	if (idx.mip.mip_method != mip_method_t::none())
	{
		CalculateMIP(img, idx);
	}
	else
	{
		this->GetImage(img, idx);
	}
}


point2_ST	CTomogram::SliceSizes(const string &st) const
{
		if (st == image_t::ct_axial())
		{
			return	point2_ST(CTSlices().sizes(1), CTSlices().sizes(2));
		}
		else if (st == image_t::ct_frontal())
		{
			return	point2_ST(CTSlices().sizes(0), CTSlices().sizes(2));
		}
		else if (st == image_t::ct_sagittal())
		{
		return	point2_ST(CTSlices().sizes(0), CTSlices().sizes(1));
		}
		else 	throw std::invalid_argument("unknown slice type");
}

size_t	CTomogram::CTSlicesSize(const string &st) const
{
		if (st == image_t::ct_axial())
		{
			return	CTSlices().sizes(0);
		}
		else if (st == image_t::ct_frontal())
		{
			return	CTSlices().sizes(1);
		}
		else if (st == image_t::ct_sagittal())
		{
			return	CTSlices().sizes(2);
		}
		else throw std::invalid_argument("unknown slice type");	
}


operation_result CTomogram::GetTomogramDimensions(point3_ST &dimensions)
{
	dimensions.z() = CTSlices().sizes(0);
	dimensions.y() = CTSlices().sizes(1);
	dimensions.x() = CTSlices().sizes(2);

	return e_successful;
}

operation_result CTomogram::GetScreenDimensions(point3_ST &dimensions)
{
	dimensions.z() = m_interpolation_sizes.z();
	dimensions.y() = m_interpolation_sizes.y();
	dimensions.x() = m_interpolation_sizes.x();

	return e_successful;
}



operation_result CTomogram::GetZFlip(bool & flip)
{
	flip = m_flip_z;
	return e_successful;
}