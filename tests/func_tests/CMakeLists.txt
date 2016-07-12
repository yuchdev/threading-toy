cmake_minimum_required(VERSION 2.8)
project(timed_queue_functional_test CXX)
set(TARGET timed_queue_functional_test)

cmake_policy(SET CMP0015 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin")

 
########################################################
# source files
file(GLOB SOURCES *.cpp ../../include/threading_toy/*.h)
  
########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} ../../include)
include_directories(${INCLUDE_DIRS})
 
########################################################
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
