int broken __attribute__((visibility(1)));

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid arguments for attribute `visibility`
// SEMANTIC: ╭─[tests/fixtures/error/malformed-attributes.c:1:1]
// SEMANTIC: 1 │ int broken __attribute__((visibility(1)));
// SEMANTIC: · ──────────────────────────────────────────
// SEMANTIC: 2 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
