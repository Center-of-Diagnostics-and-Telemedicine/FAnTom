#include "pre.h"

#include "NewClass.h"

#include <XRADBasic/Sources/Utils/BitmapContainer.h>
#include <XRADBasic/Sources/Utils/ConsoleProgress.h>
#include <XRADBasic/ThirdParty/nlohmann/json.hpp>


#include <iostream>
#include <fstream>


void SliceManager::HeapDump(const wstring& dump_file)
{
	std::ofstream File;

	File.open(dump_file.c_str());
		size_t i = 0;

		for (auto &study : m_studies_heap)
		{
			File << i << endl;
			i++;

			File << "The Study ID: ";
			File << convert_to_string8( study.complete_study_id().study_id() ) << endl;

			File << "The Accession number: ";
			File << convert_to_string8( study.complete_study_id().accession_number() ) << endl;

			for (auto &ser : study)
			{
				for (auto &stack : ser)
				{
					for (auto &acq : stack)
					{
						for (auto inst : acq)
						{
							File << "Full instance path: " << endl;
							File << convert_to_string8(	inst->dicom_container()->last_used_instancestorage()->print() )  << endl;
						}
					}
				}
			}
			File << endl;
		}
		File.close();
}