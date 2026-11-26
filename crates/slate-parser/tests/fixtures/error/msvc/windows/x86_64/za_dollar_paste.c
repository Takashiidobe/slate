#define CAT(a, b) a ## b
int CAT(name, $);

// SLATE-FILECHECK-ARGS /Za
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × character '$' is not permitted in an identifier with /Za
// PARSE:   ╰─▶ character '$' is not permitted in an identifier with /Za
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_dollar_paste.c:2:15]
// PARSE:  1 │ #define CAT(a, b) a ## b
// PARSE:  2 │ int CAT(name, $);
// PARSE:    ·               ─
// PARSE:  3 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
