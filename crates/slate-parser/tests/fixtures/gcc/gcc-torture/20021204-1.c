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
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: function definition is not allowed here
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20021204-1.c:9:3]
// DEFAULT: 8 │ {
// DEFAULT: 9 │   int q() { return 0; }
// DEFAULT: ·   ─────────────────────
// DEFAULT: 10 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
