// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

void f(int n) {
  __builtin_va_list ap;
  __builtin_va_start(ap, n);
  __builtin_va_end(ap);
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × 'va_start' used in function with fixed args
// IR:    ╭─[tests/fixtures/error/clang/linux/x86_64/va_start_fixed_args.c:4:3]
// IR:  3 │   __builtin_va_list ap;
// IR:  4 │   __builtin_va_start(ap, n);
// IR:    ·   ─────────────────────────
// IR:  5 │   __builtin_va_end(ap);
// IR:    ╰────
// SLATE-FILECHECK-END IR
