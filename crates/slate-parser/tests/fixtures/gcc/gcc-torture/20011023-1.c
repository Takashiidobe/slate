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
// DEFAULT: × unsupported in numeric IR lowering: linkage storage class
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20011023-1.c:6:1]
// DEFAULT: 5 │     void bar (void);
// DEFAULT: 6 │ ╭─▶ void bar (void)
// DEFAULT: 7 │ │   {
// DEFAULT: 8 │ │     auto void baz (void);
// DEFAULT: 9 │ │     void baz (void)
// DEFAULT: 10 │ │       {
// DEFAULT: 11 │ │         char tmp[2];
// DEFAULT: 12 │ │         foo (tmp);
// DEFAULT: 13 │ │       }
// DEFAULT: 14 │ │     baz ();
// DEFAULT: 15 │ ╰─▶ }
// DEFAULT: 16 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
