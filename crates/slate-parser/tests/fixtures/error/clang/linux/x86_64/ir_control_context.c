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
// SLATE-FILECHECK-ARGS --dump-ir
void bad(int x) {
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
#endif
}

// SLATE-FILECHECK-BEGIN BREAK
// BREAK: Error:   × semantic analysis failed
// BREAK: Error:
// BREAK: × unsupported in numeric IR lowering: break outside loop or switch
// BREAK: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_control_context.c:3:5]
// BREAK: 2 │ #if defined(BREAK)
// BREAK: 3 │     break;
// BREAK: ·     ──────
// BREAK: 4 │ #elif defined(CONTINUE)
// BREAK: ╰────
// SLATE-FILECHECK-END BREAK
// SLATE-FILECHECK-BEGIN CONTINUE
// CONTINUE: Error:   × semantic analysis failed
// CONTINUE: Error:
// CONTINUE: × unsupported in numeric IR lowering: continue outside loop
// CONTINUE: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_control_context.c:5:27]
// CONTINUE: 4 │ #elif defined(CONTINUE)
// CONTINUE: 5 │     switch (x) { default: continue; }
// CONTINUE: ·                           ─────────
// CONTINUE: 6 │ #elif defined(CASE)
// CONTINUE: ╰────
// SLATE-FILECHECK-END CONTINUE
// SLATE-FILECHECK-BEGIN CASE
// CASE: Error:   × semantic analysis failed
// CASE: Error:
// CASE: × unsupported in numeric IR lowering: case or default outside switch
// CASE: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_control_context.c:7:5]
// CASE: 6 │ #elif defined(CASE)
// CASE: 7 │     case 1: ;
// CASE: ·     ─────────
// CASE: 8 │ #elif defined(DEFAULT)
// CASE: ╰────
// SLATE-FILECHECK-END CASE
// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: case or default outside switch
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_control_context.c:9:5]
// DEFAULT: 8 │ #elif defined(DEFAULT)
// DEFAULT: 9 │     default: ;
// DEFAULT: ·     ──────────
// DEFAULT: 10 │ #elif defined(FLOAT)
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FLOAT
// FLOAT: Error:   × semantic analysis failed
// FLOAT: Error:
// FLOAT: × unsupported in numeric IR lowering: noninteger switch discriminant
// FLOAT: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_control_context.c:11:5]
// FLOAT: 10 │ #elif defined(FLOAT)
// FLOAT: 11 │     switch (1.0) {}
// FLOAT: ·     ───────────────
// FLOAT: 12 │ #elif defined(NONCONSTANT)
// FLOAT: ╰────
// SLATE-FILECHECK-END FLOAT
// SLATE-FILECHECK-BEGIN NONCONSTANT
// NONCONSTANT: Error:   × semantic analysis failed
// NONCONSTANT: Error:
// NONCONSTANT: × unsupported in numeric IR lowering: nonconstant case expression
// NONCONSTANT: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_control_context.c:13:18]
// NONCONSTANT: 12 │ #elif defined(NONCONSTANT)
// NONCONSTANT: 13 │     switch (x) { case x: ; }
// NONCONSTANT: ·                  ─────────
// NONCONSTANT: 14 │ #elif defined(INDIRECT)
// NONCONSTANT: ╰────
// SLATE-FILECHECK-END NONCONSTANT
// SLATE-FILECHECK-BEGIN INDIRECT
// INDIRECT: Error:   × semantic analysis failed
// INDIRECT: Error:
// INDIRECT: × unsupported in numeric IR lowering: nonpointer computed goto
// INDIRECT: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_control_context.c:15:5]
// INDIRECT: 14 │ #elif defined(INDIRECT)
// INDIRECT: 15 │     goto *1;
// INDIRECT: ·     ────────
// INDIRECT: 16 │ #endif
// INDIRECT: ╰────
// SLATE-FILECHECK-END INDIRECT
