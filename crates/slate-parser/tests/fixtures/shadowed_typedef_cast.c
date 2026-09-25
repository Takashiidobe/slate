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
// DEFAULT: Error:   × unsupported in numeric IR lowering: non-arithmetic operand
// SLATE-FILECHECK-END DEFAULT
