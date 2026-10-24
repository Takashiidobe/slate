long long value = 1i64;

// SLATE-FILECHECK-ERROR ERROR

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × semantic analysis failed
// ERROR: Error:
// ERROR:   × invalid floating literal `1i64`
// ERROR:    ╭─[tests/fixtures/error/clang/linux/x86_64/microsoft_integer_suffix.c:1:19]
// ERROR:  1 │ long long value = 1i64;
// ERROR:    ·                   ────
// ERROR:  2 │
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
