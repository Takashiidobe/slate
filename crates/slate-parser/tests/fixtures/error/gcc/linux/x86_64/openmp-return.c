int f(void) {
#pragma omp parallel
  {
    return 1;
  }
  return 0;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × invalid branch to/from OpenMP structured block
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-return.c:4:5]
// PARSE:  3 │   {
// PARSE:  4 │     return 1;
// PARSE:    ·     ─────────
// PARSE:  5 │   }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
