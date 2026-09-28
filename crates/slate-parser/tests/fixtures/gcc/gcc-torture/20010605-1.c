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
// DEFAULT: × unsupported in numeric IR lowering: nonconstant or unknown identifier
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20010605-1.c:7:3]
// DEFAULT: 6 │
// DEFAULT: 7 │ ╭─▶   typedef struct {
// DEFAULT: 8 │ │       char val[size];
// DEFAULT: 9 │ ╰─▶   } block;
// DEFAULT: 10 │       block retframe_block()
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
