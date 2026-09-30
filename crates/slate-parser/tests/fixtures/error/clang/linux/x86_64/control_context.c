// SLATE-FILECHECK-DEFINES BREAK BREAK
// SLATE-FILECHECK-ERROR BREAK
// SLATE-FILECHECK-DEFINES CONTINUE CONTINUE
// SLATE-FILECHECK-ERROR CONTINUE
// SLATE-FILECHECK-DEFINES CASE CASE
// SLATE-FILECHECK-ERROR CASE
// SLATE-FILECHECK-DEFINES DEFAULT DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-DEFINES FLOAT FLOAT
// SLATE-FILECHECK-ERROR FLOAT
// SLATE-FILECHECK-DEFINES NONCONSTANT NONCONSTANT
// SLATE-FILECHECK-ERROR NONCONSTANT
// SLATE-FILECHECK-DEFINES INDIRECT INDIRECT
// SLATE-FILECHECK-ERROR INDIRECT
// SLATE-FILECHECK-DEFINES FALLTHROUGH FALLTHROUGH
// SLATE-FILECHECK-ERROR FALLTHROUGH
// SLATE-FILECHECK-DEFINES VOID_RETURN VOID_RETURN
// SLATE-FILECHECK-ERROR VOID_RETURN
// SLATE-FILECHECK-DEFINES VALUELESS VALUELESS
// SLATE-FILECHECK-ERROR VALUELESS
// SLATE-FILECHECK-DEFINES NONSCALAR NONSCALAR
// SLATE-FILECHECK-ERROR NONSCALAR
struct s { int a; };
#if defined(VALUELESS)
int valueless(void) { return; }
#endif
void bad(int x, struct s s) {
#if defined(BREAK)
    break;
#elif defined(CONTINUE)
    switch (x) { default: continue; }
#elif defined(CASE)
    case 1: ;
#elif defined(DEFAULT)
    default: ;
#elif defined(FLOAT)
    switch (1.0) {}
#elif defined(NONCONSTANT)
    switch (x) { case x: ; }
#elif defined(INDIRECT)
    goto *1;
#elif defined(FALLTHROUGH)
    [[fallthrough]];
#elif defined(VOID_RETURN)
    return x;
#elif defined(NONSCALAR)
    while (s) {}
#endif
}

// SLATE-FILECHECK-BEGIN BREAK
// BREAK: Error:   × semantic analysis failed
// BREAK: Error:
// BREAK: × break outside loop or switch
// BREAK: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:7:5]
// BREAK: 6 │ #if defined(BREAK)
// BREAK: 7 │     break;
// BREAK: ·     ──────
// BREAK: 8 │ #elif defined(CONTINUE)
// BREAK: ╰────
// SLATE-FILECHECK-END BREAK
// SLATE-FILECHECK-BEGIN CONTINUE
// CONTINUE: Error:   × semantic analysis failed
// CONTINUE: Error:
// CONTINUE: × continue outside loop
// CONTINUE: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:9:27]
// CONTINUE: 8 │ #elif defined(CONTINUE)
// CONTINUE: 9 │     switch (x) { default: continue; }
// CONTINUE: ·                           ─────────
// CONTINUE: 10 │ #elif defined(CASE)
// CONTINUE: ╰────
// SLATE-FILECHECK-END CONTINUE
// SLATE-FILECHECK-BEGIN CASE
// CASE: Error:   × semantic analysis failed
// CASE: Error:
// CASE: × case or default outside switch
// CASE: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:11:5]
// CASE: 10 │ #elif defined(CASE)
// CASE: 11 │     case 1: ;
// CASE: ·     ─────────
// CASE: 12 │ #elif defined(DEFAULT)
// CASE: ╰────
// SLATE-FILECHECK-END CASE
// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × case or default outside switch
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:13:5]
// DEFAULT: 12 │ #elif defined(DEFAULT)
// DEFAULT: 13 │     default: ;
// DEFAULT: ·     ──────────
// DEFAULT: 14 │ #elif defined(FLOAT)
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FLOAT
// FLOAT: Error:   × semantic analysis failed
// FLOAT: Error:
// FLOAT: × noninteger switch discriminant
// FLOAT: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:15:5]
// FLOAT: 14 │ #elif defined(FLOAT)
// FLOAT: 15 │     switch (1.0) {}
// FLOAT: ·     ───────────────
// FLOAT: 16 │ #elif defined(NONCONSTANT)
// FLOAT: ╰────
// SLATE-FILECHECK-END FLOAT
// SLATE-FILECHECK-BEGIN NONCONSTANT
// NONCONSTANT: Error:   × semantic analysis failed
// NONCONSTANT: Error:
// NONCONSTANT: × nonconstant case expression
// NONCONSTANT: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:17:18]
// NONCONSTANT: 16 │ #elif defined(NONCONSTANT)
// NONCONSTANT: 17 │     switch (x) { case x: ; }
// NONCONSTANT: ·                  ─────────
// NONCONSTANT: 18 │ #elif defined(INDIRECT)
// NONCONSTANT: ╰────
// SLATE-FILECHECK-END NONCONSTANT
// SLATE-FILECHECK-BEGIN INDIRECT
// INDIRECT: Error:   × semantic analysis failed
// INDIRECT: Error:
// INDIRECT: × nonpointer computed goto
// INDIRECT: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:19:5]
// INDIRECT: 18 │ #elif defined(INDIRECT)
// INDIRECT: 19 │     goto *1;
// INDIRECT: ·     ────────
// INDIRECT: 20 │ #elif defined(FALLTHROUGH)
// INDIRECT: ╰────
// SLATE-FILECHECK-END INDIRECT
// SLATE-FILECHECK-BEGIN FALLTHROUGH
// FALLTHROUGH: Error:   × semantic analysis failed
// FALLTHROUGH: Error:
// FALLTHROUGH: × fallthrough outside switch
// FALLTHROUGH: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:21:5]
// FALLTHROUGH: 20 │ #elif defined(FALLTHROUGH)
// FALLTHROUGH: 21 │     {{\[\[}}fallthrough]];
// FALLTHROUGH: ·     ────────────────
// FALLTHROUGH: 22 │ #elif defined(VOID_RETURN)
// FALLTHROUGH: ╰────
// SLATE-FILECHECK-END FALLTHROUGH
// SLATE-FILECHECK-BEGIN VOID_RETURN
// VOID_RETURN: Error:   × semantic analysis failed
// VOID_RETURN: Error:
// VOID_RETURN: × value return from void function
// VOID_RETURN: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:23:5]
// VOID_RETURN: 22 │ #elif defined(VOID_RETURN)
// VOID_RETURN: 23 │     return x;
// VOID_RETURN: ·     ─────────
// VOID_RETURN: 24 │ #elif defined(NONSCALAR)
// VOID_RETURN: ╰────
// SLATE-FILECHECK-END VOID_RETURN
// SLATE-FILECHECK-BEGIN VALUELESS
// VALUELESS: Error:   × semantic analysis failed
// VALUELESS: Error:
// VALUELESS: × non-void function should return a value
// VALUELESS: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:3:23]
// VALUELESS: 2 │ #if defined(VALUELESS)
// VALUELESS: 3 │ int valueless(void) { return; }
// VALUELESS: ·                       ───────
// VALUELESS: 4 │ #endif
// VALUELESS: ╰────
// SLATE-FILECHECK-END VALUELESS
// SLATE-FILECHECK-BEGIN NONSCALAR
// NONSCALAR: Error:   × semantic analysis failed
// NONSCALAR: Error:
// NONSCALAR: × non-scalar condition
// NONSCALAR: ╭─[tests/fixtures/error/clang/linux/x86_64/control_context.c:25:12]
// NONSCALAR: 24 │ #elif defined(NONSCALAR)
// NONSCALAR: 25 │     while (s) {}
// NONSCALAR: ·            ─
// NONSCALAR: 26 │ #endif
// NONSCALAR: ╰────
// SLATE-FILECHECK-END NONSCALAR
