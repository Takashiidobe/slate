// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

__attribute__((always_inline)) int conflict(void);
__attribute__((noinline)) int conflict(void) { return 0; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: conflicting always_inline and noinline attributes
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_inline_conflict.c:3:1]
// DEFAULT: 2 │ __attribute__((always_inline)) int conflict(void);
// DEFAULT: 3 │ __attribute__((noinline)) int conflict(void) { return 0; }
// DEFAULT: · ──────────────────────────────────────────────────────────
// DEFAULT: 4 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
