#ifndef ManageWebPages_h__
#define ManageWebPages_h__


#include "QtWebApp/httpserver/httpconnectionhandler.h"
#include "QtWebApp/httpserver/httplistener.h"



XRAD_BEGIN

using namespace std;

void LoadCTbyAccession(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

void GenerateLoginPage(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

void GenerateStartPage(std::wstringstream &message);

void GetTomogramLocationFromScreenCoordinate(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

void GenerateInterpolatedPixelData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

void GeneratePixelLengthData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

void GetTomogramDimension(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

void GetScreenDimension(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

void GenerateHUValueData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

//void CloseTomogram(QMultiMap<QByteArray, QByteArray> &q_params_map);

//void GenerateNativeCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

//void GenerateInterpolatedCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);

void GetNumbersOfAccessions(std::wstringstream &message);


wstring ReadDocument(wstring file_name_no_slash);


XRAD_END

#endif // ManageWebPages_h__
