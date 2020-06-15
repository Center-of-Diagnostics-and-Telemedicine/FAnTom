﻿#ifndef XRay_h__
#define XRay_h__

#include <iostream>

//#include <XRADBasic/MathFunctionTypesMD.h>
//#include <XRADDicom/XRADDicom.h>
#include "SliceManager.h"

#include "XRADDicom\Sources\DicomClasses\ProcessContainers\XRAYAcquisition.h"

XRAD_USING

class XRay : protected SliceManager
{
	PARENT(SliceManager);
public:
	using parent::InitHeap;
	using parent::HeapDump;
	using parent::GetAccessionHeapPosition;
	using parent::GetLargestAcquisition;
	using parent::GetAccNumber;
	using parent::GetInstancesOfStudy;

	virtual int LoadByAccession(const wstring accession_number);

	virtual void GetImage(frame_t &img, image_index_t idx);

	virtual void GetScreenImage(const unsigned char **img, int *length, image_index_t idx, double black, double white, double gamma, mip_index_t mip) { return; }

	virtual void CreateQByteArrayPngFromChar(QByteArray &png, const unsigned char *img, int length, const wstring &format) { return; }

	virtual void GetBrightness(double *value, image_index_t idx, size_t y, size_t x);

	XRAYAcquisition& XrayAcquisition_ptr() { return dynamic_cast<XRAYAcquisition&>(*m_proc_acquisition_ptr); }

	const vector<RealFunction2D_F32>	&XRSlices() const { return m_XR_slices; }

private:
	vector<RealFunction2D_F32> m_XR_slices;
};



#endif // XRay_h__

