cmake_minimum_required(VERSION 3.22)
project(data_storage)

set(CMAKE_CXX_STANDARD 20)
set(HAVE_CLANG_THREAD_SAFETY 0)

add_subdirectory(dependencies/leveldb)
add_subdirectory(dependencies/g3log)
add_subdirectory(dependencies/alpaca)

FILE(GLOB SRC src/*.cpp src/*.h)
include_directories(dependencies/leveldb/include)
include_directories(dependencies/g3log/src)
include_directories(dependencies/alpaca/include)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system REQUIRED)

add_executable(data_storage ${SRC} ${SRC})
set_target_properties(data_storage PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wpedantic")

target_link_libraries(data_storage leveldb g3log alpaca Boost::system)
