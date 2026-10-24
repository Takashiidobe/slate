// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

#if (UNDEF(1))
#endif

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × invalid #if expression: expected `)`, found `(`
// IR:   ╰─▶ invalid #if expression: expected `)`, found `(`
// IR:    ╭─[tests/fixtures/error/msvc/windows/x86_64/if-trailing-tokens-in-parens.c:2:11]
// IR:  1 │
// IR:  2 │ #if (UNDEF(1))
// IR:    ·           ─
// IR:  3 │ #endif
// IR:    ╰────
// SLATE-FILECHECK-END IR
