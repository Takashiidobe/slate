// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-additional-options "-std=gnu89" } */

void zzz (char *s1, char *s2, int len, int *q)
{
  int z = 5;
  unsigned int i,  b;
  struct { char a[z]; } x;
          
  for (i = 0; i < len; i++)
    s1[i] = s2[i];

  b = z & 0x3;

  len += (b == 0 ? 0 : 1) + z;
    
  *q = len;

  foo (x, x);
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `foo`
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/gcc/linux/x86_64/20030224-1.c:19:3]
// DEFAULT: 18 │
// DEFAULT: 19 │   foo (x, x);
// DEFAULT: ·   ───
// DEFAULT: 20 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
