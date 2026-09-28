double strict(double a, double b) {
#pragma float_control(except, on)
  {
#pragma float_control(precise, off)
    return a / b;
  }
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × '#pragma float_control(precise, off)' is illegal when except is enabled
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/fp_pragma_precise_off_requires_no_except.c:4:1]
// DEFAULT: 3 │   {
// DEFAULT: 4 │ #pragma float_control(precise, off)
// DEFAULT: · ───────────────────────────────────
// DEFAULT: 5 │     return a / b;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
