// SLATE-FILECHECK-ARGS --dump-ir -std=c2y
// SLATE-FILECHECK-ERROR SEMANTIC

int f(int a) {
  int x = (static_assert(1, "passes"), a);
  (void) static_assert(0, "cast");
  a ? static_assert(sizeof(int) == 2) : (void) 0;
  extern typeof(static_assert(0, "typeof")) g(void);
  for (static_assert(0, "for");;)
    break;
  return x;
}

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × static assertion failed: cast
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/static_assert_expression_failed.c:4:24]
// SEMANTIC: 3 │   int x = (static_assert(1, "passes"), a);
// SEMANTIC: 4 │   (void) static_assert(0, "cast");
// SEMANTIC: ·                        ─
// SEMANTIC: 5 │   a ? static_assert(sizeof(int) == 2) : (void) 0;
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × static assertion failed
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/static_assert_expression_failed.c:5:21]
// SEMANTIC: 4 │   (void) static_assert(0, "cast");
// SEMANTIC: 5 │   a ? static_assert(sizeof(int) == 2) : (void) 0;
// SEMANTIC: ·                     ────────────────
// SEMANTIC: 6 │   extern typeof(static_assert(0, "typeof")) g(void);
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × static assertion failed: typeof
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/static_assert_expression_failed.c:6:31]
// SEMANTIC: 5 │   a ? static_assert(sizeof(int) == 2) : (void) 0;
// SEMANTIC: 6 │   extern typeof(static_assert(0, "typeof")) g(void);
// SEMANTIC: ·                               ─
// SEMANTIC: 7 │   for (static_assert(0, "for");;)
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × static assertion failed: for
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/static_assert_expression_failed.c:7:22]
// SEMANTIC: 6 │   extern typeof(static_assert(0, "typeof")) g(void);
// SEMANTIC: 7 │   for (static_assert(0, "for");;)
// SEMANTIC: ·                      ─
// SEMANTIC: 8 │     break;
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
