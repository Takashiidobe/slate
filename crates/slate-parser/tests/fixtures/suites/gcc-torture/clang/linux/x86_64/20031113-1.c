// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-additional-options "-std=gnu89" } */

/* On Darwin, the stub for simple_cst_equal was not being emitted at all 
   causing the as to die and not create an object file.  */

int
attribute_list_contained ()
{
  return (simple_cst_equal ());
}
int
simple_cst_list_equal ()
{
  return (simple_cst_equal ());
}


int __attribute__((noinline))
simple_cst_equal ()
{
  return simple_cst_list_equal ();
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error: -Wimplicit-function-declaration
// DEFAULT:   × call to undeclared function 'simple_cst_equal'; ISO C99 and later do not
// DEFAULT:     ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20031113-1.c:10:11]
// DEFAULT:   9 │ {
// DEFAULT:  10 │   return (simple_cst_equal ());
// DEFAULT:     ·           ────────────────
// DEFAULT:  11 │ }
// DEFAULT:     ╰────
// SLATE-FILECHECK-END DEFAULT
