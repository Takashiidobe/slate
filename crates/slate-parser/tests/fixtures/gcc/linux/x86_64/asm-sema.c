register long unlabeled_global;
register long bad_global asm("ebx");
int f(int x) {
  register int unknown asm("notareg");
  asm("" : "=r"(x + 1));
  asm("" : "=r,m"(x) : "r"(x));
  asm goto("" : : : : missing);
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved label name `missing`
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/asm-sema.c:7:23]
// DEFAULT: 6 │   asm("" : "=r,m"(x) : "r"(x));
// DEFAULT: 7 │   asm goto("" : : : : missing);
// DEFAULT: ·                       ───────
// DEFAULT: 8 │   return 0;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
