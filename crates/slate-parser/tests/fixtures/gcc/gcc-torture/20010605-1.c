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
// DEFAULT: Error:   × unsupported in numeric IR lowering: nonconstant or unknown identifier
// SLATE-FILECHECK-END DEFAULT
