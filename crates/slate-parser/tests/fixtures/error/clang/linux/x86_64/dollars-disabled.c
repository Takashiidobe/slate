// SLATE-FILECHECK-ARGS -fno-dollars-in-identifiers
// SLATE-FILECHECK-ERROR PARSE

int a$b;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `;`
// PARSE:   ╰─▶ expected `;`
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/dollars-disabled.c:2:6]
// PARSE:  1 │
// PARSE:  2 │ int a$b;
// PARSE:    ·      ─
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
