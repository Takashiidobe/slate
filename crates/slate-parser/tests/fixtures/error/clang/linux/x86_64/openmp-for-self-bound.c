void f(int n) { int i;
#pragma omp for
for(i=0;i<i+4;i++) {}
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × condition expression refers to iteration variable
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/openmp-for-self-bound.c:3:11]
// PARSE:  2 │ #pragma omp for
// PARSE:  3 │ for(i=0;i<i+4;i++) {}
// PARSE:    ·           ───
// PARSE:  4 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
