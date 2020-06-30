cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

project(conexant)
find_package(PkgConfig)
pkg_check_modules(LIBSERIAL libserial)

include_directories(${LIBSERIAL_INCLUDE_DIRS} ./include)

file(GLOB SOURCES ./src/*.cpp)

add_executable(conexant ${SOURCES})

set_property(TARGET conexant PROPERTY CXX_STANDARD 17)

target_link_libraries(conexant ${LIBSERIAL_LINK_LIBRARIES} stdc++fs)