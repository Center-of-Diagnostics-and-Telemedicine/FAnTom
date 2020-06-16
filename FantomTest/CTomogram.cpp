#include "pre.h"

#include "CTomogram.h"



//#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>
//#include <XRADBasic/ThirdParty/nlohmann/json.hpp>


#include <iostream>


void CTomogram::CreateQByteArrayPngFromChar(QByteArray &png, const unsigned char *img, int length, const wstring &format)
{
	QImage q_image;
	QByteArray tmp;

	if (q_image.loadFromData(img, length, ".bmp"))
	{
		QBuffer buffer;
		buffer.open(QIODevice::ReadWrite);
		//q_image.save(&buffer, "bmp"); // writes pixmap into bytes in BMP format
		q_image.save(&buffer, convert_to_string(format).c_str());	// writes pixmap into bytes in PNG format
																	
		q_image.save("C:/temp/bbb.png", "png");
		//tmp = buffer.buffer();
		png = buffer.buffer().toBase64();
	}
	
}



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

void CTomogram::GetBrightness(double *value, image_index_t idx, size_t y, size_t x)
{
//	XRAD_ASSERT_THROW(idx.modality == modality_t::CT);

	size_t  x1 = 0, y1 = 0, z1 = 0;

	switch (idx.image_type)
	{
		case e_ct_axial:
		{
			x1 = range(x, 0, CTSlices().sizes(2) - 1);
			y1 = range(y, 0, CTSlices().sizes(1) - 1);
			z1 = range(idx.image_no, 0, CTSlices().sizes(0) - 1);
			break;
		}
		case e_ct_frontal:
		{
			x1 = range(x, 0, CTSlices().sizes(2) - 1);
			y1 = range(idx.image_no, 0, CTSlices().sizes(1) - 1);
			z1 = range(y, 0, CTSlices().sizes(0) - 1);
			break;
		}
		case e_ct_sagittal:
		{
			x1 = range(idx.image_no, 0, CTSlices().sizes(2) - 1);
			y1 = range(x, 0, CTSlices().sizes(1) - 1);
			z1 = range(y, 0, CTSlices().sizes(0) - 1);
			break;
		}
		default:
			throw std::invalid_argument("unknown slice type");
	}

	*value = CTSlices().at({ z1, y1, x1 });

	return;// e_successful;

}

void CTomogram::GetImage(frame_t &img, image_index_t idx)
{
	XRAD_ASSERT_THROW(idx.modality == modality_t::CT());

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

void CTomogram::GetScreenImage(const unsigned char **img, int *length, image_index_t idx, double black, double white, double gamma, mip_index_t mip)
{
	frame_t img_screen;

	switch (idx.image_type)
	{
	case e_ct_frontal:
		img_screen.realloc(m_interpolation_sizes.z(), m_interpolation_sizes.x());
		break;

	case e_ct_sagittal:
		img_screen.realloc(m_interpolation_sizes.z(), m_interpolation_sizes.y());
		break;

	case e_ct_axial:
	default:
		img_screen.realloc(m_interpolation_sizes.y(), m_interpolation_sizes.x());
		break;
	}
	frame_t	buffer;

	this->GetTomogramSlice(buffer, idx, mip);
	//this->GetImage(buffer, idx);

	RescaleImageFromTomogramToScreenCoordinates(img_screen, buffer, idx.image_type);

	ApplyFunction(img_screen, [black, white](float x) {return x<black ? 0 : x>white ? 255 : 255.*(x - black) / (white - black); });

	ApplyFunction(img_screen, [gamma](float x) {return 255.*pow(x / 255., gamma); });



	m_bmp.SetSizes(img_screen.vsize(), img_screen.hsize());

	m_bmp.palette.realloc(256);

	for (size_t i = 0; i < 256; ++i)
	{
		m_bmp.palette[i] = static_cast<uint8_t>(i);
	}
//	m_bmp[idx.image_type].CopyData(img_screen);

	m_bmp.CopyData(img_screen);

	*length = static_cast<int>(m_bmp.GetBitmapFileSize());

	*img = reinterpret_cast<const unsigned char*>(m_bmp.GetBitmapFile());
}

//operation_result Fantom::RescaleImageFromTomogramToScreenCoordinates(frame_t &rescaled_image, const frame_t &tomogram_slice, slice_type st)
void CTomogram::RescaleImageFromTomogramToScreenCoordinates(frame_t &rescaled_image, const frame_t &tomogram_slice, image_t slice_type)
{
	axis_t	v, h;

	switch (slice_type)
	{
	case e_ct_frontal:
		v = e_z;
		h = e_x;
		break;

	case e_ct_sagittal:
		v = e_z;
		h = e_y;
		break;

	case e_ct_axial:
	default:
		v = e_y;
		h = e_x;
		break;
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


void CTomogram::CalculateMIP(frame_t &img, image_index_t idx, mip_index_t mip)
{
	auto	mip_function = [&mip](const RealFunctionF32	 &row)->double
	{
		switch (mip.mip_method)
		{
		case e_mip_average:
			return AverageValue(row);
		case e_mip_maxvalue:
			return MaxValue(row);
		case e_mip_minvalue:
			return MinValue(row);
		default:
		case e_mip_no_mip:
			return row[row.size() / 2];
		}
	};

	// используется двухступенчатая процедура извлечения подмножества, поэтому два разных буфера. неуклюже, можно подумать об улучшении
	RealFunctionMD_F32	acquisition_buffer;
	RealFunctionMD_F32	b1;

	auto	frame_sizes = SliceSizes(idx.image_type);

	img.realloc(frame_sizes.y(), frame_sizes.x(), 0);

	size_t	p0 = range(idx.image_no - mip.mip_half_size, 0, CTSlicesSize(idx.image_type) - 1);
	size_t	p1 = range(idx.image_no + mip.mip_half_size, 0, CTSlicesSize(idx.image_type) - 1);


	switch (idx.image_type)
	{
	case e_ct_axial:
		b1.UseDataFragment(m_CTslices, { p0, 0, 0 }, { p1, m_CTslices.sizes(1), m_CTslices.sizes(2) });
		b1.GetSubset(acquisition_buffer, { slice_mask(0), slice_mask(1), slice_mask(2) });
		break;

	case e_ct_frontal:
		b1.UseDataFragment(m_CTslices, { 0, p0, 0 }, { m_CTslices.sizes(0), p1, m_CTslices.sizes(2) });
		b1.GetSubset(acquisition_buffer, { slice_mask(1), slice_mask(0), slice_mask(2) });
		break;

	case e_ct_sagittal:
		b1.UseDataFragment(m_CTslices, { 0, 0, p0 }, { m_CTslices.sizes(0), m_CTslices.sizes(1), p1 });
		b1.GetSubset(acquisition_buffer, { slice_mask(1), slice_mask(2), slice_mask(0) });
		break;
	}


	for (size_t i = 0; i < img.vsize(); ++i)
	{
		for (size_t j = 0; j < img.hsize(); ++j)
		{
			img.at(i, j) = mip_function(acquisition_buffer.GetRow({ slice_mask(0),i, j }));
		}
	}
}

//auto	get_iv(const )

void CTomogram::GetTomogramSlice(frame_t &img, image_index_t idx, mip_index_t mip)
{
	//size_t	slice_no = size_t(tomogram_slice_position);
	//TODO заменить на пересчет из z?

	if (mip.mip_half_size)
	{
		CalculateMIP(img, idx, mip);
	}
	else
	{
		this->GetImage(img, idx);
	}
}


point2_ST	CTomogram::SliceSizes(image_t st) const
{
	switch (st)
	{
	case	e_ct_axial:
		return	point2_ST(CTSlices().sizes(1), CTSlices().sizes(2));
	case	e_ct_frontal:
		return	point2_ST(CTSlices().sizes(0), CTSlices().sizes(2));
	case	e_ct_sagittal:
		return	point2_ST(CTSlices().sizes(0), CTSlices().sizes(1));
	default:
		throw std::invalid_argument("unknown slice type");
	}
}

size_t	CTomogram::CTSlicesSize(image_t st) const
{
	switch (st)
	{
	case	e_ct_axial:
		return	CTSlices().sizes(0);
	case	e_ct_frontal:
		return	CTSlices().sizes(1);
	case	e_ct_sagittal:
		return	CTSlices().sizes(2);
	default:
		throw std::invalid_argument("unknown slice type");
	}
}