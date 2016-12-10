cmake_minimum_required (VERSION 2.8.11)
project (reflang)
enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(LibClang REQUIRED)

include_directories(${CLANG_INCLUDEDIR})
include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -stdlib=libc++")
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

add_executable(
	reflang
	gen/main.cpp
	gen/types.cpp gen/types.hpp
	gen/serializer.cpp gen/serializer.hpp
	gen/parser.cpp gen/parser.hpp
	gen/parser.util.cpp gen/parser.util.hpp
	gen/parser.enum.cpp gen/parser.enum.hpp
	gen/parser.class.cpp gen/parser.class.hpp)
target_link_libraries(reflang ${CLANG_LIBS})

add_subdirectory(tests)
