double imprecise(double a, double b) {
#pragma float_control(precise, off)
  {
#pragma STDC FENV_ACCESS ON
    return a / b;
  }
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: '#pragma STDC FENV_ACCESS ON' is illegal when
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/fp_pragma_fenv_access_requires_precise.c:4:1]
// DEFAULT: 3 │   {
// DEFAULT: 4 │ #pragma STDC FENV_ACCESS ON
// DEFAULT: · ───────────────────────────
// DEFAULT: 5 │     return a / b;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
