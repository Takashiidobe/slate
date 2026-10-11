// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

void f(int n, ...) {
  __builtin_ms_va_list ap;
  __builtin_ms_va_start(ap, n);
  __builtin_ms_va_end(ap);
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × '__builtin_ms_va_start' used in System V ABI function
// IR: ╭─[tests/fixtures/error/clang/linux/aarch64/ms_va_start_sysv_function.c:4:3]
// IR: 3 │   __builtin_ms_va_list ap;
// IR: 4 │   __builtin_ms_va_start(ap, n);
// IR: ·   ────────────────────────────
// IR: 5 │   __builtin_ms_va_end(ap);
// IR: ╰────
// SLATE-FILECHECK-END IR
