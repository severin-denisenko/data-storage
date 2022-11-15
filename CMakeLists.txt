cmake_minimum_required(VERSION 3.22)
project(data_storage)

set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(dependencies/leveldb)

FILE(GLOB SRC src/*.cpp src/*.h)
include_directories(dependencies/leveldb/include)

add_executable(data_storage ${SRC})

target_link_libraries(data_storage leveldb glog)
