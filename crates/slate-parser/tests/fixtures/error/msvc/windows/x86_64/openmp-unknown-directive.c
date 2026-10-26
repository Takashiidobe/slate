void f(void) {
#pragma omp frobnicate
  ;
}

// SLATE-FILECHECK-ARGS /Zc:preprocessor /openmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected an OpenMP directive
// PARSE:   ╰─▶ expected an OpenMP directive
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/openmp-unknown-directive.c:2:13]
// PARSE:  1 │ void f(void) {
// PARSE:  2 │ #pragma omp frobnicate
// PARSE:    ·             ──────────
// PARSE:  3 │   ;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
