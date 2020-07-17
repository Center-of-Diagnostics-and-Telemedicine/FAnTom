#ifndef SliceManager_h__
#define SliceManager_h__

#include <iostream>

/*
#include <QtGui/QImage>
#include <QtCore/QBuffer>
#include <QtCore/QFile>
*/

#include <XRADBasic/MathFunctionTypesMD.h>
#include <XRADDicom/XRADDicom.h>
#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <XRADBasic/ThirdParty/nlohmann/json.hpp>

#include "SliceDefs.h"
#include "FantomDefs.h"
//#include "XRADDicom\Sources\DicomClasses\ProcessContainers\XRAYAcquisition.h"

XRAD_USING

class SliceManager
{
public:
	operation_result InitHeap(const wstring& dicom_folder);
	operation_result HeapDump(const string& dump_file);
//	operation_result GetAccNumber(size_t no, wstring &acc_no) { if (no < m_accession_numbers.size()) { acc_no = m_accession_numbers[no]; return e_successful; } else return e_out_of_range; }

	Dicom::study_loader & GetStudy() { return m_Study; }
//	vector<wstring>& GetAccNumVector() { return m_accession_numbers; }

public:

//	size_t GetAccessionHeapPosition(const wstring &accession_number, bool &acc_found);

	Dicom::acquisition_loader& GetLargestAcquisition();
	Dicom::acquisition_loader GetInstancesOfStudy();

protected:
	shared_ptr<ProcessAcquisition> m_proc_acquisition_ptr;

	wstring m_patient_id;
	wstring m_patient_sex;
	wstring m_patient_age;

	wstring m_accession_number;
	wstring m_study_id;
	wstring m_study_instance_uid;

private:

	 Dicom::study_loader  m_Study;


public:

	virtual operation_result LoadByAccession() = 0;

	virtual operation_result GetModality(string &modality) = 0;

	virtual operation_result GetImage(frame_t &img, const image_index_t idx) = 0;

	virtual operation_result GetScreenImage(const unsigned char **img, int *length, image_index_t idx, brightness brightness) = 0;
	
	virtual operation_result GetBrightness(double *value, image_index_t idx, int y, int x) = 0;

	virtual operation_result GetDimensions(nlohmann::json &j) = 0;

	virtual operation_result GetDictionary(nlohmann::json &j) = 0;

	virtual operation_result GetZFlip(bool &flip) = 0;

//	virtual operation_result GetScreenDimensions(point3_ST &v) = 0;
//	virtual operation_result GetTomogramDimensions(point3_ST &v) = 0;

public:
	SliceManager(Dicom::study_loader  &&in_Study) : m_Study(in_Study) {};


};



#endif // SliceManager_h__
