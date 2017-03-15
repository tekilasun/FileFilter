cmake_minimum_required(VERSION 3.6)
project(Filefilter)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


set(SOURCE_FILES main.cpp FileFilter.cpp)
add_executable(Filefilter ${SOURCE_FILES})


target_link_libraries(Filefilter boost_system boost_filesystem )