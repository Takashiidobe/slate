// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

void f(int n, ...) {
  char *ap;
  __builtin_ms_va_start(ap, n);
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × this builtin is only available on x86-64 and aarch64 targets
// IR:    ╭─[tests/fixtures/error/clang/linux/i686/ms_va_start_unsupported_target.c:4:3]
// IR:  3 │   char *ap;
// IR:  4 │   __builtin_ms_va_start(ap, n);
// IR:    ·   ────────────────────────────
// IR:  5 │ }
// IR:    ╰────
// SLATE-FILECHECK-END IR
