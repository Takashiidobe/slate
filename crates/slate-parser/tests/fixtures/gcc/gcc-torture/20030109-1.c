// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-additional-options "-std=gnu89" } */

void foo ()
{
  int x1, x2, x3;

  bar (&x2 - &x1, &x3 - &x2);
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `bar`
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20030109-1.c:8:3]
// DEFAULT: 7 │
// DEFAULT: 8 │   bar (&x2 - &x1, &x3 - &x2);
// DEFAULT: ·   ───
// DEFAULT: 9 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
