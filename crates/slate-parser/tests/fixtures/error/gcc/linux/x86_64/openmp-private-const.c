void f(void) {
  const int a = 1;
#pragma omp parallel private(a)
  ;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × const-qualified variable `a` cannot be private
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-private-const.c:3:30]
// PARSE:  2 │   const int a = 1;
// PARSE:  3 │ #pragma omp parallel private(a)
// PARSE:    ·                              ─
// PARSE:  4 │   ;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
