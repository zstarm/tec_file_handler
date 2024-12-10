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

set(OUTTER_SOURCE_DIR "/Users/starman/Documents/Projects/tec_file_handler")

add_library(tec_fileHandling "${OUTTER_SOURCE_DIR}/src/tec_asciiFileHandling.cpp")
target_sources(tec_fileHandling PRIVATE "${OUTTER_SOURCE_DIR}/src/tec_asciiFormat.cpp" "${OUTTER_SOURCE_DIR}/src/tec_fileContent.cpp" "${OUTTER_SOURCE_DIR}/src/tec_error.cpp")

target_include_directories(tec_fileHandling PRIVATE ${OUTTER_SOURCE_DIR}/include/)
target_compile_options(tec_fileHandling PUBLIC "-g")


