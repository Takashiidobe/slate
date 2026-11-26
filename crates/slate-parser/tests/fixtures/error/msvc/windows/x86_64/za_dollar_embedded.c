int em$bedded;

// SLATE-FILECHECK-ARGS /Za
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × character '$' is not permitted in an identifier with /Za
// PARSE:   ╰─▶ character '$' is not permitted in an identifier with /Za
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_dollar_embedded.c:1:7]
// PARSE:  1 │ int em$bedded;
// PARSE:    ·       ─
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
