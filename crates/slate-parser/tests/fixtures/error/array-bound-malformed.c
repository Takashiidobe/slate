int values[1+];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unexpected token `Plus`
// DEFAULT: ╰─▶ unexpected token `Plus`
// DEFAULT: ╭─[tests/fixtures/error/array-bound-malformed.c:1:14]
// DEFAULT: 1 │ int values[1+];
// DEFAULT: ·              ─
// DEFAULT: 2 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
