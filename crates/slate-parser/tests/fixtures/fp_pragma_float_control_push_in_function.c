double pushed(double a, double b) {
#pragma float_control(precise, on, push)
  return a / b;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: '#pragma float_control push/pop' can only appear
// DEFAULT: ╭─[tests/fixtures/fp_pragma_float_control_push_in_function.c:1:1]
// DEFAULT: 1 │ ╭─▶ double pushed(double a, double b) {
// DEFAULT: 2 │ │   #pragma float_control(precise, on, push)
// DEFAULT: 3 │ │     return a / b;
// DEFAULT: 4 │ ╰─▶ }
// DEFAULT: 5 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
