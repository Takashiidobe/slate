#if 1
#error unsupported target
#endif
int value;

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × #error unsupported target
// PARSE: ╭─[tests/fixtures/error-directive.c:2:1]
// PARSE: 1 │ #if 1
// PARSE: 2 │ #error unsupported target
// PARSE: · ─────────────────────────
// PARSE: 3 │ #endif
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
