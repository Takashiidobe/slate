void f(void) {
  int a = 0;
#pragma omp parallel private(a) shared(a)
  a++;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × `a` appears more than once in data-sharing clauses
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-duplicate-variable.c:3:40]
// PARSE:  2 │   int a = 0;
// PARSE:  3 │ #pragma omp parallel private(a) shared(a)
// PARSE:    ·                                        ─
// PARSE:  4 │   a++;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
