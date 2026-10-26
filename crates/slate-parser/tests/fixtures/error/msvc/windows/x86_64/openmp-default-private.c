void f(void) {
  int a = 0;
#pragma omp parallel default(private)
  a++;
}

// SLATE-FILECHECK-ARGS /Zc:preprocessor /openmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid argument in OpenMP `default` clause
// PARSE:   ╰─▶ invalid argument in OpenMP `default` clause
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/openmp-default-private.c:3:30]
// PARSE:  2 │   int a = 0;
// PARSE:  3 │ #pragma omp parallel default(private)
// PARSE:    ·                              ───────
// PARSE:  4 │   a++;
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
