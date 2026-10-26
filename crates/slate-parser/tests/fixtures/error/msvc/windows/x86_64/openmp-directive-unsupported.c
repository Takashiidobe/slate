int main(void) {
  int total = 0;
#pragma omp parallel for reduction(+ : total)
  for (int i = 0; i < 10; i++)
    total += i;
  return total;
}

// SLATE-FILECHECK-ARGS /openmp:llvm
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × OpenMP directives are not supported yet
// PARSE:   ╰─▶ OpenMP directives are not supported yet
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/openmp-directive-unsupported.c:3:9]
// PARSE:  2 │   int total = 0;
// PARSE:  3 │ #pragma omp parallel for reduction(+ : total)
// PARSE:    ·         ───
// PARSE:  4 │   for (int i = 0; i < 10; i++)
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
