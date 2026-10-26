void f(int n) { int i;
#pragma omp for
for(float f=0;f<8;f++) {}
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × invalid type for iteration variable `f`
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-for-float-variable.c:3:1]
// PARSE:  2 │ #pragma omp for
// PARSE:  3 │ for(float f=0;f<8;f++) {}
// PARSE:    · ─────────────────────────
// PARSE:  4 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
