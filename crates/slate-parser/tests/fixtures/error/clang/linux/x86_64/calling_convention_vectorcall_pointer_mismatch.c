// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

int __vectorcall f(int);
int (*q)(int) = f;

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error: -Wincompatible-pointer-types
// SEMANTIC: × incompatible pointer types
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/calling_convention_vectorcall_pointer_mismatch.c:3:17]
// SEMANTIC: 2 │ int __vectorcall f(int);
// SEMANTIC: 3 │ int (*q)(int) = f;
// SEMANTIC: ·                 ─
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
