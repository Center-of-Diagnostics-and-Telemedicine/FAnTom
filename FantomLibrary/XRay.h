#ifndef XRay_h__
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
//	using parent::CreateQByteArrayPngFromChar;

//	void CreateQByteArrayPngFromChar(QByteArray &png, const unsigned char *img, int length, const wstring &format);

	virtual operation_result LoadByAccession(const wstring accession_number);

	virtual operation_result GetModality(string &modality);

	virtual operation_result GetImage(frame_t &img, const image_index_t idx);

	virtual operation_result GetScreenImage(const unsigned char **img, int *length, image_index_t idx, double black, double white, double gamma, mip_index_t mip);

	virtual operation_result GetBrightness(double *value, image_index_t idx, size_t y, size_t x);

	XRAYAcquisition& XrayAcquisition_ptr() { return dynamic_cast<XRAYAcquisition&>(*m_proc_acquisition_ptr); }

	int XRay::AddToStepsVector(vector<wstring> var1, vector <wstring> var2);

	void XRay::RescaleImageToScreenCoordinates(frame_t &img_screen, const frame_t &buffer, image_index_t idx);

	const vector<RealFunction2D_F32>	&m_XR_Images() const { return m_XR_images; }

private:
	vector<RealFunction2D_F32> m_XR_images;

	vector<pair<double, double>> m_Steps;

	vector<pair<size_t, size_t>> m_ScreenSize;

	vector<bool> m_EqualSteps;

	vector<BitmapContainerIndexed>	m_bmp;
};



#endif // XRay_h__

