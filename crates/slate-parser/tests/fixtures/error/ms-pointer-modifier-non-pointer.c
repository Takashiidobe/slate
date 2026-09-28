int __ptr32 not_a_pointer;

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: __ptr32, __ptr64, __sptr and __uptr only apply to
// SEMANTIC: ╭─[tests/fixtures/error/ms-pointer-modifier-non-pointer.c:1:1]
// SEMANTIC: 1 │ int __ptr32 not_a_pointer;
// SEMANTIC: · ──────────────────────────
// SEMANTIC: 2 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
