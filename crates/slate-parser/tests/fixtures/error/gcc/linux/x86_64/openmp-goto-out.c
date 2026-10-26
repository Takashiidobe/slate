void f(void) {
#pragma omp parallel
  {
    goto out;
  }
out:;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × invalid branch to/from OpenMP structured block
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-goto-out.c:4:5]
// PARSE:  3 │   {
// PARSE:  4 │     goto out;
// PARSE:    ·     ─────────
// PARSE:  5 │   }
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
