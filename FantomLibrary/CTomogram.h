#ifndef CTomogram_h__
#define CTomogram_h__

#include <iostream>


//#include <XRADBasic/MathFunctionTypesMD.h>
//#include <XRADDicom/XRADDicom.h>
#include "SliceManager.h"
//#include "SliceDefs.h"

//#include "XRADDicom\Sources\DicomClasses\ProcessContainers\XRAYAcquisition.h"

XRAD_USING


class CTomogram : protected SliceManager
{
	PARENT(SliceManager);
public:
	using parent::InitHeap;
	using parent::HeapDump;
	using parent::GetAccessionHeapPosition;
	using parent::GetLargestAcquisition;
	using parent::GetAccNumber;
//	using parent::CreateQByteArrayPngFromChar;

	virtual operation_result LoadByAccession(const wstring accession_number);

	virtual operation_result GetImage(frame_t &img, const image_index_t idx);

	virtual operation_result GetScreenImage(const unsigned char **img, int *length, image_index_t idx, double black, double white, double gamma, mip_index_t mip);
	
	virtual operation_result GetBrightness(double *value, image_index_t idx, size_t y, size_t x);

//	virtual void CreateQByteArrayPngFromChar(QByteArray &png, const unsigned char *img, int length, const wstring &format);

	CTAcquisition& CTAcquisition_ptr() { return dynamic_cast<CTAcquisition&>(*m_proc_acquisition_ptr); }

	void CalculateInterpolationScales();

	void RescaleImageFromTomogramToScreenCoordinates(frame_t &rescaled_image, const frame_t &tomogram_slice, const wstring slice_type);

	double DicomToScreenCoordinate(double t, axis_t axis);

	double ScreenToDicomCoordinate(double t, axis_t axis);

	point2_ST	SliceSizes(const wstring st) const;

	size_t	CTSlicesSize(const wstring st) const;

	void GetTomogramSlice(frame_t &img, image_index_t idx, mip_index_t mip);

	void CalculateMIP(frame_t &img, image_index_t idx, mip_index_t mip);

	const RealFunctionMD_F32	&CTSlices() const { return m_CTslices; }

private:
	map < wstring , BitmapContainerIndexed>	m_bmp;
//	BitmapContainerIndexed	m_bmp;
//	unique_ptr<unsigned char[]> bitmap_buffer;
	VectorFunction3_F64 m_image_positions_patient;
	bool	m_flip_z;
	point3_F64 m_CTscales;
	point3_F64	m_interpolation_factor;
	point3_ST		  m_interpolation_sizes;
	RealFunctionMD_F32 m_CTslices;
};



#endif // CTomogram_h__

