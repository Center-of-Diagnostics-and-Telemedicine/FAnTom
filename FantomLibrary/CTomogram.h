#ifndef CTomogram_h__
#define CTomogram_h__

#include <iostream>


//#include <XRADBasic/MathFunctionTypesMD.h>
//#include <XRADDicom/XRADDicom.h>
#include "SliceManager.h"
//#include "SliceDefs.h"

//#include "XRADDicom\Sources\DicomClasses\ProcessContainers\XRAYAcquisition.h"

XRAD_USING


class CTomogram : public SliceManager
{
public:
	PARENT(SliceManager);
	using parent::InitHeap;
	using parent::HeapDump;
	using parent::GetAccessionHeapPosition;
	using parent::GetLargestAcquisition;
	using parent::GetAccNumber;


	virtual operation_result LoadByAccession(const wstring accession_number);

	virtual operation_result GetModality(string &modality) override;

	virtual operation_result GetImage(frame_t &img, const image_index_t idx) override;

	virtual operation_result GetScreenImage(const unsigned char **img, int *length, image_index_t idx, brightness brightness) override;
	
	virtual operation_result GetBrightness(double *value, image_index_t idx, size_t y, size_t x) override;

	virtual operation_result GetDimensions(nlohmann::json &j) override;

	virtual operation_result GetTomogramDimensions(point3_ST &v) override ;

	virtual operation_result GetScreenDimensions(point3_ST &v) override ;

	virtual operation_result GetZFlip(bool & flip) override;


private:

	CTAcquisition& CTAcquisition_ptr() { return dynamic_cast<CTAcquisition&>(*m_proc_acquisition_ptr); }

	void CalculateInterpolationScales();

	void RescaleImageFromTomogramToScreenCoordinates(frame_t &rescaled_image, const frame_t &tomogram_slice, const string slice_type);

	double DicomToScreenCoordinate(double t, axis_t axis);

	double ScreenToDicomCoordinate(double t, axis_t axis);

	point2_ST	SliceSizes(const string &st) const;

	size_t	CTSlicesSize(const string &st) const;

	void GetTomogramSlice(frame_t &img, image_index_t idx);

	void CalculateMIP(frame_t &img, image_index_t idx);

	const RealFunctionMD_F32	&CTSlices() const { return m_CTslices; }

private:
	map < string , BitmapContainerIndexed>	m_bmp;
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

