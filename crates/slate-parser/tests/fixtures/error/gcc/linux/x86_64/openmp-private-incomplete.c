extern struct T t;
void f(void) {
#pragma omp parallel private(t)
  ;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × variable `t` in a privatizing clause has incomplete type
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-private-incomplete.c:3:30]
// PARSE:  2 │ void f(void) {
// PARSE:  3 │ #pragma omp parallel private(t)
// PARSE:    ·                              ─
// PARSE:  4 │   ;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
