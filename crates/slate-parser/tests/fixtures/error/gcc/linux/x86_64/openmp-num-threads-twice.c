void f(void) {
#pragma omp parallel num_threads(2) num_threads(3)
  ;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × `#pragma omp parallel` cannot contain more than one `num_threads` clause
// PARSE:   ╰─▶ `#pragma omp parallel` cannot contain more than one `num_threads` clause
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-num-threads-twice.c:2:37]
// PARSE:  1 │ void f(void) {
// PARSE:  2 │ #pragma omp parallel num_threads(2) num_threads(3)
// PARSE:    ·                                     ───────────
// PARSE:  3 │   ;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
