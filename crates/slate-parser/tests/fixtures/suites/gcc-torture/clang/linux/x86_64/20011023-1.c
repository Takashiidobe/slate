// SLATE-FILECHECK-DEFINES DEFAULT

/* Test whether tree inlining works with prototyped nested functions.  */

extern void foo (char *x);
void bar (void);
void bar (void)
{
  auto void baz (void);
  void baz (void)
    {
      char tmp[2];
      foo (tmp);
    }
  baz ();
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × linkage storage class
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20011023-1.c:8:13]
// DEFAULT: 7 │ {
// DEFAULT: 8 │   auto void baz (void);
// DEFAULT: ·             ──────────
// DEFAULT: 9 │   void baz (void)
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × function definition is not allowed here
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20011023-1.c:9:3]
// DEFAULT: 8 │       auto void baz (void);
// DEFAULT: 9 │ ╭─▶   void baz (void)
// DEFAULT: 10 │ │       {
// DEFAULT: 11 │ │         char tmp[2];
// DEFAULT: 12 │ │         foo (tmp);
// DEFAULT: 13 │ ╰─▶     }
// DEFAULT: 14 │       baz ();
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
