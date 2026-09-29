// SLATE-FILECHECK-DEFINES OBJECT OBJECT
// SLATE-FILECHECK-ERROR OBJECT
// SLATE-FILECHECK-DEFINES BOUNDS BOUNDS
// SLATE-FILECHECK-ERROR BOUNDS
// SLATE-FILECHECK-DEFINES POINTER POINTER
// SLATE-FILECHECK-ERROR POINTER

typedef __SIZE_TYPE__ size_t;
void close_at(int, void *);
int object;

#ifdef OBJECT
void *allocate(size_t) __attribute__((malloc(object)));
#endif

#ifdef BOUNDS
void *allocate(size_t) __attribute__((malloc(close_at, 3)));
#endif

#ifdef POINTER
void *allocate(size_t) __attribute__((malloc(close_at, 1)));
#endif

// SLATE-FILECHECK-BEGIN OBJECT
// OBJECT: Error:   × semantic analysis failed
// OBJECT: Error:
// OBJECT: × 'malloc' argument is not a function
// OBJECT: ╭─[tests/fixtures/error/clang/linux/x86_64/malloc_deallocator_invalid.c:7:46]
// OBJECT: 6 │ #ifdef OBJECT
// OBJECT: 7 │ void *allocate(size_t) __attribute__((malloc(object)));
// OBJECT: ·                                              ──────
// OBJECT: 8 │ #endif
// OBJECT: ╰────
// SLATE-FILECHECK-END OBJECT
// SLATE-FILECHECK-BEGIN BOUNDS
// BOUNDS: Error:   × semantic analysis failed
// BOUNDS: Error:
// BOUNDS: × 'malloc' attribute parameter is out of bounds
// BOUNDS: ╭─[tests/fixtures/error/clang/linux/x86_64/malloc_deallocator_invalid.c:11:46]
// BOUNDS: 10 │ #ifdef BOUNDS
// BOUNDS: 11 │ void *allocate(size_t) __attribute__((malloc(close_at, 3)));
// BOUNDS: ·                                              ────────
// BOUNDS: 12 │ #endif
// BOUNDS: ╰────
// SLATE-FILECHECK-END BOUNDS
// SLATE-FILECHECK-BEGIN POINTER
// POINTER: Error:   × semantic analysis failed
// POINTER: Error:
// POINTER: × 'malloc' argument refers to a non-pointer parameter
// POINTER: ╭─[tests/fixtures/error/clang/linux/x86_64/malloc_deallocator_invalid.c:15:46]
// POINTER: 14 │ #ifdef POINTER
// POINTER: 15 │ void *allocate(size_t) __attribute__((malloc(close_at, 1)));
// POINTER: ·                                              ────────
// POINTER: 16 │ #endif
// POINTER: ╰────
// SLATE-FILECHECK-END POINTER
