cmake_minimum_required(VERSION 2.8)
project(threading_toy CXX)
set(TARGET threading_toy)

include(CTest)
enable_testing()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/tools/cmake")

set(BOOST_MIN_VERSION "1.59.0")
set(Boost_ADDITIONAL_VERSIONS "1.59.0" "1.59")
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT ${MY_BOOST_DIR}) 

find_package(Boost ${BOOST_MIN_VERSION} REQUIRED) 

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message("UNIX congiguration, Clang")
    set(CMAKE_CXX_FLAGS "-std=gnu++11 -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "-std=gnu++11")
elseif(${CMAKE_COMPILER_IS_GNUCC})
    message("UNIX congiguration, GCC")
    set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
    set(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread")
elseif(${WIN32})
    message("Windows configuraion")
    set(CMAKE_CXX_FLAGS "/EHsc")
    set(MY_BOOST_DIR ${WINDOWS_BOOST_DIR})

    # define WIN32 macro version
    macro(get_WIN32_WINNT version)
        if (WIN32 AND CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REPLACE "." "" ver ${ver})
            string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

            set(${version} "0x${ver}")
        endif()
    endmacro()

    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)

endif()

# Use folders to sort out projects in VS solution
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)
add_subdirectory(tests/func_tests)
add_subdirectory(tests/load_tests)
add_subdirectory(tests/perf_tests)
