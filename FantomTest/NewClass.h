#ifndef NewClass_h__
#define NewClass_h__

#include <iostream>


#include <XRADBasic/MathFunctionTypesMD.h>
#include <XRADDicom/XRADDicom.h>
#include "NewClassDefs.h"

//#include "XRADDicom\Sources\DicomClasses\ProcessContainers\XRAYAcquisition.h"

XRAD_USING

Dicom::acquisition_loader &GetLargestAcquisition(Dicom::study_loader &study);

Dicom::acquisition_loader CreateXRayAcqusition(Dicom::study_loader &study);


class SliceManager
{

protected:

	vector<Dicom::study_loader> m_studies_heap;
	shared_ptr<ProcessAcquisition> proc_acquisition_work_ptr;


	wstring m_patient_id;
	wstring m_patient_sex;
	wstring m_patient_age;

	wstring m_accession_number;
	wstring m_study_id;
	wstring m_study_instance_uid;

	vector<wstring> m_accession_numbers;

	void InitHeap(const wstring& dicom_folder);
	void HeapDump(const wstring& dump_file);

private:

	virtual void ProcessAcqusionByAccessionNumber(const wstring accession_number) = 0;

	virtual void GetImage(const unsigned char **img, image_index_t idx) = 0;

	virtual double GetBrightness(image_index_t idx, size_t x, size_t y) = 0;

};


class CTomogram : protected SliceManager
{
	PARENT(SliceManager);
public:
	using parent::InitHeap;
	using parent::HeapDump;

	virtual void ProcessAcqusionByAccessionNumber(const wstring accession_number);

	virtual void GetImage(const unsigned char **img, image_index_t idx) {}

	virtual double GetBrightness(image_index_t idx, size_t x, size_t y) { std::wcout << "GetBrightness" << std::endl; return 0.; }

private:
	point3_F64		  m_interpolation_factor;
	RealFunctionMD_F32 m_CTslices;
};


class XRay : protected SliceManager
{
	PARENT(SliceManager);
public:

	virtual void ProcessAcqusionByAccessionNumber(char* accession_number);

	virtual void GetImage(const unsigned char **img, image_index_t idx);

	virtual double GetBrightness(image_index_t idx, size_t x, size_t y);

private:
	vector<RealFunction2D_F32> m_XRslices;
};



#endif // NewClass_h__
