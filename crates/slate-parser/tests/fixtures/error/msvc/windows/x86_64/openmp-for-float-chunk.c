void f(int n) { int i;
#pragma omp for schedule(static,1.5)
for(i=0;i<8;i++) {}
}

// SLATE-FILECHECK-ARGS /Zc:preprocessor /openmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × chunk size expression must have integral type
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/openmp-for-float-chunk.c:2:33]
// PARSE:  1 │ void f(int n) { int i;
// PARSE:  2 │ #pragma omp for schedule(static,1.5)
// PARSE:    ·                                 ───
// PARSE:  3 │ for(i=0;i<8;i++) {}
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
