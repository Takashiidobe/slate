// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-DEFINES C89 C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-WARNING C89
// SLATE-FILECHECK-DEFINES GNU89 GNU89
// SLATE-FILECHECK-STD GNU89 gnu89
// SLATE-FILECHECK-WARNING GNU89
// SLATE-FILECHECK-ARGS --dump-ir

// gcc 16.2.1 demotes both default errors to warnings in c89/gnu89 only; c99
// and later error. Measured 2026-09-21; the c99 half is in
// error/conversion_default_errors_gcc.c.
#if defined(C89) || defined(GNU89)
int *to_pointer(int value) { return value; }
#endif

// SLATE-FILECHECK-BEGIN C89
// C89: -Wint-conversion
// C89: ⚠ incompatible integer to pointer conversion
// C89: ╭─[tests/fixtures/sema/conversion_severity_gcc.c:6:37]
// C89: 5 │ #if defined(C89) || defined(GNU89)
// C89: 6 │ int *to_pointer(int value) { return value; }
// C89: ·                                     ─────
// C89: 7 │ #endif
// C89: ╰────
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN GNU89
// GNU89: -Wint-conversion
// GNU89: ⚠ incompatible integer to pointer conversion
// GNU89: ╭─[tests/fixtures/sema/conversion_severity_gcc.c:6:37]
// GNU89: 5 │ #if defined(C89) || defined(GNU89)
// GNU89: 6 │ int *to_pointer(int value) { return value; }
// GNU89: ·                                     ─────
// GNU89: 7 │ #endif
// GNU89: ╰────
// SLATE-FILECHECK-END GNU89
