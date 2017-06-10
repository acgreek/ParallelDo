cmake_minimum_required (VERSION 0.1)
project (serialparrelleprocessor)

find_package(Boost 1.40 COMPONENTS thread REQUIRED)
find_package(PkgConfig)
pkg_check_modules (EXTREMECUNIT ExtremeCUnit)

string(REPLACE ";" " " EXTREMECUNIT_LDFLAGS2 "${EXTREMECUNIT_LDFLAGS}" )
string(REPLACE ";" " " EXTREMECUNIT_CFLAGS2 "${EXTREMECUNIT_CFLAGS}" )
message(STATUS "CFLAGS=" ${EXTREMECUNIT_CFLAGS2})

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-ggdb ${EXTREMECUNIT_CFLAGS2}")
	set(CMAKE_CXX_FLAGS "-ggdb ${EXTREMECUNIT_CFLAGS2}")
endif()


add_executable(test test.cc) 
add_executable(bench bench.cpp) 

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-O3 -Wextra -Wall -ggdb  -std=c++11 -DUNIT_TEST -fPIC")
set(CMAKE_CXX_LINK_FLAGS "-Wall -ggdb -pie -rdynamic -Wl,--rpath,.,--rpath,dist")
set(CMAKE_LINKER_FLAGS "-pie -Wl,--rpath,.,--rpath,dist")
set(CMAKE_LINKER_FLAGS "-pie -Wl,--rpath,.,--rpath,dist")

IF(CYGWIN)
set(BOOST_LIBS -lboost_thread-mt -lboost_system-mt)
ELSE()
set(BOOST_LIBS -lboost_thread -lboost_system)
ENDIF(CYGWIN)

target_link_libraries(test ${EXTREMECUNIT_LDFLAGS} ${BOOST_LIBS})
target_link_libraries(bench ${EXTREMECUNIT_LDFLAGS} ${BOOST_LIBS})

add_custom_target(run ./test )
add_custom_target(runv ./test -v)

ADD_DEPENDENCIES(run test)
ADD_DEPENDENCIES(runv test)

