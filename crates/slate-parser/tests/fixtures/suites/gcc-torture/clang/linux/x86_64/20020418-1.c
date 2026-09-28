// SLATE-FILECHECK-DEFINES DEFAULT

/* PR c/6358
   This testcase ICEd on IA-32 in foo, because current_function_return_rtx
   was assigned a hard register only after expand_null_return was called,
   thus return pseudo was clobbered twice and the hard register not at
   all.  */
/* { dg-additional-options "-std=gnu89" } */

void baz (void);
                       
double foo (void)
{
  baz ();
  return;
}

double bar (void)
{
  baz ();
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × non-void function should return a value
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20020418-1.c:14:3]
// DEFAULT: 13 │   baz ();
// DEFAULT: 14 │   return;
// DEFAULT: ·   ───────
// DEFAULT: 15 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
