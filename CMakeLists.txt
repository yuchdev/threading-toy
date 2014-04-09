cmake_minimum_required(VERSION 2.8)
project(timed_queue CXX)
set(TARGET timed_queue)

set(WINDOWS_BOOST_DIR "C:/boost/boost_1_55_0")

if(UNIX)

  set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
  set(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread -Wl,--no-as-needed")

elseif("WIN32")
message("Windows")
  set(CMAKE_CXX_FLAGS "/EHsc")
  set(MY_BOOST_DIR ${WINDOWS_BOOST_DIR})

endif()

add_subdirectory(src)
add_subdirectory(tests/func_tests)
add_subdirectory(tests/load_tests)
add_subdirectory(tests/perf_tests)
