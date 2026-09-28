// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/7520 */
/* ICE at -O3 on x86 due to register life problems caused by
   the return-without-value in bar.  */
/* { dg-additional-options "-std=gnu89" } */

int
foo ()
{
  int i;
  long long int j;

  while (1)
    {
      if (j & 1)
	++i;
      j >>= 1;
      if (j)
	return i;
    }
}

int
bar ()
{
  if (foo ())
    return;
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: non-void function should return a value
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20020927-1.c:27:5]
// DEFAULT: 26 │   if (foo ())
// DEFAULT: 27 │     return;
// DEFAULT: ·     ───────
// DEFAULT: 28 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
