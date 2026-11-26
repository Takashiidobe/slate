struct S { union { int x; }; };

// SLATE-FILECHECK-ARGS /Za
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × nameless struct or union member is not allowed with /Za
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_nameless_union.c:1:1]
// PARSE:  1 │ struct S { union { int x; }; };
// PARSE:    · ───────────────────────────────
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
