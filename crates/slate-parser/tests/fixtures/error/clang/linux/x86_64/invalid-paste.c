// SLATE-FILECHECK-ERROR PARSE

#define PASTE(a, b) a##b
int x = PASTE(+, ;) 1;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × pasting formed '+;', an invalid preprocessing token
// PARSE: ╰─▶ pasting formed '+;', an invalid preprocessing token
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/invalid-paste.c:3:15]
// PARSE: 2 │ #define PASTE(a, b) a##b
// PARSE: 3 │ int x = PASTE(+, ;) 1;
// PARSE: ·               ────
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
