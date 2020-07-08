#include "pre.h"

#include "SliceManager.h"

#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>
#include <XRADBasic/ThirdParty/nlohmann/json.hpp>


#include <iostream>
#include <fstream>


operation_result SliceManager::HeapDump(const wstring& dump_file)
{
	std::ofstream File;

	File.open(dump_file.c_str());
		size_t  ii = 0;

//		for (auto &study : m_studies_heap)
//		{
//			File << i << endl;
//			i++;

			File << "The Study ID: ";
			File << convert_to_string8( GetStudy().complete_study_id().study_id() ) << endl;

			File << "The Accession number: ";
			File << convert_to_string8( GetStudy().complete_study_id().accession_number() ) << endl;

			ii = 0;

			for (auto &ser : GetStudy())
			{
				for (auto &stack : ser)
				{
					for (auto &acq : stack)
					{
						for (auto &inst : acq)
						{
							File << "Full instance path: " << endl;
							File << convert_to_string8(	inst->dicom_container()->last_used_instancestorage()->print() )  << endl;
							inst->dicom_container()->open_instancestorage();
							File << convert_to_string8(inst->get_wstring(Dicom::e_acquisition_device_processing_description) ) << endl;
							File << convert_to_string8(inst->get_wstring(Dicom::e_imager_pixel_spacing)) << endl;
							File << convert_to_string8(inst->get_wstring(Dicom::e_pixel_spacing)) << endl;
							inst->dicom_container()->close_instancestorage();

							ii++;
						}
					}
				}
			}
			File << "The total number of instances = " << ii  << endl;
			File << "---------------------------------------" << endl;
			File << endl;
//		}
		File.close();

		return e_successful;
}