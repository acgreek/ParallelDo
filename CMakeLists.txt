cmake_minimum_required (VERSION 0.1)
project (serialparrelleprocessor)

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-DUNIT_TEST -g -std=c++0x")
	set(CMAKE_CXX_FLAGS "-DUNIT_TEST -ggdb3 -std=c++0x -Wextra -Wall -pedantic")
endif()


add_executable(test test.cc) 



set(test CMAKE_CXX_FLAGS "-Wall -g  -std=c++0x -DUNIT_TEST -fPIC -pie -rdynamic")
set(test CMAKE_LINKER_FLAGS "-pie -Wl,--rpath,.,--rpath,dist -lboost_thread -lboost_system")
set (test CMAKE_CXX_LINK_FLAGS "-Wall -pie -Wl,--rpath,.,--rpath,dist -lboost_thread -lboost_system")

target_link_libraries(test -pie -Wl,--rpath,.,--rpath,dist ExtremeCUnit dl -lboost_thread -lboost_system)

add_custom_target(run ./test )
add_custom_target(runv ./test -v)

ADD_DEPENDENCIES(run test)
ADD_DEPENDENCIES(runv test)
