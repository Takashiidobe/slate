// SLATE-FILECHECK-DEFINES DEFAULT

/* PR c/7622 */

/* Verify that GCC can handle the mix of
   extern inline and nested functions. */

extern inline int t()
{
  int q() { return 0; }

  return q();
}

int foo()
{
  return t();
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: module statement
// SLATE-FILECHECK-END DEFAULT
