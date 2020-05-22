cmake_minimum_required (VERSION 3.8)

set(Project "FantomWebServer")
project(${Project} LANGUAGES CXX)

set(XRAD_Root $ENV{XRADRoot} CACHE PATH "XRAD library sources directory.")
if("${XRAD_Root}" STREQUAL "")
	message(FATAL_ERROR "Please, specify XRAD library sources directory (XRAD_Root).")
endif()

include(${XRAD_Root}/XRADConsoleUI/CMake/Package.cmake)
include(../FantomLibrary/CMake/Package.cmake)
include(CMake/DetectQtConfiguration.cmake)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# TODO: Отключаем RPATH по умолчанию и добавляем . в путь поиска библиотек.
	set(CMAKE_SKIP_RPATH TRUE)
	# CMAKE_SKIP_RPATH=TRUE отключает CMAKE_BUILD_RPATH. Надо использовать ключ линкера.
	#set(CMAKE_BUILD_RPATH .)
	#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--rpath=.")
else()
	message(FATAL_ERROR "Unsupported CMAKE_CXX_COMPILER_ID: \"${CMAKE_CXX_COMPILER_ID}\".")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(Project_IncludeDirs ${Project_IncludeDirs}
	.
	${FantomLibrary_IncludeDir}
	${XRAD_IncludeDir}
	${Qt5Widgets_INCLUDE_DIRS}
	)

set(Project_pre_h
	pre.h
	)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(Project_Sources_QtWebApp_sys
		QtWebApp/qtservice/qtservice_win.cpp
		)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(Project_Sources_QtWebApp_sys
		QtWebApp/qtservice/qtservice_unix.cpp
		)
endif()

set(Project_Sources_QtWebApp
	${Project_Sources_QtWebApp_sys}
	QtWebApp/httpserver/httpconnectionhandler.cpp
	QtWebApp/httpserver/httpconnectionhandler.h
	QtWebApp/httpserver/httpconnectionhandlerpool.cpp
	QtWebApp/httpserver/httpconnectionhandlerpool.h
	QtWebApp/httpserver/httpcookie.cpp
	QtWebApp/httpserver/httpcookie.h
	QtWebApp/httpserver/httpglobal.cpp
	QtWebApp/httpserver/httpglobal.h
	QtWebApp/httpserver/httplistener.cpp
	QtWebApp/httpserver/httplistener.h
	QtWebApp/httpserver/httprequest.cpp
	QtWebApp/httpserver/httprequest.h
	QtWebApp/httpserver/httprequesthandler.cpp
	QtWebApp/httpserver/httprequesthandler.h
	QtWebApp/httpserver/httpresponse.cpp
	QtWebApp/httpserver/httpresponse.h
	QtWebApp/httpserver/httpsession.cpp
	QtWebApp/httpserver/httpsession.h
	QtWebApp/httpserver/httpsessionstore.cpp
	QtWebApp/httpserver/httpsessionstore.h
	QtWebApp/httpserver/staticfilecontroller.cpp
	QtWebApp/httpserver/staticfilecontroller.h
	QtWebApp/logging/dualfilelogger.cpp
	QtWebApp/logging/dualfilelogger.h
	QtWebApp/logging/filelogger.cpp
	QtWebApp/logging/filelogger.h
	QtWebApp/logging/logger.cpp
	QtWebApp/logging/logger.h
	QtWebApp/logging/logglobal.h
	QtWebApp/logging/logmessage.cpp
	QtWebApp/logging/logmessage.h
	QtWebApp/qtservice/qtservice.cpp
	QtWebApp/qtservice/qtservice.h
	QtWebApp/qtservice/qtservice_p.h
	QtWebApp/qtservice/QtServiceBase
	QtWebApp/qtservice/QtServiceController
	QtWebApp/qtservice/qtunixserversocket.cpp
	QtWebApp/qtservice/qtunixserversocket.h
	QtWebApp/qtservice/qtunixsocket.cpp
	QtWebApp/qtservice/qtunixsocket.h
	QtWebApp/templateengine/template.cpp
	QtWebApp/templateengine/template.h
	QtWebApp/templateengine/templatecache.cpp
	QtWebApp/templateengine/templatecache.h
	QtWebApp/templateengine/templateglobal.h
	QtWebApp/templateengine/templateloader.cpp
	QtWebApp/templateengine/templateloader.h
	)

set(Project_Sources_CXX
	main.cpp
	ManageBitmap.cpp
	ManageBitmap.h
	ManageServerCommands.cpp
	ManageServerCommands.h
	ManageStrings.cpp
	ManageStrings.h
	ManageTomogram.cpp
	ManageTomogram.h
	ManageWebPages.cpp
	ManageWebPages.h
	RequestMapper.cpp
	RequestMapper.h
	../Common/StringConverters_Qt_Fantom.cpp
	../Common/StringConverters_Qt_Fantom.h
	../Common/WebServerSettings.cpp
	../Common/WebServerSettings.h
	)
# FantomWebServer_QtLink.h

set(Project_Sources_All
	${Project_pre_h}
	${Project_Sources_QtWebApp}
	${Project_Sources_CXX}
	)

add_executable(${Project} ${Project_Sources_All})
target_include_directories(${Project} PRIVATE ${Project_IncludeDirs})
target_link_libraries(${Project}
	${FantomLibrary_Libraries}
	${XRADConsoleUI_Libraries}
	Qt5::Core Qt5::Gui Qt5::Network
	)
xrad_add_dependencies(${Project} "XRADConsoleUI;FantomLibrary")
