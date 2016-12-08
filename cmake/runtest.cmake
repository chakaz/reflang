message(STATUS "Generating ${OUTPUT} from ${INPUT} with ${GEN}")
execute_process(
	COMMAND ${GEN} --gen .* -- -std=c++14 --stdlib=libc++ -g ${INPUT}
	RESULT_VARIABLE result
	OUTPUT_FILE ${OUTPUT})
if (result)
	message(FATAL_ERROR "Error running ${CMD}")
endif()
