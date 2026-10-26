struct S {
  int x;
};
void f(struct S s) {
#pragma omp parallel if(s)
  ;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × non-scalar condition
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-if-struct.c:5:25]
// PARSE:  4 │ void f(struct S s) {
// PARSE:  5 │ #pragma omp parallel if(s)
// PARSE:    ·                         ─
// PARSE:  6 │   ;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
