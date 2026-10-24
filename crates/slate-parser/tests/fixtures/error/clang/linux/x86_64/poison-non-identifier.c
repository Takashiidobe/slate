#pragma GCC poison 123


// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × can only poison identifier tokens
// PARSE:   ╰─▶ can only poison identifier tokens
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/poison-non-identifier.c:1:20]
// PARSE:  1 │ #pragma GCC poison 123
// PARSE:    ·                    ───
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
