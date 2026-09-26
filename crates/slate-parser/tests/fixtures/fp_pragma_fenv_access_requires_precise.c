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
// DEFAULT: Error:   × invalid in this context: '#pragma STDC FENV_ACCESS ON' is illegal when
// SLATE-FILECHECK-END DEFAULT
