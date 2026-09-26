double pushed(double a, double b) {
#pragma float_control(precise, on, push)
  return a / b;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid in this context: '#pragma float_control push/pop' can only appear
// SLATE-FILECHECK-END DEFAULT
