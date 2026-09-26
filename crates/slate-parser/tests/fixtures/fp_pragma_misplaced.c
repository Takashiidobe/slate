double misplaced(double a, double b) {
  double x = a;
#pragma STDC FENV_ACCESS ON
  return x / b;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid in this context: floating-point pragma can only appear at file
// SLATE-FILECHECK-END DEFAULT
