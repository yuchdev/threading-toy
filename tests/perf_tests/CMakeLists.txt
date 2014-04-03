cmake_minimum_required(VERSION 2.8)
project(timed_queue_performance_test CXX)
set(TARGET timed_queue_performance_test)
 
cmake_policy(SET CMP0015 NEW)
if(UNIX)
message("Unix")
  set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
  set(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread -Wl,--no-as-needed")

elseif(WIN32)
  set(CMAKE_CXX_FLAGS "/EHsc")
  set(MY_BOOST_DIR "C:/boost/boost_1_55_0")

endif()
 
set(BOOST_ROOT ${MY_BOOST_DIR}) 
set(Boost_DEBUG OFF)
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.55.0 COMPONENTS unit_test_framework REQUIRED) 

if(Boost_FOUND)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})                                                                                                                                                                                                                             
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