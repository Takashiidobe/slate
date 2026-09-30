// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-additional-options "-std=gnu89" } */

typedef struct {
  char y;
  char x[32];
} X;

int z (void)
{
  X xxx;
  xxx.x[0] =
  xxx.x[31] = '0';
  xxx.y = 0xf;
  return f (xxx, xxx);
}

int main (void)
{
  int val;

  val = z ();
  if (val != 0x60)
    abort ();
  exit (0);
}

int f(X x, X y)
{
  if (x.y != y.y)
    return 'F';

  return x.x[0] + y.x[0];
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `f`
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20000511-1.c:15:10]
// DEFAULT: 14 │   xxx.y = 0xf;
// DEFAULT: 15 │   return f (xxx, xxx);
// DEFAULT: ·          ─
// DEFAULT: 16 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
