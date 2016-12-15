message(STATUS "Generating ${OUTPUT} from ${INPUT} with ${GEN}")
execute_process(
	COMMAND "${GEN}" --reflang-include "#include \"lib/reflang.hpp\"" -- -std=c++14 -g ${INPUT}
	RESULT_VARIABLE result
	OUTPUT_FILE ${OUTPUT})
if (result)
	message(FATAL_ERROR "Error running ${CMD}")
endif()
