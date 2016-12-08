add_custom_target(
	cpp11-enum-rm-gen
	${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_SOURCE_DIR}/cpp11-enum.gen.hpp
	DEPENDS cpp11-enum.src.hpp $<TARGET_FILE:reflang>)

add_custom_command(
	OUTPUT cpp11-enum.gen.hpp
	COMMAND ${CMAKE_COMMAND}
	ARGS
		-DINPUT=${CMAKE_CURRENT_SOURCE_DIR}/cpp11-enum.src.hpp
		-DOUTPUT=${CMAKE_CURRENT_SOURCE_DIR}/cpp11-enum.gen.hpp
		-DGEN=$<TARGET_FILE:reflang>
		-P ${CMAKE_SOURCE_DIR}/cmake/runtest.cmake)

add_executable(
	cpp11-enum cpp11-enum.test.cpp cpp11-enum.gen.hpp)

add_test(cpp11-enum-test cpp11-enum)
