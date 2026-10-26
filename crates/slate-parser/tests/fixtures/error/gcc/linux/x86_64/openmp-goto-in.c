void f(void) {
  goto in;
#pragma omp parallel
  {
  in:;
  }
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × invalid branch to/from OpenMP structured block
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-goto-in.c:2:3]
// PARSE:  1 │ void f(void) {
// PARSE:  2 │   goto in;
// PARSE:    ·   ────────
// PARSE:  3 │ #pragma omp parallel
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
