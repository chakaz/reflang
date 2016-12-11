cmake_minimum_required (VERSION 2.8.11)
project (reflang)
enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(LibClang REQUIRED)

include_directories(${CLANG_INCLUDEDIR})
include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -stdlib=libc++")
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

add_library(libreflang STATIC lib/reflang.hpp lib/reflang.cpp)

add_executable(
	reflang
	gen/cmdargs.cpp gen/cmdargs.hpp
	gen/main.cpp
	gen/parser.class.cpp gen/parser.class.hpp
	gen/parser.cpp gen/parser.hpp
	gen/parser.enum.cpp gen/parser.enum.hpp
	gen/parser.function.cpp gen/parser.function.hpp
	gen/parser.util.cpp gen/parser.util.hpp
	gen/serializer.class.cpp gen/serializer.class.hpp
	gen/serializer.cpp gen/serializer.hpp
	gen/serializer.enum.cpp gen/serializer.enum.hpp
	gen/serializer.function.cpp gen/serializer.function.hpp
	gen/types.cpp gen/types.hpp)
target_link_libraries(reflang ${CLANG_LIBS})

add_subdirectory(tests)
