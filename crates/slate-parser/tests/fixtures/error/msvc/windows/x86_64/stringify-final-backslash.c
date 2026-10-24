// SLATE-FILECHECK-ERROR PARSE

#define STR(x) #x
const char *final_backslash = STR(\);

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × newline in string literal
// PARSE:   ╰─▶ newline in string literal
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/stringify-final-backslash.c:2:16]
// PARSE:  1 │
// PARSE:  2 │ #define STR(x) #x
// PARSE:    ·                ─
// PARSE:  3 │ const char *final_backslash = STR(\);
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
