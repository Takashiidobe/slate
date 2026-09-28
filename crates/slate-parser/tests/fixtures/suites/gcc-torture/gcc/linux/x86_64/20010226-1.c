// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-require-effective-target trampolines } */

void f1 (void *);
void f3 (void *, void (*)(void *));
void f2 (void *);

int foo (void *a, int b)
{
  if (!b)
    {
      f1 (a);
      return 1;
    }
  if (b)
    {
      void bar (void *c)
      {
	if (c == a)
	  f2 (c);
      }
      f3 (a, bar);
    }
  return 0;
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: GNU nested function
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/gcc/linux/x86_64/20010226-1.c:17:7]
// DEFAULT: 16 │         {
// DEFAULT: 17 │ ╭─▶       void bar (void *c)
// DEFAULT: 18 │ │         {
// DEFAULT: 19 │ │       if (c == a)
// DEFAULT: 20 │ │         f2 (c);
// DEFAULT: 21 │ ╰─▶       }
// DEFAULT: 22 │           f3 (a, bar);
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
