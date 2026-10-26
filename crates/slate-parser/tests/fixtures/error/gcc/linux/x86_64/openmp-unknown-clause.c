void f(void) {
#pragma omp parallel bogus(1)
  ;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × unexpected OpenMP clause `bogus` in `#pragma omp parallel`
// PARSE:   ╰─▶ unexpected OpenMP clause `bogus` in `#pragma omp parallel`
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-unknown-clause.c:2:22]
// PARSE:  1 │ void f(void) {
// PARSE:  2 │ #pragma omp parallel bogus(1)
// PARSE:    ·                      ─────
// PARSE:  3 │   ;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
