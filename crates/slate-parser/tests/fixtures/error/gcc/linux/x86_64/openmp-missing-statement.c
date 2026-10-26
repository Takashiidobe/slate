void f(void) {
#pragma omp parallel
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected a statement after `#pragma omp parallel`
// PARSE:   ╰─▶ expected a statement after `#pragma omp parallel`
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-missing-statement.c:2:13]
// PARSE:  1 │ void f(void) {
// PARSE:  2 │ #pragma omp parallel
// PARSE:    ·             ────────
// PARSE:  3 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
