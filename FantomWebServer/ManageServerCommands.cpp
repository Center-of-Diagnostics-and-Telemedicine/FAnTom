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

	else if ((q_params_map.value("command", "") == "GetSlice") && q_params_map.size() == 9) return e_get_one_slice;

	else if ((q_params_map.value("command", "") == "GetTomogramDimension") && (q_params_map.size() == 3)) return e_get_tomogram_dimension;

	else if ((q_params_map.value("command", "") == "GetScreenDimension") && (q_params_map.size() == 3)) return e_get_screen_dimension;

//	else if ((q_params_map.value("command", "") == "GetCoordinateNative") && (q_params_map.size() == 4)) return e_get_coordinate_native;

//	else if ((q_params_map.value("command", "") == "GetCoordinateInterpolated") && (q_params_map.size() == 4)) return e_get_coordinate_interpolated;

	else if ((q_params_map.value("command", "") == "GetTomogramCoordinate") && (q_params_map.size() == 5)) return e_get_tomogram_coordinate;

	else if ((q_params_map.value("command", "") == "GetPixelLength") && (q_params_map.size() == 2)) return e_get_pixel_length;

	else if ((q_params_map.value("command", "") == "GetInterpolatedPixelCoordinate") && (q_params_map.size() == 4)) return e_get_pixel_interpolated;

	else if ((q_params_map.value("command", "") == "GetPointHU") && (q_params_map.size() == 5)) return e_get_point_HU;

	else if ((q_params_map.value("command", "") == "GetNumbersOfAccessions") && (q_params_map.size() == 1)) return e_get_numbers_of_accessions;

	else return e_no_command;
}

XRAD_END