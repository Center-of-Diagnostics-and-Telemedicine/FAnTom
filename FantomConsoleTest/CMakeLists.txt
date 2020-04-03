cmake_minimum_required(VERSION 3.7)

set(Project "FantomConsoleTest")
project(${Project} LANGUAGES CXX)

set(XRAD_Root $ENV{XRADRoot} CACHE PATH "XRAD library sources directory.")
if("${XRAD_Root}" STREQUAL "")
	message(FATAL_ERROR "Please, specify XRAD library sources directory (XRAD_Root).")
endif()

include(${XRAD_Root}/XRADBasic/CMake/CommonOptions.cmake)
include(${XRAD_Root}/XRADBasic/CMake/Package.cmake)
include(${XRAD_Root}/XRADSystem/CMake/Package.cmake)
include(${XRAD_Root}/XRADConsoleUI/CMake/Package.cmake)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(FantomLibrary_Libraries "${CMAKE_CURRENT_BINARY_DIR}/../../../../FantomLibrary/${XRAD_MSVCRelativeBuildDir}FantomLibrary.lib")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(FantomLibrary_Libraries "${CMAKE_CURRENT_BINARY_DIR}/../FantomLibrary/libFantomLibrary.so")
else()
	message(FATAL_ERROR "Unsupported CMAKE_CXX_COMPILER_ID: \"${CMAKE_CXX_COMPILER_ID}\".")
endif()

set(Project_IncludeDirs ${Project_IncludeDirs}
	..
	${XRAD_IncludeDir}
	)

set(Project_Sources_cpp
	FantomConsoleTest.cpp
	)

set(Project_Sources_h
	pre.h
	)

set(Project_Sources_All
	${Project_Sources_cpp}
	${Project_Sources_h}
	)

add_executable(${Project} ${Project_Sources_All})
target_include_directories(${Project} PRIVATE ${Project_IncludeDirs})
target_link_libraries(${Project}
	${FantomLibrary_Libraries}
	${XRADConsoleUI_Libraries}
	${XRADSystem_Libraries}
	${XRADBasic_Libraries}
	)
