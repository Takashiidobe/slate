int main(void) {
  int total = 0;
#pragma omp parallel for reduction(+ : total)
  for (int i = 0; i < 10; i++)
    total += i;
  return total;
}

// SLATE-FILECHECK-ARGS -fopenmp
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × OpenMP directive `#pragma omp parallel for` is not supported yet
// PARSE:   ╰─▶ OpenMP directive `#pragma omp parallel for` is not supported yet
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/openmp-directive-unsupported.c:3:13]
// PARSE:  2 │   int total = 0;
// PARSE:  3 │ #pragma omp parallel for reduction(+ : total)
// PARSE:    ·             ────────
// PARSE:  4 │   for (int i = 0; i < 10; i++)
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
