int invalid = 1wbuq;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid floating literal `1wbuq`
// DEFAULT: ╭─[tests/fixtures/invalid-integer-literal-suffix.c:1:15]
// DEFAULT: 1 │ int invalid = 1wbuq;
// DEFAULT: ·               ─────
// DEFAULT: 2 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
