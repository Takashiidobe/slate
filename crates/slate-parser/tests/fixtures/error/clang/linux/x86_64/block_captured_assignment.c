// SLATE-FILECHECK-ARGS -fblocks
// SLATE-FILECHECK-ERROR SEMANTIC

int captured(void) {
  int x = 0;
  int (^f)(void) = ^{
    x = 1;
    return x;
  };
  return f();
}

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC:   × variable is not assignable (missing __block type specifier)
// SEMANTIC:    ╭─[tests/fixtures/error/clang/linux/x86_64/block_captured_assignment.c:5:5]
// SEMANTIC:  4 │   int (^f)(void) = ^{
// SEMANTIC:  5 │     x = 1;
// SEMANTIC:    ·     ─
// SEMANTIC:  6 │     return x;
// SEMANTIC:    ╰────
// SLATE-FILECHECK-END SEMANTIC
