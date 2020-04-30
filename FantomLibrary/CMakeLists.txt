cmake_minimum_required(VERSION 3.7)

set(Project "FantomLibrary")
project(${Project} LANGUAGES CXX)

set(XRAD_Root $ENV{XRADRoot} CACHE PATH "XRAD library sources directory.")
if("${XRAD_Root}" STREQUAL "")
	message(FATAL_ERROR "Please, specify XRAD library sources directory (XRAD_Root).")
endif()

include(${XRAD_Root}/XRADSystem/CMake/Package.cmake)
include(${XRAD_Root}/XRADDicom/CMake/Package.cmake)

set(Project_IncludeDirs ${Project_IncludeDirs}
	.
	internal
	${XRAD_IncludeDir}
	)

set(Project_Sources_All
	internal/pre.h
	FantomClass.cpp
	FantomClass.h
	FantomDefs.h
	FantomLibrary.cpp
	FantomLibrary.h
	FantomLogger.cpp
	FantomLogger.h
	)

add_library(${Project} SHARED ${Project_Sources_All})
target_include_directories(${Project} PRIVATE ${Project_IncludeDirs})
target_link_libraries(${Project}
	${XRADDicom_Libraries}
	${XRADSystem_Libraries}
	)
xrad_add_dependencies(${Project} "XRADSystem;XRADDicom")
