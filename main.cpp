#include <string>
#include <iostream>

#include <clang-c/Index.h>

int main(int argc, char *argv[]) {
	CXIndex Index = clang_createIndex(0, 0);
	CXTranslationUnit TU = clang_parseTranslationUnit(Index, 0,
			argv, argc, 0, 0, CXTranslationUnit_None);
	for (unsigned I = 0, N = clang_getNumDiagnostics(TU); I != N; ++I) {
		CXDiagnostic Diag = clang_getDiagnostic(TU, I);
		CXString String = clang_formatDiagnostic(Diag,
				clang_defaultDiagnosticDisplayOptions());
		fprintf(stderr, "%s\n", clang_getCString(String));
		clang_disposeString(String);
	}
	clang_disposeTranslationUnit(TU);
	clang_disposeIndex(Index);
	return 0;
}
