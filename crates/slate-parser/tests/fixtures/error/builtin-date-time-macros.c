#if __DATE__ && __TIME__
#error date and time must not be integers
#endif

// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid #if expression: string literal is not a constant expression
// DEFAULT: ╰─▶ invalid #if expression: string literal is not a constant expression
// DEFAULT: ╭─[tests/fixtures/error/builtin-date-time-macros.c:1:5]
// DEFAULT: 1 │ #if __DATE__ && __TIME__
// DEFAULT: ·     ────────────────────
// DEFAULT: 2 │ #error date and time must not be integers
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
