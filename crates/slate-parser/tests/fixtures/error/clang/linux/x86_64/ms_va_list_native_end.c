// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

void __attribute__((ms_abi)) f(int n, ...) {
  __builtin_ms_va_list ap;
  __builtin_ms_va_start(ap, n);
  __builtin_va_end(ap);
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × va builtin on non-va_list
// IR:    ╭─[tests/fixtures/error/clang/linux/x86_64/ms_va_list_native_end.c:5:3]
// IR:  4 │   __builtin_ms_va_start(ap, n);
// IR:  5 │   __builtin_va_end(ap);
// IR:    ·   ────────────────────
// IR:  6 │ }
// IR:    ╰────
// SLATE-FILECHECK-END IR
