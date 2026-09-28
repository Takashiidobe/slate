// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/7675 */
/* Contributed by Volker Reichelt */

/* Verify that we don't put automatic variables
   in registers too early.  */

extern int dummy (int *);

void foo(int i)
{
  int j=i;

  void bar() { int x=j, y=i; }

  dummy(&i);
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: function definition is not allowed here
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20030418-1.c:14:3]
// DEFAULT: 13 │
// DEFAULT: 14 │   void bar() { int x=j, y=i; }
// DEFAULT: ·   ────────────────────────────
// DEFAULT: 15 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
