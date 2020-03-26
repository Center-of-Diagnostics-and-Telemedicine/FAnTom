#ifndef __LoadTomogram_h
#define __LoadTomogram_h
#include "DicomClasses/DicomFileInfo.h"

XRAD_BEGIN



void				WriteDICOMData(RealFunctionMD_I16 &data, dicom_params_list &chosen_series, string &s_preset_description);
void				WriteDICOMData(RealFunctionMD_I16 &data, dicom_params_list &chosen_series, wstring &ws_preset_description);

XRAD_END

#endif