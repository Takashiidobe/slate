// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

#if 1 )
#endif

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × invalid #if expression: unexpected tokens after expression
// IR:   ╰─▶ invalid #if expression: unexpected tokens after expression
// IR:    ╭─[tests/fixtures/error/msvc/windows/x86_64/if-trailing-rparen.c:2:7]
// IR:  1 │
// IR:  2 │ #if 1 )
// IR:    ·       ─
// IR:  3 │ #endif
// IR:    ╰────
// SLATE-FILECHECK-END IR
