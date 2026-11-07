// SLATE-FILECHECK-DEFINES SCALE SCALE
// SLATE-FILECHECK-DEFINES NEGATE NEGATE
// SLATE-FILECHECK-DEFINES CAST CAST
// SLATE-FILECHECK-ERROR SCALE
// SLATE-FILECHECK-ERROR NEGATE
// SLATE-FILECHECK-ERROR CAST
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
