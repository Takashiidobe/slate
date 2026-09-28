typedef int T;

int shadowed(int *x) {
  int T = 1;
  return (T) * x + sizeof(T) + _Generic(T, int: 1, default: 0);
}

int unshadowed(int *x) {
  return (T) * x + sizeof(T) + (T){1};
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: non-arithmetic operand
// DEFAULT: ╭─[tests/fixtures/shadowed_typedef_cast.c:3:1]
// DEFAULT: 2 │
// DEFAULT: 3 │ ╭─▶ int shadowed(int *x) {
// DEFAULT: 4 │ │     int T = 1;
// DEFAULT: 5 │ │     return (T) * x + sizeof(T) + _Generic(T, int: 1, default: 0);
// DEFAULT: 6 │ ╰─▶ }
// DEFAULT: 7 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
