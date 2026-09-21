// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-DEFINES C99 C99
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-ERROR C99
// SLATE-FILECHECK-DEFINES C17 C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-ERROR C17
// SLATE-FILECHECK-ARGS --dump-ir

// The other half of sema/conversion_severity_gcc.c: gcc errors from c99 on.
#if defined(C99) || defined(C17)
int *to_pointer(int value) { return value; }
#endif

// SLATE-FILECHECK-BEGIN C99
// C99: Error:   × semantic analysis failed
// C99: Error: -Wint-conversion
// C99: × incompatible integer to pointer conversion
// C99: ╭─[tests/fixtures/error/conversion_default_errors_gcc.c:4:37]
// C99: 3 │ #if defined(C99) || defined(C17)
// C99: 4 │ int *to_pointer(int value) { return value; }
// C99: ·                                     ─────
// C99: 5 │ #endif
// C99: ╰────
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C17
// C17: Error:   × semantic analysis failed
// C17: Error: -Wint-conversion
// C17: × incompatible integer to pointer conversion
// C17: ╭─[tests/fixtures/error/conversion_default_errors_gcc.c:4:37]
// C17: 3 │ #if defined(C99) || defined(C17)
// C17: 4 │ int *to_pointer(int value) { return value; }
// C17: ·                                     ─────
// C17: 5 │ #endif
// C17: ╰────
// SLATE-FILECHECK-END C17
