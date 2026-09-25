register long unlabeled_global;
register long bad_global asm("ebx");
int f(int x) {
  register int unknown asm("notareg");
  asm("" : "=r"(x + 1));
  asm("" : "=r,m"(x) : "r"(x));
  asm goto("" : : : : missing);
  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unresolved label name `missing`
// SLATE-FILECHECK-END DEFAULT
