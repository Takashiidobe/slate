void f(int n) { int i;
#pragma omp for
for(;i<8;i++) {}
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × invalid initialization in OpenMP `for` loop: expected `var = init` or `T
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/openmp-for-missing-init.c:3:1]
// PARSE:  2 │ #pragma omp for
// PARSE:  3 │ for(;i<8;i++) {}
// PARSE:    · ────────────────
// PARSE:  4 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
