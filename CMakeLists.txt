cmake_minimum_required (VERSION 2.8.11)
project (reflang)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(LibClang REQUIRED)
include_directories(${CLANG_INCLUDEDIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -stdlib=libc++")

add_executable(reflang main.cpp)
target_link_libraries(reflang ${CLANG_LIBS})
