struct S;

void h(void) { struct S s; }

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × variable has incomplete type
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/incomplete-local-object.c:3:25]
// PARSE:  2 │
// PARSE:  3 │ void h(void) { struct S s; }
// PARSE:    ·                         ─
// PARSE:  4 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
