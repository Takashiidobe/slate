int g(void) { return _Alignof(; }

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `;`
// PARSE: ╰─▶ expected `;`
// PARSE: ╭─[tests/fixtures/error/alignof-malformed.c:1:1]
// PARSE: 1 │ ╭─▶ int g(void) { return _Alignof(; }
// PARSE: 2 │ │
// PARSE: 3 │ ╰─▶
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
