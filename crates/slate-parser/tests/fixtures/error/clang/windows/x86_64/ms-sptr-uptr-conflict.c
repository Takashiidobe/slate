int *__ptr32 __sptr __uptr both;

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × '__sptr' and '__uptr' attributes are not compatible
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/ms-sptr-uptr-conflict.c:1:1]
// SEMANTIC: 1 │ int *__ptr32 __sptr __uptr both;
// SEMANTIC: · ────────────────────────────────
// SEMANTIC: 2 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
