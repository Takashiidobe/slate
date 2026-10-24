// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

void __attribute__((ms_abi)) f(int n, ...) {
  __builtin_va_list ap;
  __builtin_va_start(ap, n);
  __builtin_va_end(ap);
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × 'va_start' used in Win64 ABI function
// IR:    ╭─[tests/fixtures/error/clang/linux/x86_64/va_start_win64_function.c:4:3]
// IR:  3 │   __builtin_va_list ap;
// IR:  4 │   __builtin_va_start(ap, n);
// IR:    ·   ─────────────────────────
// IR:  5 │   __builtin_va_end(ap);
// IR:    ╰────
// SLATE-FILECHECK-END IR
