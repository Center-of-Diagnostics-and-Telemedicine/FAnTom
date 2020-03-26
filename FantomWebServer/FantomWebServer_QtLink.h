// file FantomWebServer_QtLink.h
//--------------------------------------------------------------
#ifndef __FantomWebServer_QtLink_h
#define __FantomWebServer_QtLink_h
//--------------------------------------------------------------

#ifndef _MSC_VER
	#error Этот файл может использоваться только с MS VisualStudio
#endif //_MSC_VER

#if _MSC_VER == 1900 // MSVC2015, Qt5

#ifdef _DEBUG
	#define XRAD_Fantom_QtLibSuffix "d"
#else
	#define XRAD_Fantom_QtLibSuffix ""
#endif

#pragma comment(lib, "Qt5Core" XRAD_Fantom_QtLibSuffix ".lib")
#pragma comment(lib, "Qt5Network" XRAD_Fantom_QtLibSuffix ".lib")
#pragma comment(lib, "Qt5Gui" XRAD_Fantom_QtLibSuffix ".lib")

#undef XRAD_Fantom_QtLibSuffix

#else
	#error Unsupported Visual Studio version
#endif

//--------------------------------------------------------------
#endif // __FantomWebServer_QtLink_h
