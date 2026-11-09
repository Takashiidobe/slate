// SLATE-FILECHECK-ERROR PARSE

_Static_assert(sizeof(int) == 4, "int");

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × a type specifier is required for all declarations
// PARSE:   ╰─▶ a type specifier is required for all declarations
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/static-assert-without-std-c11.c:2:1]
// PARSE:  1 │
// PARSE:  2 │ _Static_assert(sizeof(int) == 4, "int");
// PARSE:    · ──────────────
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
