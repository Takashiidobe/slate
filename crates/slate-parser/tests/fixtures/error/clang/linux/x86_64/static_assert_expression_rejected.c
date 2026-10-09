// SLATE-FILECHECK-ARGS --dump-ir -std=c2y
// SLATE-FILECHECK-ERROR SEMANTIC

void f(void) {
  (void) static_assert(1, "clang has no static assertion expressions");
}

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × unresolved ordinary name `_Static_assert`
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/static_assert_expression_rejected.c:3:10]
// SEMANTIC: 2 │ void f(void) {
// SEMANTIC: 3 │   (void) static_assert(1, "clang has no static assertion expressions");
// SEMANTIC: ·          ─────────────
// SEMANTIC: 4 │ }
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
