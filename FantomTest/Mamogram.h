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
	using parent::CreateQByteArrayPngFromChar;

	virtual int LoadByAccession(const wstring accession_number);

	virtual void GetImage(frame_t &img, image_index_t idx);

	virtual void GetScreenImage(const unsigned char **img, int *length, image_index_t idx, double black, double white, double gamma, mip_index_t mip);

	virtual void GetBrightness(double *value, image_index_t idx, size_t y, size_t x);

	int AddToStepsMap(const wstring image_type, vector<wstring> var1, vector <wstring> var2);

	void RescaleImageToScreenCoordinates(frame_t &img_screen, const frame_t &buffer, image_index_t idx);

	XRAYAcquisition& MMAcquisition_ptr() { return dynamic_cast<XRAYAcquisition&>(*m_proc_acquisition_ptr); }

	map<std::wstring, RealFunction2D_F32> &m_MM_Images() { return m_MM_images; }

private:

	map<std::wstring, RealFunction2D_F32> m_MM_images;

	map<std::wstring, bool> m_EqualSteps;

	map<wstring, pair<double, double>> m_Steps;

	pair<size_t, size_t> m_ScreenSize;
};



#endif // Mamogram_h__

