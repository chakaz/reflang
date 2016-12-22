message(STATUS "Generating ${OUTPUT} from ${INPUT} with ${GEN} (filter: ${REGEX})")
execute_process(
	COMMAND "${GEN}" --include "${REGEX}" --out-hpp "${OUTPUT}" --reflang-include "#include \"lib/reflang.hpp\"" -- -std=c++14 -g ${INPUT}
	RESULT_VARIABLE result)
if (result)
	message(FATAL_ERROR "Error running ${CMD}")
endif()
