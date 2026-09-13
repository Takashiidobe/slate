asm volatile("top");

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × meaningless `volatile` on asm outside function
// PARSE: ╰─▶ meaningless `volatile` on asm outside function
// PARSE: ╭─[tests/fixtures/asm-file-scope-qualifier.c:1:5]
// PARSE: 1 │ asm volatile("top");
// PARSE: ·     ────────
// PARSE: 2 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
