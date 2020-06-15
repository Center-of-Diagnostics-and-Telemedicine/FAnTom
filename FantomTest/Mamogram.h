#ifndef Mamogram_h__
#define Mamogram_h__

#include <iostream>

//#include <XRADBasic/MathFunctionTypesMD.h>
//#include <XRADDicom/XRADDicom.h>
#include "SliceManager.h"

#include "XRADDicom\Sources\DicomClasses\ProcessContainers\XRAYAcquisition.h"

XRAD_USING

class Mamogram : protected SliceManager
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

	virtual void GetBrightness(double *value, image_index_t idx, size_t y, size_t x);

	virtual void CreateQByteArrayPngFromChar(QByteArray &png, const unsigned char *img, int length, const wstring &format) { return; }

	XRAYAcquisition& MMAcquisition_ptr() { return dynamic_cast<XRAYAcquisition&>(*m_proc_acquisition_ptr); }

	map<std::wstring, RealFunction2D_F32> &MMSlices() { return m_MM_slices; }

private:

	map<std::wstring, RealFunction2D_F32> m_MM_slices;
};



#endif // Mamogram_h__

