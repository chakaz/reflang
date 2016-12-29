message(STATUS "Generating ${OUT_HPP} & ${OUT_CPP} from ${INPUT} with ${GEN} (filter: ${REGEX})")
execute_process(
	COMMAND "${GEN}" --include "${REGEX}" --out-hpp "${OUT_HPP}" --out-cpp "${OUT_CPP}" --reflang-include "#include \"lib/reflang.hpp\"" ${INPUT} -- -std=c++14 -g
	RESULT_VARIABLE result)
if (result)
	message(FATAL_ERROR "Error running ${CMD}")
endif()
