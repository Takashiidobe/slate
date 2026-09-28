int *__ptr32 __ptr64 both;

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: '__ptr32' and '__ptr64' attributes are not
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/ms-ptr32-ptr64-conflict.c:1:1]
// SEMANTIC: 1 │ int *__ptr32 __ptr64 both;
// SEMANTIC: · ──────────────────────────
// SEMANTIC: 2 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
