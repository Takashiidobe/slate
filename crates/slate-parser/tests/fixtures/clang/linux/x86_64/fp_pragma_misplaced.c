double misplaced(double a, double b) {
  double x = a;
#pragma STDC FENV_ACCESS ON
  return x / b;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: floating-point pragma can only appear at file
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/fp_pragma_misplaced.c:3:1]
// DEFAULT: 2 │   double x = a;
// DEFAULT: 3 │ #pragma STDC FENV_ACCESS ON
// DEFAULT: · ───────────────────────────
// DEFAULT: 4 │   return x / b;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
