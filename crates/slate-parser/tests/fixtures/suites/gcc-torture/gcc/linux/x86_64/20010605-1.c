// SLATE-FILECHECK-DEFINES DEFAULT

int
main (int argc, char **argv)
{
  int size = 10;

  typedef struct {
    char val[size];
  } block;
  block retframe_block()
    {
      return *(block*)0;
    }

  return 0;
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT:   × not implemented: value of a record with a variably modified member
// DEFAULT:     ╭─[tests/fixtures/suites/gcc-torture/gcc/linux/x86_64/20010605-1.c:12:14]
// DEFAULT:  11 │     {
// DEFAULT:  12 │       return *(block*)0;
// DEFAULT:     ·              ──────────
// DEFAULT:  13 │     }
// DEFAULT:     ╰────
// SLATE-FILECHECK-END DEFAULT
