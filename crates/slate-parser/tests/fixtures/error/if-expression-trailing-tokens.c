#if 1 2
int value;
#endif

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid #if expression: unexpected tokens after expression
// PARSE: ╰─▶ invalid #if expression: unexpected tokens after expression
// PARSE: ╭─[tests/fixtures/error/if-expression-trailing-tokens.c:1:7]
// PARSE: 1 │ #if 1 2
// PARSE: ·       ─
// PARSE: 2 │ int value;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
