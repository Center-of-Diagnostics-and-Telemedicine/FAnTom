#ifndef __pre_h_precompiled
#define __pre_h_precompiled


#ifdef __pre_h
#error File "pre.h" must be included once!!!
#endif
#define __pre_h
#define __pre_h_first
#pragma warning (disable:4018)
#pragma warning (disable:4482)
#pragma warning (disable:4297)

#include <XRAD.h>
#include <XRADBasic/Sources/Utils/TimeProfiler.h>
#include <MathFunctionGUI2D.h>
#include <MathFunctionGUIMD.h>
#include <XRADBasic/DataArrayIO.h>
#include <XRADSystem/CFile.h>
#include <XRADBasic/ContainersAlgebra.h>
//#include <XRAD/FIRFilterKernel2D.h>
//#include <DataArrayMD.h>
#include <XRADSystem/Sources/IniFile/XRADIniFile.h>
#include <XRADBasic/Sources/Utils/ExponentialBlurAlgorithms.h>

#include <XRADDicom/XRADDicom.h>
#include <XRADDicomGUI/XRADDicomGUI.h>

#include <iostream>

#undef	__pre_h_first

#endif //__pre_h_precompiled
