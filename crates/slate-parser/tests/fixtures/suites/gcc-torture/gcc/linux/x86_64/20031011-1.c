// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/12544 */
/* Origin: Tony Hosking <hosking@cs.purdue.edu> */

/* Verify that non-local structures passed by invisible
   reference are correctly put in the stack.  */

typedef struct {
  int a;
  int f;
} A;

A *b;

void x (A a) {
  void y () {
    a.a = 0;
  }

  b = &a;
  y();
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: GNU nested function
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/gcc/linux/x86_64/20031011-1.c:16:3]
// DEFAULT: 15 │     void x (A a) {
// DEFAULT: 16 │ ╭─▶   void y () {
// DEFAULT: 17 │ │       a.a = 0;
// DEFAULT: 18 │ ╰─▶   }
// DEFAULT: 19 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
