int x;
asm("%0" : : "r"(x));

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `)`
// PARSE: ╰─▶ expected `)`
// PARSE: ╭─[tests/fixtures/asm-file-scope-operands.c:2:10]
// PARSE: 1 │ int x;
// PARSE: 2 │ asm("%0" : : "r"(x));
// PARSE: ·          ─
// PARSE: 3 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
