static extern int invalid_storage;

// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × multiple storage classes
// DEFAULT: ╰─▶ multiple storage classes
// DEFAULT: ╭─[tests/fixtures/error/qualifiers_errors.c:1:8]
// DEFAULT: 1 │ static extern int invalid_storage;
// DEFAULT: ·        ──────
// DEFAULT: 2 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
