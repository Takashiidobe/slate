void f(void) {
  int a = 0;
#pragma omp parallel default(none)
  a++;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × variable `a` must have explicitly specified data-sharing attributes
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-default-none.c:4:3]
// PARSE:  3 │ #pragma omp parallel default(none)
// PARSE:  4 │   a++;
// PARSE:    ·   ─
// PARSE:  5 │ }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
