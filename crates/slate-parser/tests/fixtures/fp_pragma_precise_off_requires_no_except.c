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
// DEFAULT: Error:   × invalid in this context: '#pragma float_control(precise, off)' is illegal
// SLATE-FILECHECK-END DEFAULT
