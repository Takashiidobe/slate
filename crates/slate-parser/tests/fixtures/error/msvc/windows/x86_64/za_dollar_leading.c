int $leading;

// SLATE-FILECHECK-ARGS /Za
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × character '$' is not permitted in an identifier with /Za
// PARSE:   ╰─▶ character '$' is not permitted in an identifier with /Za
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_dollar_leading.c:1:5]
// PARSE:  1 │ int $leading;
// PARSE:    ·     ─
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
