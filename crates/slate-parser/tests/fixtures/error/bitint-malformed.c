_BitInt 3 x;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × expected `LParen` after `_BitInt`
// DEFAULT: ╰─▶ expected `LParen` after `_BitInt`
// DEFAULT: ╭─[tests/fixtures/error/bitint-malformed.c:1:9]
// DEFAULT: 1 │ _BitInt 3 x;
// DEFAULT: ·         ─
// DEFAULT: 2 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
