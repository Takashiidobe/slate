// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir /fp:fast
#pragma fenv_access(on)
double mul_add(double a, double b, double c) { return a * b + c; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT:   × '#pragma STDC FENV_ACCESS ON' is illegal when precise is disabled
// DEFAULT:    ╭─[tests/fixtures/error/msvc/windows/x86_64/fp_option_fast_fenv_access.c:1:1]
// DEFAULT:  1 │ #pragma fenv_access(on)
// DEFAULT:    · ───────────────────────
// DEFAULT:  2 │ double mul_add(double a, double b, double c) { return a * b + c; }
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
