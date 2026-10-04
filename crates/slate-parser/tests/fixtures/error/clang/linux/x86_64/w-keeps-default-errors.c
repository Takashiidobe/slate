int *pointer;
void assign(long value) { pointer = value; }

// SLATE-FILECHECK-ARGS -w
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error: -Wint-conversion
// PARSE: × incompatible integer to pointer conversion
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/w-keeps-default-errors.c:2:37]
// PARSE: 1 │ int *pointer;
// PARSE: 2 │ void assign(long value) { pointer = value; }
// PARSE: ·                                     ─────
// PARSE: 3 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
