#include "pre.h"
#include <sstream>
#include <XRADSystem/CFile.h>
#include "ManageStrings.h"
#include "ManageServerCommands.h"


using namespace std;
extern QString	web_server_path;

XRAD_BEGIN

command_type ParseCommand(QMultiMap<QByteArray,QByteArray> &q_params_map)
{
	if (!(q_params_map.contains("command")) ) return e_no_command;
	
//	if (!(q_params_map.contains("command")) && (q_params_map.size() == 0)) return e_load_start_page;
//	else if (!(q_params_map.contains("command")) && (q_params_map.size() == 2)) return e_load_web_page;

	else if ((q_params_map.value("command", "") == "GetSlice") && q_params_map.size() == 9) return e_get_one_slice;
	else if ((q_params_map.value("command", "") == "GetNFramesReal") && (q_params_map.size() == 3)) return e_get_n_frames_real;
	else if ((q_params_map.value("command", "") == "GetNFramesInterpolated") && (q_params_map.size() == 3)) return e_get_n_frames_interpolated;
	else if ((q_params_map.value("command", "") == "GetCoordinateNative") && (q_params_map.size() == 4)) return e_get_coordinate_native;
	else if ((q_params_map.value("command", "") == "GetCoordinateInterpolated") && (q_params_map.size() == 4)) return e_get_coordinate_interpolated;
	else if ((q_params_map.value("command", "") == "GetOriginalPixelCoordinate") && (q_params_map.size() == 5)) return e_get_original_coordinate;
	else if ((q_params_map.value("command", "") == "GetPixelLength") && (q_params_map.size() == 2)) return e_get_pixel_length;
	else if ((q_params_map.value("command", "") == "GetInterpolatedPixelCoordinate") && (q_params_map.size() == 4)) return e_get_pixel_interpolated;
	else if ((q_params_map.value("command", "") == "GetPointHU") && (q_params_map.size() == 5)) return e_get_point_HU;
	else if ((q_params_map.value("command", "") == "CloseTomogram") && (q_params_map.size() == 2)) return e_delete_ct;
	else if ((q_params_map.value("command", "") == "GetStudyAccessionNumber") && (q_params_map.size() == 1)) return e_get_study_accession;
	else if ((q_params_map.value("command", "") == "CheckDoctorIDAndPassword") && (q_params_map.size() == 3)) return e_check_doctor_login;
	else if ((q_params_map.value("command", "") == "GetAccessionNames") && (q_params_map.size() == 1)) return e_get_accession_numbers;
	else return e_no_command;
}

XRAD_END