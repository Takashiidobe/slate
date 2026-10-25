// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir /fp:fast
#pragma float_control(except, on)
double mul_add(double a, double b, double c) { return a * b + c; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT:   × '#pragma float_control(except, on)' is illegal when precise is disabled
// DEFAULT:    ╭─[tests/fixtures/error/msvc/windows/x86_64/fp_option_fast_float_control_except.c:1:1]
// DEFAULT:  1 │ #pragma float_control(except, on)
// DEFAULT:    · ─────────────────────────────────
// DEFAULT:  2 │ double mul_add(double a, double b, double c) { return a * b + c; }
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
