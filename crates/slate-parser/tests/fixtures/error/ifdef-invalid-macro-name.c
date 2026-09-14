#ifdef 123
int value;
#endif

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected macro name after #ifdef
// PARSE: ╰─▶ expected macro name after #ifdef
// PARSE: ╭─[tests/fixtures/error/ifdef-invalid-macro-name.c:1:8]
// PARSE: 1 │ #ifdef 123
// PARSE: ·        ───
// PARSE: 2 │ int value;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
