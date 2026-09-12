#define PAIR(first, second
int value;

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `)` after macro parameters
// PARSE: ╰─▶ expected `)` after macro parameters
// PARSE: ╭─[tests/fixtures/define-unclosed-parameters.c:1:13]
// PARSE: 1 │ #define PAIR(first, second
// PARSE: ·             ─
// PARSE: 2 │ int value;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
