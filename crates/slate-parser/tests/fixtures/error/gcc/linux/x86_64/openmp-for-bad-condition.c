void f(int n) { int i;
#pragma omp for
for(i=0;i==8;i++) {}
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × invalid controlling predicate in OpenMP `for` loop
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-for-bad-condition.c:3:9]
// PARSE:  2 │ #pragma omp for
// PARSE:  3 │ for(i=0;i==8;i++) {}
// PARSE:    ·         ────
// PARSE:  4 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
