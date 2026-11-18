double misplaced(double a, double b, int c) {
  if (c)
#pragma STDC FENV_ACCESS ON
  a = 1;
  return a / b;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT:   × floating-point pragma can only appear at file scope or at the start of a
// DEFAULT:    ╭─[tests/fixtures/clang/linux/x86_64/fp_pragma_misplaced_in_if_body.c:3:1]
// DEFAULT:  2 │   if (c)
// DEFAULT:  3 │ #pragma STDC FENV_ACCESS ON
// DEFAULT:    · ───────────────────────────
// DEFAULT:  4 │   a = 1;
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
