void h(void);
void f(void) {
#pragma omp parallel private(h)
  ;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × `h` is not a variable in clause `private`
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-not-variable.c:3:30]
// PARSE:  2 │ void f(void) {
// PARSE:  3 │ #pragma omp parallel private(h)
// PARSE:    ·                              ─
// PARSE:  4 │   ;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
