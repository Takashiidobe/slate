// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

void f(__builtin_ms_va_list ap);

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × unknown type name '__builtin_ms_va_list'
// IR: ╭─[tests/fixtures/error/clang/linux/i686/ms_va_list_unknown_type.c:2:1]
// IR: 1 │
// IR: 2 │ void f(__builtin_ms_va_list ap);
// IR: · ────────────────────────────────
// IR: ╰────
// SLATE-FILECHECK-END IR
