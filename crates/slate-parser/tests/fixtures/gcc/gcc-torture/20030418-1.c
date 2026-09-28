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
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20030418-1.c:10:1]
// DEFAULT: 9 │
// DEFAULT: 10 │ ╭─▶ void foo(int i)
// DEFAULT: 11 │ │   {
// DEFAULT: 12 │ │     int j=i;
// DEFAULT: 13 │ │
// DEFAULT: 14 │ │     void bar() { int x=j, y=i; }
// DEFAULT: 15 │ │
// DEFAULT: 16 │ │     dummy(&i);
// DEFAULT: 17 │ ╰─▶ }
// DEFAULT: 18 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
