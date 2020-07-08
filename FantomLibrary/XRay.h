#ifndef XRay_h__
#define XRay_h__

#include <iostream>

//#include <XRADBasic/MathFunctionTypesMD.h>
//#include <XRADDicom/XRADDicom.h>
#include "SliceManager.h"

#include "XRADDicom\Sources\DicomClasses\ProcessContainers\XRAYAcquisition.h"

XRAD_USING

class XRay : public SliceManager
{
	PARENT(SliceManager);
public:
	using parent::InitHeap;
	using parent::HeapDump;
	using parent::GetLargestAcquisition;
	using parent::GetInstancesOfStudy;
	using parent::parent;

	virtual operation_result LoadByAccession() override;

	virtual operation_result GetModality(string &modality) override;

	virtual operation_result GetImage(frame_t &img, const image_index_t idx) override;

	virtual operation_result GetScreenImage(const unsigned char **img, int *length, image_index_t idx, brightness brightness) override;

	virtual operation_result GetBrightness(double *value, image_index_t idx, size_t y, size_t x) override;

	virtual operation_result GetDimensions(nlohmann::json &j) override { return e_successful; };

	virtual operation_result GetScreenDimensions(point3_ST &v) override { return e_successful; };

	virtual operation_result GetTomogramDimensions(point3_ST &v) override { return e_successful; };

	virtual operation_result GetZFlip(bool &flip)
	{
		flip = false;
		return e_successful;
	}


private:

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

