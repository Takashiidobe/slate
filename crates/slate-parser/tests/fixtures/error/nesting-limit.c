// SLATE-FILECHECK-DEFINES EXPR
// SLATE-FILECHECK-DEFINES BLOCK BLOCK
// SLATE-FILECHECK-DEFINES CONDITION CONDITION
// SLATE-FILECHECK-ERROR EXPR
// SLATE-FILECHECK-ERROR BLOCK
// SLATE-FILECHECK-ERROR CONDITION
// SLATE-FILECHECK-ARGS -std=c23

#define PARENS1(x) ((((((((((x))))))))))
#define PARENS2(x) PARENS1(PARENS1(PARENS1(PARENS1(PARENS1(PARENS1(PARENS1(PARENS1(PARENS1(PARENS1(x))))))))))
#define PARENS3(x) PARENS2(PARENS2(PARENS2(PARENS2(PARENS2(PARENS2(PARENS2(PARENS2(PARENS2(PARENS2(x))))))))))
#define BRACES1(x) {{{{{{{{{{x}}}}}}}}}}
#define BRACES2(x) BRACES1(BRACES1(BRACES1(BRACES1(BRACES1(BRACES1(BRACES1(BRACES1(BRACES1(BRACES1(x))))))))))
#define BRACES3(x) BRACES2(BRACES2(BRACES2(BRACES2(BRACES2(BRACES2(BRACES2(BRACES2(BRACES2(BRACES2(x))))))))))

#if defined(BLOCK)
void nested(void) BRACES3(BRACES3(BRACES3(;)))
#elif defined(CONDITION)
#if PARENS3(PARENS2(1))
int selected;
#endif
#else
int deep = PARENS3(PARENS2(1));
#endif

// SLATE-FILECHECK-BEGIN EXPR
// EXPR: Error:   × nesting level exceeded maximum
// EXPR: ╰─▶ nesting level exceeded maximum
// EXPR: ╭─[tests/fixtures/error/nesting-limit.c:16:12]
// EXPR: 15 │ #else
// EXPR: 16 │ int deep = PARENS3(PARENS2(1));
// EXPR: ·            ───────
// EXPR: 17 │ #endif
// EXPR: ╰────
// SLATE-FILECHECK-END EXPR
// SLATE-FILECHECK-BEGIN BLOCK
// BLOCK: Error:   × nesting level exceeded maximum
// BLOCK: ╰─▶ nesting level exceeded maximum
// BLOCK: ╭─[tests/fixtures/error/nesting-limit.c:10:27]
// BLOCK: 9 │ #if defined(BLOCK)
// BLOCK: 10 │ void nested(void) BRACES3(BRACES3(BRACES3(;)))
// BLOCK: ·                           ───────
// BLOCK: 11 │ #elif defined(CONDITION)
// BLOCK: ╰────
// SLATE-FILECHECK-END BLOCK
// SLATE-FILECHECK-BEGIN CONDITION
// CONDITION: Error:   × invalid #if expression: nesting level exceeded maximum
// CONDITION: ╰─▶ invalid #if expression: nesting level exceeded maximum
// CONDITION: ╭─[tests/fixtures/error/nesting-limit.c:12:5]
// CONDITION: 11 │ #elif defined(CONDITION)
// CONDITION: 12 │ #if PARENS3(PARENS2(1))
// CONDITION: ·     ───────
// CONDITION: 13 │ int selected;
// CONDITION: ╰────
// SLATE-FILECHECK-END CONDITION
