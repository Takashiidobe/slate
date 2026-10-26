#pragma omp parallel
int x;

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × unexpected OpenMP directive `#pragma omp parallel`
// PARSE:   ╰─▶ unexpected OpenMP directive `#pragma omp parallel`
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-file-scope.c:1:13]
// PARSE:  1 │ #pragma omp parallel
// PARSE:    ·             ────────
// PARSE:  2 │ int x;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
