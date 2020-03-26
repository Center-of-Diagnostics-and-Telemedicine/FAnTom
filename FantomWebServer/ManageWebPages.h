﻿#ifndef ManageWebPages_h__
#define ManageWebPages_h__


#include "QtWebApp/httpserver/httpconnectionhandler.h"
#include "QtWebApp/httpserver/httplistener.h"



XRAD_BEGIN

using namespace std;

void GenerateDICOMPage(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void GenerateLoginPage(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void GenerateStartPage(std::wstringstream &message);
void GenerateOriginalPixelCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void GenerateInterpolatedPixelData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void GeneratePixelLengthData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void GenerateNFramesRealData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void GenerateNFramesInterpolatedData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void GenerateStudyAccessionNumberData(std::wstringstream &message);
void GenerateHUValueData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
//void CloseTomogram(QMultiMap<QByteArray, QByteArray> &q_params_map);
void GenerateNativeCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void GenerateInterpolatedCoordData(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void CheckDoctorLogin(QMultiMap<QByteArray, QByteArray> &q_params_map, std::wstringstream &message);
void GetAccNamesData(std::wstringstream &message);


wstring ReadDocument(wstring file_name_no_slash);


XRAD_END

#endif // ManageWebPages_h__
