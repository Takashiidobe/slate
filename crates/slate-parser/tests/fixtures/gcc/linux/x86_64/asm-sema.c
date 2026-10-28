register long unlabeled_global;
register long bad_global asm("ebx");
int f(int x) {
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
// DEFAULT:   × lvalue required in 'asm' statement
// DEFAULT:    ╭─[tests/fixtures/gcc/linux/x86_64/asm-sema.c:4:17]
// DEFAULT:  3 │ int f(int x) {
// DEFAULT:  4 │   asm("" : "=r"(x + 1));
// DEFAULT:    ·                 ─────
// DEFAULT:  5 │   asm("" : "=r,m"(x) : "r"(x));
// DEFAULT:    ╰────
// DEFAULT: Error:
// DEFAULT:   × operand constraints for 'asm' differ in number of alternatives
// DEFAULT:    ╭─[tests/fixtures/gcc/linux/x86_64/asm-sema.c:5:28]
// DEFAULT:  4 │   asm("" : "=r"(x + 1));
// DEFAULT:  5 │   asm("" : "=r,m"(x) : "r"(x));
// DEFAULT:    ·                            ─
// DEFAULT:  6 │   asm goto("" : : : : missing);
// DEFAULT:    ╰────
// DEFAULT: Error:
// DEFAULT:   × use of undeclared label 'missing'
// DEFAULT:    ╭─[tests/fixtures/gcc/linux/x86_64/asm-sema.c:6:23]
// DEFAULT:  5 │   asm("" : "=r,m"(x) : "r"(x));
// DEFAULT:  6 │   asm goto("" : : : : missing);
// DEFAULT:    ·                       ───────
// DEFAULT:  7 │   return 0;
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
