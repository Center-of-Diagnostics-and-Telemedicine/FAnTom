#ifndef Mamogram_h__
#define Mamogram_h__

#include <iostream>

//#include <XRADBasic/MathFunctionTypesMD.h>
//#include <XRADDicom/XRADDicom.h>
#include "SliceManager.h"

#include "XRADDicom\Sources\DicomClasses\ProcessContainers\XRAYAcquisition.h"

XRAD_USING

class Mammogram : public SliceManager
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

	virtual operation_result GetBrightness(double *value, image_index_t idx, int y, int x) override;

	virtual operation_result GetDimensions(nlohmann::json &j) override;

//	virtual operation_result GetScreenDimensions(point3_ST &v) override { return e_successful; };

//	virtual operation_result GetTomogramDimensions(point3_ST &v) override { return e_successful; };

	virtual operation_result GetZFlip(bool &flip)
	{
		flip = false;
		return e_successful;
	}

private:
	int AddToStepsMap(const string image_type, vector<wstring> var1, vector <wstring> var2);

	void RescaleImageToScreenCoordinates(frame_t &img_screen, const frame_t &buffer, image_index_t idx);

	XRAYAcquisition& MMAcquisition_ptr() { return dynamic_cast<XRAYAcquisition&>(*m_proc_acquisition_ptr); }

	map<string, RealFunction2D_F32> &m_MM_Images() { return m_MM_images; }


private:

	map<string, RealFunction2D_F32> m_MM_images;

	map<string, bool> m_EqualSteps;

	map<string, point2_F32> m_Steps;

	map<string, point2_ST> m_ScreenSize;

	map <string, BitmapContainerIndexed>	m_bmp;
//public:
//public:
//	Mammogram::Mammogram();

};



#endif // Mamogram_h__

