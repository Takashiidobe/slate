int values[] = { . = 1 };

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × unexpected token `Equal`
// SEMANTIC: ╰─▶ unexpected token `Equal`
// SEMANTIC: ╭─[tests/fixtures/clang/linux/x86_64/initializer-designator-malformed.c:1:16]
// SEMANTIC: 1 │ int values[] = { . = 1 };
// SEMANTIC: ·                ─
// SEMANTIC: 2 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
