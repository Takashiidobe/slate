// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir /fp:strict
#pragma float_control(precise, off)
double mul_add(double a, double b, double c) { return a * b + c; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT:   × '#pragma float_control(precise, off)' is illegal when except is enabled
// DEFAULT:    ╭─[tests/fixtures/error/msvc/windows/x86_64/fp_option_strict_float_control_precise.c:1:1]
// DEFAULT:  1 │ #pragma float_control(precise, off)
// DEFAULT:    · ───────────────────────────────────
// DEFAULT:  2 │ double mul_add(double a, double b, double c) { return a * b + c; }
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
