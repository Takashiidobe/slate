int *__ptr32 __sptr __uptr both;

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: '__sptr' and '__uptr' attributes are not
// SEMANTIC: ╭─[tests/fixtures/error/ms-sptr-uptr-conflict.c:1:1]
// SEMANTIC: 1 │ int *__ptr32 __sptr __uptr both;
// SEMANTIC: · ────────────────────────────────
// SEMANTIC: 2 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
