// SLATE-FILECHECK-ARGS -fblocks
// SLATE-FILECHECK-ERROR SEMANTIC

typedef __builtin_va_list va_list;

int outer(int n, ...) {
  int (^f)(void) = ^{
    va_list ap;
    __builtin_va_start(ap, n);
    __builtin_va_end(ap);
    return 0;
  };
  return f();
}

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC:   × 'va_start' used in function with fixed args
// SEMANTIC:    ╭─[tests/fixtures/error/clang/linux/x86_64/block_va_start_fixed.c:7:5]
// SEMANTIC:  6 │     va_list ap;
// SEMANTIC:  7 │     __builtin_va_start(ap, n);
// SEMANTIC:    ·     ─────────────────────────
// SEMANTIC:  8 │     __builtin_va_end(ap);
// SEMANTIC:    ╰────
// SLATE-FILECHECK-END SEMANTIC
