cmake_minimum_required(VERSION 2.8)
project(timed_queue_performance_test CXX)
set(TARGET timed_queue_performance_test)
 
cmake_policy(SET CMP0015 NEW)
if(UNIX)
SET(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread -Wl,--no-as-needed")
elseif(WINDOWS)
SET(CMAKE_CXX_FLAGS "/EHsc")
endif()
 
 
########################################################
# source files
file(GLOB Files_src *.cpp)
 
 
########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})
 
source_group("Sources" FILES ${SOURCES})
 
 
########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} . ../../include)
include_directories(${INCLUDE_DIRS})
 
########################################################
add_executable(${TARGET} ${SOURCES})