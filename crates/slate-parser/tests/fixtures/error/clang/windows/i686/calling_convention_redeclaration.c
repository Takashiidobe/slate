// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

int g(int);
int __stdcall g(int a) { return a; }

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × function redeclared with a different calling convention
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/i686/calling_convention_redeclaration.c:3:1]
// SEMANTIC: 2 │ int g(int);
// SEMANTIC: 3 │ int __stdcall g(int a) { return a; }
// SEMANTIC: · ────────────────────────────────────
// SEMANTIC: 4 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
