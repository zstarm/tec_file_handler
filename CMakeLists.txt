#Tecplot ASCII and SZL file interface library 
#Version 0.1
cmake_minimum_required(VERSION 3.10)

#project details
project("Tecplot File Handling Library" VERSION 0.1)

if(${APPLE})
	set(CMAKE_OSX_DEPLOYMENT_TARGET 14.0)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(OUTTER_SOURCE_DIR "~/Documents/Github Repos/tec_file_handler/")
#set(TECIO_DIR "/Users/starman/Documents/Projects/tec_file_handler/tecio")

#find_library(TECIOLIB NAMES tecio libtecio HINTS "${CODE_DIR}/libs/")

add_library(tec_fileHandling "${CODE_DIR}/src/tec_asciiFileHandling.cpp")
target_sources(tec_fileHandling PRIVATE "${CODE_DIR}/src/tec_asciiFormat.cpp")
target_sources(tec_fileHandling PRIVATE "${CODE_DIR}/src/tec_szlFileHandling.cpp")
target_sources(tec_fileHandling PUBLIC "${CODE_DIR}/src/tec_fileContainer.cpp" "${CODE_DIR}/src/tec_variable.cpp" "${CODE_DIR}/src/tec_zoneInformation.cpp" "${CODE_DIR}/src/tec_zoneData.cpp")
target_sources(tec_fileHandling PRIVATE "${CODE_DIR}/src/tec_error.cpp")

target_include_directories(tec_fileHandling PRIVATE ${CODE_DIR}/include/)
target_include_directories(tec_fileHandling PRIVATE ${TECIO_DIR})
target_compile_options(tec_fileHandling PRIVATE -g)
