// SLATE-FILECHECK-DEFINES DEFAULT

int
sub1 (char *p, int i)
{
  char j = p[i];

  {
    void
    sub2 ()
      {
	i = 2;
	p = p + 2;
      }
  }
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: GNU nested function
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/gcc/linux/x86_64/20020309-1.c:8:5]
// DEFAULT: 7 │       {
// DEFAULT: 8 │ ╭─▶     void
// DEFAULT: 9 │ │       sub2 ()
// DEFAULT: 10 │ │         {
// DEFAULT: 11 │ │       i = 2;
// DEFAULT: 12 │ │       p = p + 2;
// DEFAULT: 13 │ ╰─▶       }
// DEFAULT: 14 │       }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
