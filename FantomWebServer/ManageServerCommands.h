#ifndef ManageServerCommands_h__
#define ManageServerCommands_h__


#include "QtWebApp/httpserver/httpconnectionhandler.h"
#include "QtWebApp/httpserver/httplistener.h"

using namespace std;

XRAD_BEGIN

enum command_type
{
	e_get_one_slice,
	e_get_tomogram_dimension, 
	e_get_screen_dimension,
	e_get_coordinate_native,
	e_get_coordinate_interpolated,
	e_get_original_coordinate,
	e_get_pixel_length,
	e_get_point_HU,
	e_get_pixel_interpolated,
	e_get_numbers_of_accessions,
	e_no_command
};

command_type ParseCommand(QMultiMap<QByteArray, QByteArray> &q_params_map);

XRAD_END

#endif // ManageServerCommands_h__

