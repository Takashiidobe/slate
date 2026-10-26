void f(void) {
  for (;;) {
#pragma omp parallel
    {
      break;
    }
  }
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × invalid branch to/from OpenMP structured block
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-break-out.c:5:7]
// PARSE:  4 │     {
// PARSE:  5 │       break;
// PARSE:    ·       ──────
// PARSE:  6 │     }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
