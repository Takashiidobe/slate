#define INCOMPLETE 1 +
#if INCOMPLETE
int value;
#endif

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid #if expression: unexpected token `Plus`
// PARSE: ╰─▶ invalid #if expression: unexpected token `Plus`
// PARSE: ╭─[tests/fixtures/error/if-expression-macro-invalid.c:2:5]
// PARSE: 1 │ #define INCOMPLETE 1 +
// PARSE: 2 │ #if INCOMPLETE
// PARSE: ·     ──────────
// PARSE: 3 │ int value;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
