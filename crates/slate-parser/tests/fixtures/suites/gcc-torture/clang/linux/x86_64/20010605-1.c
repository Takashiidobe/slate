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
// DEFAULT: × function definition is not allowed here
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20010605-1.c:10:3]
// DEFAULT: 9 │       } block;
// DEFAULT: 10 │ ╭─▶   block retframe_block()
// DEFAULT: 11 │ │       {
// DEFAULT: 12 │ │         return *(block*)0;
// DEFAULT: 13 │ ╰─▶     }
// DEFAULT: 14 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
