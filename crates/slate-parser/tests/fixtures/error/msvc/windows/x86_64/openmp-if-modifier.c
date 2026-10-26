void f(int n) {
#pragma omp parallel if(parallel : n)
  ;
}

// SLATE-FILECHECK-ARGS /Zc:preprocessor /openmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × unexpected tokens after expression
// PARSE:   ╰─▶ unexpected tokens after expression
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/openmp-if-modifier.c:2:25]
// PARSE:  1 │ void f(int n) {
// PARSE:  2 │ #pragma omp parallel if(parallel : n)
// PARSE:    ·                         ────────
// PARSE:  3 │   ;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
