#define name$ 1

// SLATE-FILECHECK-ARGS /Za
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × character '$' is not permitted in an identifier with /Za
// PARSE:   ╰─▶ character '$' is not permitted in an identifier with /Za
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_dollar_macro_name.c:1:13]
// PARSE:  1 │ #define name$ 1
// PARSE:    ·             ─
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
