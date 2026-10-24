size_t length(const char *s);
// SLATE-FILECHECK-ARGS -fno-ms-compatibility
// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × semantic analysis failed
// ERROR: Error:
// ERROR:   × unknown type name `size_t`
// ERROR:    ╭─[tests/fixtures/error/clang/windows/x86_64/ms_no_compatibility_size_t.c:1:1]
// ERROR:  1 │ size_t length(const char *s);
// ERROR:    · ──────
// ERROR:  2 │
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
