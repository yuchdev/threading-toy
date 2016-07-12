cmake_minimum_required(VERSION 2.8)
project(timed_queue_test_app CXX)
set(TARGET timed_queue_test_app)
 
cmake_policy(SET CMP0015 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

 
########################################################
# source files
file(GLOB SOURCES *.cpp ../include/threading_toy/*.h)
 
 
########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})
 
 
########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} ../include)
include_directories(${INCLUDE_DIRS})
 
########################################################
add_executable(${TARGET} ${SOURCES})