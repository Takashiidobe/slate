// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-additional-options "-std=gnu89" } */

static inline void
foo (long long const v0, long long const v1)
{
  bar (v0 == v1);
}

void
test (void)
{
  foo (0, 1);
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `bar`
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/gcc/linux/x86_64/20030612-1.c:7:3]
// DEFAULT: 6 │ {
// DEFAULT: 7 │   bar (v0 == v1);
// DEFAULT: ·   ───
// DEFAULT: 8 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
