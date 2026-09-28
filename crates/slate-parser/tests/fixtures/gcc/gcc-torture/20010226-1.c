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
// DEFAULT: × invalid in this context: function definition is not allowed here
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20010226-1.c:8:1]
// DEFAULT: 7 │
// DEFAULT: 8 │ ╭─▶ int foo (void *a, int b)
// DEFAULT: 9 │ │   {
// DEFAULT: 10 │ │     if (!b)
// DEFAULT: 11 │ │       {
// DEFAULT: 12 │ │         f1 (a);
// DEFAULT: 13 │ │         return 1;
// DEFAULT: 14 │ │       }
// DEFAULT: 15 │ │     if (b)
// DEFAULT: 16 │ │       {
// DEFAULT: 17 │ │         void bar (void *c)
// DEFAULT: 18 │ │         {
// DEFAULT: 19 │ │       if (c == a)
// DEFAULT: 20 │ │         f2 (c);
// DEFAULT: 21 │ │         }
// DEFAULT: 22 │ │         f3 (a, bar);
// DEFAULT: 23 │ │       }
// DEFAULT: 24 │ │     return 0;
// DEFAULT: 25 │ ╰─▶ }
// DEFAULT: 26 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
