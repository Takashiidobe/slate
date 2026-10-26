void f(int n) { int i;
#pragma omp for
for(i=0;i<8;i*=2) {}
}

// SLATE-FILECHECK-ARGS /Zc:preprocessor /openmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × invalid increment expression in OpenMP `for` loop
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/openmp-for-bad-increment.c:3:13]
// PARSE:  2 │ #pragma omp for
// PARSE:  3 │ for(i=0;i<8;i*=2) {}
// PARSE:    ·             ────
// PARSE:  4 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
