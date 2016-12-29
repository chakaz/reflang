macro(reflang_lib_test TEST)
	message(STATUS "Generating lib test target: ${TEST}")

	add_executable("${TEST}" "${TEST}.cpp")
	target_link_libraries("${TEST}" libreflang)
	add_test("${TEST}" "${TEST}")
endmacro()

reflang_lib_test(object.test)
reflang_lib_test(reference.test)
