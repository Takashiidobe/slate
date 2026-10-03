// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

struct s {
  int a;
  _Static_assert (sizeof (struct s) > 0, "incomplete");
  _Static_assert (0, "member");
};

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × static assertion requires an integer constant expression: sizeof of
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/static_assert_member_failed.c:4:19]
// SEMANTIC: 3 │   int a;
// SEMANTIC: 4 │   _Static_assert (sizeof (struct s) > 0, "incomplete");
// SEMANTIC: ·                   ─────────────────────
// SEMANTIC: 5 │   _Static_assert (0, "member");
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × static assertion failed: member
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/static_assert_member_failed.c:5:19]
// SEMANTIC: 4 │   _Static_assert (sizeof (struct s) > 0, "incomplete");
// SEMANTIC: 5 │   _Static_assert (0, "member");
// SEMANTIC: ·                   ─
// SEMANTIC: 6 │ };
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
