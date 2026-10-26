int __cdecl WinMain(void *, void *, char *, int);
int WinMain(void *, void *, char *, int);

// SLATE-FILECHECK-ARGS /std:c11
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × function redeclared with a different calling convention
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/i686/entry_point_redeclared_cdecl.c:2:5]
// PARSE:  1 │ int __cdecl WinMain(void *, void *, char *, int);
// PARSE:  2 │ int WinMain(void *, void *, char *, int);
// PARSE:    ·     ────────────────────────────────────
// PARSE:  3 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
