﻿include_guard()
if("${XRAD_Root}" STREQUAL "")
	message(FATAL_ERROR "XRAD_Root is not defined.")
endif()
include(${XRAD_Root}/XRADBasic/CMake/Package.cmake)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# Записать в переменную var_name имя файла библиотеки package_name/library_name с путем
	function(fantom_compose_shared_library_name var_name package_name)
		if(ARGC GREATER_EQUAL 3)
			set(library_name ${ARGV2})
		else()
			set(library_name ${package_name})
		endif()
		set(${var_name} "${CMAKE_CURRENT_BINARY_DIR}/../../../../${package_name}/${XRAD_MSVCRelativeBuildDir}${library_name}.lib" PARENT_SCOPE)
	endfunction()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# Записать в переменную var_name имя файла библиотеки package_name/library_name с путем
	function(fantom_compose_shared_library_name var_name package_name)
		if(ARGC GREATER_EQUAL 3)
			set(library_name ${ARGV2})
		else()
			set(library_name ${package_name})
		endif()
		set(${var_name} "${CMAKE_CURRENT_BINARY_DIR}/../${package_name}/lib${library_name}.so" PARENT_SCOPE)
	endfunction()
else()
	message(FATAL_ERROR "Unsupported CMAKE_CXX_COMPILER_ID: \"${CMAKE_CXX_COMPILER_ID}\".")
endif()

fantom_compose_shared_library_name(FantomLibrary_Library "FantomLibrary")
set(FantomLibrary_Libraries
	${FantomLibrary_Library}
	)
get_filename_component(FantomLibrary_IncludeDir "../.." ABSOLUTE BASE_DIR "${CMAKE_CURRENT_LIST_DIR}")
#message(FantomLibrary_IncludeDir=${FantomLibrary_IncludeDir})
