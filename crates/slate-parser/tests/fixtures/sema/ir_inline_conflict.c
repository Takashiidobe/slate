// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

__attribute__((always_inline)) int conflict(void);
__attribute__((noinline)) int conflict(void) { return 0; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid in this context: conflicting always_inline and noinline attributes
// SLATE-FILECHECK-END DEFAULT
