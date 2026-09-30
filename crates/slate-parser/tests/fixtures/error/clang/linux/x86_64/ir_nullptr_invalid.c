// SLATE-FILECHECK-ARGS --dump-ir -std=c23
// SLATE-FILECHECK-DEFINES TO_INTEGER TO_INTEGER
// SLATE-FILECHECK-DEFINES FROM_POINTER FROM_POINTER
// SLATE-FILECHECK-DEFINES CAST_FROM_POINTER CAST_FROM_POINTER
// SLATE-FILECHECK-DEFINES ARITHMETIC ARITHMETIC
// SLATE-FILECHECK-ERROR TO_INTEGER
// SLATE-FILECHECK-ERROR FROM_POINTER
// SLATE-FILECHECK-ERROR CAST_FROM_POINTER
// SLATE-FILECHECK-ERROR ARITHMETIC

typedef typeof(nullptr) nullptr_t;

#ifdef TO_INTEGER
long to_integer(nullptr_t n) { return (long)n; }
#endif
#ifdef FROM_POINTER
nullptr_t from_pointer(int *p) { return p; }
#endif
#ifdef CAST_FROM_POINTER
nullptr_t cast_from_pointer(void *p) { return (nullptr_t)p; }
#endif
#ifdef ARITHMETIC
int arithmetic(nullptr_t n) { return n + 1; }
#endif

// SLATE-FILECHECK-BEGIN TO_INTEGER
// TO_INTEGER: Error:   × semantic analysis failed
// TO_INTEGER: Error:
// TO_INTEGER: × conversion from nullptr_t to a type other than bool or a pointer
// TO_INTEGER: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_nullptr_invalid.c:5:39]
// TO_INTEGER: 4 │ #ifdef TO_INTEGER
// TO_INTEGER: 5 │ long to_integer(nullptr_t n) { return (long)n; }
// TO_INTEGER: ·                                       ───────
// TO_INTEGER: 6 │ #endif
// TO_INTEGER: ╰────
// SLATE-FILECHECK-END TO_INTEGER
// SLATE-FILECHECK-BEGIN FROM_POINTER
// FROM_POINTER: Error:   × semantic analysis failed
// FROM_POINTER: Error:
// FROM_POINTER: × conversion to nullptr_t from a type other than nullptr_t
// FROM_POINTER: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_nullptr_invalid.c:8:41]
// FROM_POINTER: 7 │ #ifdef FROM_POINTER
// FROM_POINTER: 8 │ nullptr_t from_pointer(int *p) { return p; }
// FROM_POINTER: ·                                         ─
// FROM_POINTER: 9 │ #endif
// FROM_POINTER: ╰────
// SLATE-FILECHECK-END FROM_POINTER
// SLATE-FILECHECK-BEGIN CAST_FROM_POINTER
// CAST_FROM_POINTER: Error:   × semantic analysis failed
// CAST_FROM_POINTER: Error:
// CAST_FROM_POINTER: × conversion to nullptr_t from a type other than nullptr_t
// CAST_FROM_POINTER: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_nullptr_invalid.c:11:47]
// CAST_FROM_POINTER: 10 │ #ifdef CAST_FROM_POINTER
// CAST_FROM_POINTER: 11 │ nullptr_t cast_from_pointer(void *p) { return (nullptr_t)p; }
// CAST_FROM_POINTER: ·                                               ────────────
// CAST_FROM_POINTER: 12 │ #endif
// CAST_FROM_POINTER: ╰────
// SLATE-FILECHECK-END CAST_FROM_POINTER
// SLATE-FILECHECK-BEGIN ARITHMETIC
// ARITHMETIC: Error:   × semantic analysis failed
// ARITHMETIC: Error:
// ARITHMETIC: × non-arithmetic operand
// ARITHMETIC: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_nullptr_invalid.c:14:38]
// ARITHMETIC: 13 │ #ifdef ARITHMETIC
// ARITHMETIC: 14 │ int arithmetic(nullptr_t n) { return n + 1; }
// ARITHMETIC: ·                                      ─────
// ARITHMETIC: 15 │ #endif
// ARITHMETIC: ╰────
// SLATE-FILECHECK-END ARITHMETIC
