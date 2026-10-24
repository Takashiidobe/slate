int narrow = 300i8;
int word = 4294967296i32;
int byte = 256ui8;

// SLATE-FILECHECK-ERROR ERROR

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × semantic analysis failed
// ERROR: Error:
// ERROR:   × integer literal `300i8` is too large to be represented in any integer type
// ERROR:    ╭─[tests/fixtures/error/clang/windows/x86_64/microsoft_integer_suffix_range.c:1:14]
// ERROR:  1 │ int narrow = 300i8;
// ERROR:    ·              ─────
// ERROR:  2 │ int word = 4294967296i32;
// ERROR:    ╰────
// ERROR: Error:
// ERROR:   × integer literal `4294967296i32` is too large to be represented in any
// ERROR:    ╭─[tests/fixtures/error/clang/windows/x86_64/microsoft_integer_suffix_range.c:2:12]
// ERROR:  1 │ int narrow = 300i8;
// ERROR:  2 │ int word = 4294967296i32;
// ERROR:    ·            ─────────────
// ERROR:  3 │ int byte = 256ui8;
// ERROR:    ╰────
// ERROR: Error:
// ERROR:   × integer literal `256ui8` is too large to be represented in any integer
// ERROR:    ╭─[tests/fixtures/error/clang/windows/x86_64/microsoft_integer_suffix_range.c:3:12]
// ERROR:  2 │ int word = 4294967296i32;
// ERROR:  3 │ int byte = 256ui8;
// ERROR:    ·            ──────
// ERROR:  4 │
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
