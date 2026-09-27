int __ptr32 not_a_pointer;

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × invalid in this context: __ptr32, __ptr64, __sptr and __uptr only apply to
// SLATE-FILECHECK-END SEMANTIC
