#ifdef DIRECTIVE
#assert system(linux)
#endif
#ifdef CONDITION
#if #system(linux)
#endif
#endif
int value;

// SLATE-FILECHECK-DEFINES DIRECTIVE DIRECTIVE
// SLATE-FILECHECK-ERROR DIRECTIVE
// SLATE-FILECHECK-DEFINES CONDITION CONDITION
// SLATE-FILECHECK-ERROR CONDITION

// SLATE-FILECHECK-BEGIN DIRECTIVE
// DIRECTIVE: Error:   × unsupported preprocessor directive
// DIRECTIVE: ╰─▶ unsupported preprocessor directive
// DIRECTIVE: ╭─[tests/fixtures/error/clang/linux/x86_64/pp-assert-gnu-only.c:2:2]
// DIRECTIVE: 1 │ #ifdef DIRECTIVE
// DIRECTIVE: 2 │ #assert system(linux)
// DIRECTIVE: ·  ──────
// DIRECTIVE: 3 │ #endif
// DIRECTIVE: ╰────
// SLATE-FILECHECK-END DIRECTIVE
// SLATE-FILECHECK-BEGIN CONDITION
// CONDITION: Error:   × invalid #if expression: unexpected token `Hash`
// CONDITION: ╰─▶ invalid #if expression: unexpected token `Hash`
// CONDITION: ╭─[tests/fixtures/error/clang/linux/x86_64/pp-assert-gnu-only.c:5:5]
// CONDITION: 4 │ #ifdef CONDITION
// CONDITION: 5 │ #if #system(linux)
// CONDITION: ·     ─
// CONDITION: 6 │ #endif
// CONDITION: ╰────
// SLATE-FILECHECK-END CONDITION
