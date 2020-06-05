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

	virtual int LoadByAccession(const wstring accession_number);

	virtual void GetImage(frame_t &img, image_index_t idx);

	virtual void GetBrightness(double &value, image_index_t idx, size_t y, size_t x);

	CTAcquisition& CTAcquisition_ptr() { return dynamic_cast<CTAcquisition&>(*m_proc_acquisition_ptr); }

	void CalculateInterpolationScales();

	const RealFunctionMD_F32	&CTSlices() const { return m_CTslices; }

private:
	VectorFunction3_F64 m_image_positions_patient;
	bool	m_flip_z;
	point3_F64 m_CTscales;
	point3_F64	m_interpolation_factor;
	point3_ST		  m_interpolation_sizes;
	RealFunctionMD_F32 m_CTslices;
};



#endif // CTomogram_h__

