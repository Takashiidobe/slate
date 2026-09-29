// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

int __stdcall f(int);
int (*q)(int) = f;

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error: -Wincompatible-pointer-types
// SEMANTIC: × incompatible pointer types
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/i686/calling_convention_pointer_mismatch.c:3:17]
// SEMANTIC: 2 │ int __stdcall f(int);
// SEMANTIC: 3 │ int (*q)(int) = f;
// SEMANTIC: ·                 ─
// SEMANTIC: 4 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
