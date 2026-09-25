// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-additional-options "-std=gnu89" } */

void foo ()
{
  int x1, x2, x3;

  bar (&x2 - &x1, &x3 - &x2);
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unresolved ordinary name `bar`
// SLATE-FILECHECK-END DEFAULT
