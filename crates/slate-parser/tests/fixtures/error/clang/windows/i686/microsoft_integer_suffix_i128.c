long long value = 1i128;

// SLATE-FILECHECK-ERROR ERROR

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × semantic analysis failed
// ERROR: Error:
// ERROR:   × invalid floating literal `1i128`
// ERROR:    ╭─[tests/fixtures/error/clang/windows/i686/microsoft_integer_suffix_i128.c:1:19]
// ERROR:  1 │ long long value = 1i128;
// ERROR:    ·                   ─────
// ERROR:  2 │
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
