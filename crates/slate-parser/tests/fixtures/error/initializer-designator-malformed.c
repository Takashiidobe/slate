int values[] = { . = 1 };

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × expected identifier
// DEFAULT: ╰─▶ expected identifier
// DEFAULT: ╭─[tests/fixtures/error/initializer-designator-malformed.c:1:20]
// DEFAULT: 1 │ int values[] = { . = 1 };
// DEFAULT: ·                    ─
// DEFAULT: 2 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
