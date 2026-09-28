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
// DEFAULT: ╭─[tests/fixtures/fp_pragma_fenv_access_requires_precise.c:1:1]
// DEFAULT: 1 │ ╭─▶ double imprecise(double a, double b) {
// DEFAULT: 2 │ │   #pragma float_control(precise, off)
// DEFAULT: 3 │ │     {
// DEFAULT: 4 │ │   #pragma STDC FENV_ACCESS ON
// DEFAULT: 5 │ │       return a / b;
// DEFAULT: 6 │ │     }
// DEFAULT: 7 │ ╰─▶ }
// DEFAULT: 8 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
