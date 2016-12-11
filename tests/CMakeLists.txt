macro(reflang_test TEST)
	message(STATUS "Generating test target: ${TEST}")

	set(INPUT "${TEST}.src.hpp")
	set(OUTPUT "${TEST}.gen.hpp")
	set(SOURCE "${TEST}.test.cpp")

	add_custom_command(
		OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT}"
		COMMAND ${CMAKE_COMMAND}
		ARGS
			-DINPUT=${CMAKE_CURRENT_SOURCE_DIR}/${INPUT}
			-DOUTPUT=${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT}
			-DGEN=$<TARGET_FILE:reflang>
			-P ${CMAKE_SOURCE_DIR}/cmake/runtest.cmake)

	add_executable(
		${TEST} ${SOURCE} ${INPUT} "${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT}")

	add_test("${TEST}-test" ${TEST})
endmacro()

add_subdirectory(enum)
add_subdirectory(function)
