int values[] = { 1 2 };

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × expected `}`, found `2`
// DEFAULT: ╰─▶ expected `}`, found `2`
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/initializer-comma-malformed.c:1:16]
// DEFAULT: 1 │ int values[] = { 1 2 };
// DEFAULT: ·                ─
// DEFAULT: 2 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
