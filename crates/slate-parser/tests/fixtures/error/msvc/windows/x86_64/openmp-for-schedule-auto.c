void f(void) { int i;
#pragma omp for schedule(auto)
for(i=0;i<8;i++) {}
}

// SLATE-FILECHECK-ARGS /Zc:preprocessor /openmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid schedule kind in OpenMP `schedule` clause
// PARSE:   ╰─▶ invalid schedule kind in OpenMP `schedule` clause
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/openmp-for-schedule-auto.c:2:26]
// PARSE:  1 │ void f(void) { int i;
// PARSE:  2 │ #pragma omp for schedule(auto)
// PARSE:    ·                          ────
// PARSE:  3 │ for(i=0;i<8;i++) {}
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
