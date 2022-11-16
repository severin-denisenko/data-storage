cmake_minimum_required(VERSION 3.22)
project(data_storage)

set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(dependencies/leveldb)
add_subdirectory(dependencies/g3log)
add_subdirectory(dependencies/alpaca)

FILE(GLOB SRC src/*.cpp src/*.h)
include_directories(dependencies/leveldb/include)
include_directories(dependencies/g3log/src)
include_directories(dependencies/alpaca/include)

add_executable(data_storage ${SRC})

target_link_libraries(data_storage leveldb g3log alpaca)
