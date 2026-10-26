void f(int n) {
  int v[n];
#pragma omp parallel firstprivate(v)
  v[0] = 1;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × privatizing variable-length array `v` is not supported yet
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-private-vla.c:3:35]
// PARSE:  2 │   int v[n];
// PARSE:  3 │ #pragma omp parallel firstprivate(v)
// PARSE:    ·                                   ─
// PARSE:  4 │   v[0] = 1;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
