#include "pre.h"
#include <DicomClasses/DicomFileInfo.h>
#include "LoadTomogram.H"
//#include "RASP3CTLibrarySources/RASP3CTParams.h"
//#include "RASP3CTParamsUtils.h"

XRAD_BEGIN



////////////////////////////////////////////////////////////////////////
//Запись обработанных файлов DICOM в отдельный фолдер
////////////////////////////////////////////////////////////////////////

void WriteDICOMData(RealFunctionMD_I16 &data, dicom_params_list &chosen_series, string &s_preset_description)
{
	wstring ws_preset_description = string_to_wstring(s_preset_description, e_decode_literals);
	WriteDICOMData(data, chosen_series, ws_preset_description);
}


void WriteDICOMData(RealFunctionMD_I16 &data, dicom_params_list &chosen_series, wstring &ws_preset_description)
{
	index_vector	access_v = {0,0,0};
	auto it_file = chosen_series.begin();
	dicom::opttype opt=dicom::OPT_SAVE_WITHOUT_METAINFO;
	#ifdef RASP3_DIAG
	#endif
	string patiens_folder_s((*it_file)->study_date);
	patiens_folder_s += " ";
	patiens_folder_s += (*it_file)->series_description;
	wstring patiens_folder = string_to_wstring(patiens_folder_s, e_decode_literals);
	
	wstring ini_filename = WGetAplicationDirectory();
	wstring main_folder_name = L"c:\\Procecced_DICOM";
	
	
	wstring processed_dicom_folder = L"Processed_DICOM";
	CreateFolder(main_folder_name, processed_dicom_folder);
	wstring full_folder_path(main_folder_name);
	full_folder_path += L"\\";
	full_folder_path += processed_dicom_folder;
	
	
	CreateFolder(full_folder_path, patiens_folder);
	full_folder_path +=L"\\";
	full_folder_path += patiens_folder;
	CreateFolder(full_folder_path, ws_preset_description);
	wstring file_destination_path(full_folder_path);
	file_destination_path += L"\\";
	file_destination_path += ws_preset_description;
	
	StartProgress("Please wait for images to be written", data.sizes(0));
	for (size_t i = 0; i < data.sizes(0); ++i, ++it_file)
	{
		xray_ct_dicom_frame_params *params = dynamic_cast<xray_ct_dicom_frame_params*>(it_file->get());
		access_v = {i, slice_mask(0),slice_mask(1)};
		RealFunction2D_I16 data_slice;
		data.GetSlice(data_slice, access_v);
		int rowstep = (params->horizontal_size)*sizeof(int16_t);
		int framestep =  (params->horizontal_size)*(params->vertical_size)*sizeof(int16_t);
		
		RealFunction2D_I16 buffer(data_slice);		
		char* ptr = reinterpret_cast<char*>(&buffer.at(0,0));
		dicom::dicomfile *df0;
		df0 = dicom::open_dicomfile(params->fullpathname.c_str(), opt);
		df0->set_pixeldata(df0->tsuid, ptr, params->horizontal_size, params->vertical_size, params->precision,
							params->signedness, params->ncomponents, params->nframes, rowstep, framestep);
		wstring filename_mod(file_destination_path);
		filename_mod += L"\\";
		filename_mod += (params->filename);
		
		//////////////////////////////////////////////////////////////////////////
		//	Изменение имени серии после обработки
		//////////////////////////////////////////////////////////////////////////

		dicom::dataelement *ser_desc;
		string new_series_description = params->series_description + " Modified by RASP_3D";
		ser_desc = df0->get_dataelement(0x0008103E);
		ser_desc->from_string(new_series_description.c_str());
//		AnonimyzeDicomData(df0);
		df0->save_to_file(filename_mod.c_str());
		close_dicomfile(df0);
		NextProgress();
	}
	EndProgress();
}


XRAD_END