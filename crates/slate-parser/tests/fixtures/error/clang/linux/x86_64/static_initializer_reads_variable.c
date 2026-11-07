// SLATE-FILECHECK-DEFINES SCALE SCALE
// SLATE-FILECHECK-DEFINES NEGATE NEGATE
// SLATE-FILECHECK-DEFINES CAST CAST
// SLATE-FILECHECK-DEFINES OFFSET OFFSET
// SLATE-FILECHECK-DEFINES INDEX INDEX
// SLATE-FILECHECK-ERROR SCALE
// SLATE-FILECHECK-ERROR NEGATE
// SLATE-FILECHECK-ERROR CAST
// SLATE-FILECHECK-ERROR OFFSET
// SLATE-FILECHECK-ERROR INDEX
// SLATE-FILECHECK-ARGS --dump-ir

int x;

#ifdef SCALE
static unsigned long scaled = x * 8UL;
#endif

#ifdef NEGATE
static int negated = -x;
#endif

#ifdef CAST
static long cast = (long)x;
#endif

#ifdef OFFSET
struct S { int a; int b[4]; };
static unsigned long offset = __builtin_offsetof(struct S, b[x]);
#endif

#ifdef INDEX
static int arr[4];
static int *index = &arr[x];
#endif

// SLATE-FILECHECK-BEGIN SCALE
// SCALE: Error:   × semantic analysis failed
// SCALE: Error:
// SCALE:   × initializer element is not a compile-time constant: reads a variable
// SCALE:    ╭─[tests/fixtures/error/clang/linux/x86_64/static_initializer_reads_variable.c:5:31]
// SCALE:  4 │ #ifdef SCALE
// SCALE:  5 │ static unsigned long scaled = x * 8UL;
// SCALE:    ·                               ─
// SCALE:  6 │ #endif
// SCALE:    ╰────
// SLATE-FILECHECK-END SCALE
// SLATE-FILECHECK-BEGIN NEGATE
// NEGATE: Error:   × semantic analysis failed
// NEGATE: Error:
// NEGATE:   × initializer element is not a compile-time constant: reads a variable
// NEGATE:     ╭─[tests/fixtures/error/clang/linux/x86_64/static_initializer_reads_variable.c:9:23]
// NEGATE:   8 │ #ifdef NEGATE
// NEGATE:   9 │ static int negated = -x;
// NEGATE:     ·                       ─
// NEGATE:  10 │ #endif
// NEGATE:     ╰────
// SLATE-FILECHECK-END NEGATE
// SLATE-FILECHECK-BEGIN CAST
// CAST: Error:   × semantic analysis failed
// CAST: Error:
// CAST:   × initializer element is not a compile-time constant: reads a variable
// CAST:     ╭─[tests/fixtures/error/clang/linux/x86_64/static_initializer_reads_variable.c:13:26]
// CAST:  12 │ #ifdef CAST
// CAST:  13 │ static long cast = (long)x;
// CAST:     ·                          ─
// CAST:  14 │ #endif
// CAST:     ╰────
// SLATE-FILECHECK-END CAST
// SLATE-FILECHECK-BEGIN OFFSET
// OFFSET: Error:   × semantic analysis failed
// OFFSET: Error:
// OFFSET:   × initializer element is not a compile-time constant: reads a variable
// OFFSET:     ╭─[tests/fixtures/error/clang/linux/x86_64/static_initializer_reads_variable.c:18:62]
// OFFSET:  17 │ struct S { int a; int b[4]; };
// OFFSET:  18 │ static unsigned long offset = __builtin_offsetof(struct S, b[x]);
// OFFSET:     ·                                                              ─
// OFFSET:  19 │ #endif
// OFFSET:     ╰────
// SLATE-FILECHECK-END OFFSET
// SLATE-FILECHECK-BEGIN INDEX
// INDEX: Error:   × semantic analysis failed
// INDEX: Error:
// INDEX:   × initializer element is not a compile-time constant: reads a variable
// INDEX:     ╭─[tests/fixtures/error/clang/linux/x86_64/static_initializer_reads_variable.c:23:26]
// INDEX:  22 │ static int arr[4];
// INDEX:  23 │ static int *index = &arr[x];
// INDEX:     ·                          ─
// INDEX:  24 │ #endif
// INDEX:     ╰────
// SLATE-FILECHECK-END INDEX
