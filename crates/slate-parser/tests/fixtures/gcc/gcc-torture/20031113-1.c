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
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `simple_cst_equal`
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20031113-1.c:10:11]
// DEFAULT: 9 │ {
// DEFAULT: 10 │   return (simple_cst_equal ());
// DEFAULT: ·           ────────────────
// DEFAULT: 11 │ }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `simple_cst_equal`
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20031113-1.c:15:11]
// DEFAULT: 14 │ {
// DEFAULT: 15 │   return (simple_cst_equal ());
// DEFAULT: ·           ────────────────
// DEFAULT: 16 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
