// SLATE-FILECHECK-DEFINES WIDENED WIDENED
// SLATE-FILECHECK-ERROR WIDENED
// SLATE-FILECHECK-DEFINES NESTED_SIGN NESTED_SIGN
// SLATE-FILECHECK-ERROR NESTED_SIGN
// SLATE-FILECHECK-DEFINES UNRELATED UNRELATED
// SLATE-FILECHECK-ERROR UNRELATED
// SLATE-FILECHECK-DEFINES FROM_POINTER FROM_POINTER
// SLATE-FILECHECK-ERROR FROM_POINTER
// SLATE-FILECHECK-DEFINES TO_POINTER TO_POINTER
// SLATE-FILECHECK-ERROR TO_POINTER
// SLATE-FILECHECK-DEFINES ATOMIC ATOMIC
// SLATE-FILECHECK-ERROR ATOMIC
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

// -Wincompatible-pointer-types and -Wint-conversion are default errors in
// clang 22.1.8 and gcc 16.2.1 (gcc demotes them to warnings in c89/gnu89
// only). Measured 2026-09-21; see wiki/concepts/diagnostic-severity.md.
#if defined(WIDENED)
long *widened(unsigned *value) { return value; }
#elif defined(NESTED_SIGN)
int **nested_sign(unsigned **value) { return value; }
#elif defined(UNRELATED)
struct A {
    int a;
};
struct B {
    int b;
};
struct B *unrelated(struct A *value) { return value; }
#elif defined(FROM_POINTER)
int from_pointer(int *value) { return value; }
#elif defined(TO_POINTER)
int *to_pointer(int value) { return value; }
#elif defined(ATOMIC)
int *dropped(_Atomic int *value) { return value; }
#endif

// SLATE-FILECHECK-BEGIN WIDENED
// WIDENED: Error:   × semantic analysis failed
// WIDENED: Error: -Wincompatible-pointer-types
// WIDENED: × incompatible pointer types
// WIDENED: ╭─[tests/fixtures/error/conversion_default_errors.c:6:41]
// WIDENED: 5 │ #if defined(WIDENED)
// WIDENED: 6 │ long *widened(unsigned *value) { return value; }
// WIDENED: ·                                         ─────
// WIDENED: 7 │ #elif defined(NESTED_SIGN)
// WIDENED: ╰────
// SLATE-FILECHECK-END WIDENED
// SLATE-FILECHECK-BEGIN NESTED_SIGN
// NESTED_SIGN: Error:   × semantic analysis failed
// NESTED_SIGN: Error: -Wincompatible-pointer-types
// NESTED_SIGN: × incompatible pointer types
// NESTED_SIGN: ╭─[tests/fixtures/error/conversion_default_errors.c:8:46]
// NESTED_SIGN: 7 │ #elif defined(NESTED_SIGN)
// NESTED_SIGN: 8 │ int **nested_sign(unsigned **value) { return value; }
// NESTED_SIGN: ·                                              ─────
// NESTED_SIGN: 9 │ #elif defined(UNRELATED)
// NESTED_SIGN: ╰────
// SLATE-FILECHECK-END NESTED_SIGN
// SLATE-FILECHECK-BEGIN UNRELATED
// UNRELATED: Error:   × semantic analysis failed
// UNRELATED: Error: -Wincompatible-pointer-types
// UNRELATED: × incompatible pointer types
// UNRELATED: ╭─[tests/fixtures/error/conversion_default_errors.c:16:47]
// UNRELATED: 15 │ };
// UNRELATED: 16 │ struct B *unrelated(struct A *value) { return value; }
// UNRELATED: ·                                               ─────
// UNRELATED: 17 │ #elif defined(FROM_POINTER)
// UNRELATED: ╰────
// SLATE-FILECHECK-END UNRELATED
// SLATE-FILECHECK-BEGIN FROM_POINTER
// FROM_POINTER: Error:   × semantic analysis failed
// FROM_POINTER: Error: -Wint-conversion
// FROM_POINTER: × incompatible pointer to integer conversion
// FROM_POINTER: ╭─[tests/fixtures/error/conversion_default_errors.c:18:39]
// FROM_POINTER: 17 │ #elif defined(FROM_POINTER)
// FROM_POINTER: 18 │ int from_pointer(int *value) { return value; }
// FROM_POINTER: ·                                       ─────
// FROM_POINTER: 19 │ #elif defined(TO_POINTER)
// FROM_POINTER: ╰────
// SLATE-FILECHECK-END FROM_POINTER
// SLATE-FILECHECK-BEGIN TO_POINTER
// TO_POINTER: Error:   × semantic analysis failed
// TO_POINTER: Error: -Wint-conversion
// TO_POINTER: × incompatible integer to pointer conversion
// TO_POINTER: ╭─[tests/fixtures/error/conversion_default_errors.c:20:37]
// TO_POINTER: 19 │ #elif defined(TO_POINTER)
// TO_POINTER: 20 │ int *to_pointer(int value) { return value; }
// TO_POINTER: ·                                     ─────
// TO_POINTER: 21 │ #elif defined(ATOMIC)
// TO_POINTER: ╰────
// SLATE-FILECHECK-END TO_POINTER
// SLATE-FILECHECK-BEGIN ATOMIC
// ATOMIC: Error:   × semantic analysis failed
// ATOMIC: Error: -Wincompatible-pointer-types
// ATOMIC: × pointer conversion drops _Atomic from the pointee
// ATOMIC: ╭─[tests/fixtures/error/conversion_default_errors.c:22:43]
// ATOMIC: 21 │ #elif defined(ATOMIC)
// ATOMIC: 22 │ int *dropped(_Atomic int *value) { return value; }
// ATOMIC: ·                                           ─────
// ATOMIC: 23 │ #endif
// ATOMIC: ╰────
// SLATE-FILECHECK-END ATOMIC
