void f(double d) {
#pragma omp parallel num_threads(d)
  ;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × `num_threads` expression must have integral type
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-num-threads-float.c:2:34]
// PARSE:  1 │ void f(double d) {
// PARSE:  2 │ #pragma omp parallel num_threads(d)
// PARSE:    ·                                  ─
// PARSE:  3 │   ;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
