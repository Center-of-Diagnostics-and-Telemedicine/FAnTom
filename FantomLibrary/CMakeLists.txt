cmake_minimum_required(VERSION 3.7)

project("FantomLibrary")

#if(NOT DEFINED ENV{XRADRoot})
#	message(FATAL_ERROR "The XRADRoot environment variable is not set.")
#endif()
set(XRADRoot $ENV{XRADRoot} CACHE PATH "XRAD library root directory.")
if("${XRADRoot}" STREQUAL "")
	message(FATAL_ERROR "The XRAD root directory must be set.")
endif()

set(XRADThirdParty $ENV{XRADThirdParty} CACHE PATH "XRAD third party directory.")
if("${XRADThirdParty}" STREQUAL "")
	message(FATAL_ERROR "The XRAD third party directory must be set.")
endif()

if ((CACHE{XRADBasic_Libraries} OR (DEFINED XRADBasic_Libraries)) AND ("${XRADBasic_Libraries}" STREQUAL ""))
	unset(XRADBasic_Libraries CACHE)
endif ()
if ((CACHE{XRADSystem_Libraries} OR (DEFINED XRADSystem_Libraries)) AND ("${XRADSystem_Libraries}" STREQUAL ""))
	unset(XRADSystem_Libraries CACHE)
endif ()
if ((CACHE{XRADConsoleUI_Libraries} OR (DEFINED XRADConsoleUI_Libraries)) AND ("${XRADConsoleUI_Libraries}" STREQUAL ""))
	unset(XRADConsoleUI_Libraries CACHE)
endif ()
if ((CACHE{XRADDicom_Libraries} OR (DEFINED XRADDicom_Libraries)) AND ("${XRADDicom_Libraries}" STREQUAL ""))
	unset(XRADDicom_Libraries CACHE)
endif ()
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(XRAD_IncludeDir "${XRADRoot}")
	set(XRAD_BuildDir "${XRADRoot}" CACHE PATH "Path to XRAD build directory.")
	set(XRADBasic_Libraries "${XRAD_BuildDir}/XRADBasic/out/build/x64-Debug/XRADBasicD.lib" CACHE FILEPATH "The XRADBasicD.lib file location.")
	set(XRADSystem_Libraries "${XRAD_BuildDir}/XRADSystem/out/build/x64-Debug/XRADSystemD.lib" CACHE FILEPATH "The XRADSystemD.lib file location.")
	set(XRADConsoleUI_Libraries "${XRAD_BuildDir}/XRADConsoleUI/out/build/x64-Debug/XRADConsoleUID.lib" CACHE FILEPATH "The XRADConsoleUID.lib file location.")
	set(XRADDicom_Libraries "${XRAD_BuildDir}/XRADDicom/out/build/x64-Debug/XRADDicomD.lib" CACHE FILEPATH "The XRADDicomD.lib file location.")

	if ((CACHE{XRADDCMTK_LibrariesDir} OR (DEFINED XRADDCMTK_LibrariesDir)) AND ("${XRADDCMTK_LibrariesDir}" STREQUAL ""))
		unset(XRADDCMTK_LibrariesDir CACHE)
	endif ()
	if ((CACHE{XRADOpenJPEG_LibrariesDir} OR (DEFINED XRADOpenJPEG_LibrariesDir)) AND ("${XRADOpenJPEG_LibrariesDir}" STREQUAL ""))
		unset(XRADOpenJPEG_LibrariesDir CACHE)
	endif ()

	set(XRADDCMTK_LibrariesDir "${XRADThirdParty}/dcmtk/install/x64/Debug/lib" CACHE PATH "The DCMTK library directory.")
	set(XRADDCMTK_Libraries
		"${XRADDCMTK_LibrariesDir}/dcmdata.lib"
		"${XRADDCMTK_LibrariesDir}/ofstd.lib"
		"${XRADDCMTK_LibrariesDir}/oflog.lib"
		"${XRADDCMTK_LibrariesDir}/dcmimgle.lib"
		"${XRADDCMTK_LibrariesDir}/dcmjpls.lib"
		"${XRADDCMTK_LibrariesDir}/dcmjpeg.lib"
		"${XRADDCMTK_LibrariesDir}/dcmnet.lib"
		)

	set(XRADOpenJPEG_LibrariesDir "${XRADThirdParty}/openjpeg/install/x64/Debug/lib" CACHE PATH "The OpenJPEG library directory.")
	set(XRADOpenJPEG_Libraries
		"${XRADOpenJPEG_LibrariesDir}/openjp2.lib"
		)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(XRAD_IncludeDir "${XRADRoot}")
	set(XRAD_BuildDir "${XRADRoot}/build" CACHE PATH "Path to XRAD build root directory.")
	set(XRADBasic_Libraries "${XRAD_BuildDir}/XRADBasic/libXRADBasic.a" CACHE FILEPATH "The libXRADBasic.a file location.")
	set(XRADSystem_Libraries "${XRAD_BuildDir}/XRADSystem/libXRADSystem.a" CACHE FILEPATH "The libXRADSystem.a file location.")
	set(XRADConsoleUI_Libraries "${XRAD_BuildDir}/XRADConsoleUI/libXRADConsoleUI.a" CACHE FILEPATH "The libXRADConsoleUI.a file location.")
	set(XRADDicom_Libraries "${XRAD_BuildDir}/XRADDicom/libXRADDicom.a" CACHE FILEPATH "The libXRADDicom.a file location.")

	set(XRADDCMTK_LibrariesDir "${XRADThirdParty}/lib")
	set(XRADDCMTK_Libraries
		"${XRADDCMTK_LibrariesDir}/libdcmnet.a"
		"${XRADDCMTK_LibrariesDir}/libdcmimgle.a"
		"${XRADDCMTK_LibrariesDir}/libdcmdata.a"
		"${XRADDCMTK_LibrariesDir}/libdcmjpeg.a"
		"${XRADDCMTK_LibrariesDir}/libdcmjpls.a"
		"${XRADDCMTK_LibrariesDir}/libcharls.a"
		"${XRADDCMTK_LibrariesDir}/liboflog.a"
		"${XRADDCMTK_LibrariesDir}/libofstd.a"
		"${XRADDCMTK_LibrariesDir}/libijg16.a"
		"${XRADDCMTK_LibrariesDir}/libijg12.a"
		"${XRADDCMTK_LibrariesDir}/libijg8.a"
		)

	set(XRADOpenJPEG_LibrariesDir "${XRADThirdParty}/lib")
	set(XRADOpenJPEG_Libraries
		"${XRADOpenJPEG_LibrariesDir}/libopenjp2.a"
		)

	set(XRADZLib_LibrariesDir "${XRADThirdParty}/lib")
	set(XRADZLib_Libraries
		"${XRADZLib_LibrariesDir}/libz.a"
		)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")
else()
	message(FATAL_ERROR "Unsupported CMAKE_CXX_COMPILER_ID: \"${CMAKE_CXX_COMPILER_ID}\".")
endif()

include_directories(${XRAD_IncludeDir})

include_directories(. internal)

# sources/internal-FantomLibrary/pre.cpp
set(FantomLibrary_Sources_cpp
	FantomClass.cpp
	FantomLibrary.cpp
)
# internal/pre.cpp

set(FantomLibrary_Sources_h
	internal/pre.h
	FantomClass.h
	FantomDefs.h
	FantomLibrary.h
)
# MSVC_FantomLibraryLink.h

set(FantomLibrary_Sources_All
	${FantomLibrary_Sources_cpp}
	${FantomLibrary_Sources_h}
)

add_library(FantomLibrary SHARED ${FantomLibrary_Sources_All})
target_link_libraries(FantomLibrary
	${XRADDicom_Libraries}
	${XRADDCMTK_Libraries}
	${XRADOpenJPEG_Libraries}
	${XRADZLib_Libraries}
	${XRADConsoleUI_Libraries}
	${XRADSystem_Libraries}
	${XRADBasic_Libraries}
)
