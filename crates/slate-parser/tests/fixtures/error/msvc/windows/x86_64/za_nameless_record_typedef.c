typedef struct { int x; } Member;
struct S { Member; };

// SLATE-FILECHECK-ARGS /Za
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × nameless struct or union member is not allowed with /Za
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_nameless_record_typedef.c:2:1]
// PARSE:  1 │ typedef struct { int x; } Member;
// PARSE:  2 │ struct S { Member; };
// PARSE:    · ─────────────────────
// PARSE:  3 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
