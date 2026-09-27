int *__ptr32 __sptr __uptr both;

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × invalid in this context: '__sptr' and '__uptr' attributes are not
// SLATE-FILECHECK-END SEMANTIC
