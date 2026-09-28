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
// BREAK: ╭─[tests/fixtures/error/ir_control_context.c:1:1]
// BREAK: 1 │ ╭─▶ void bad(int x) {
// BREAK: 2 │ │   #if defined(BREAK)
// BREAK: 3 │ │       break;
// BREAK: 4 │ │   #elif defined(CONTINUE)
// BREAK: 5 │ │       switch (x) { default: continue; }
// BREAK: 6 │ │   #elif defined(CASE)
// BREAK: 7 │ │       case 1: ;
// BREAK: 8 │ │   #elif defined(DEFAULT)
// BREAK: 9 │ │       default: ;
// BREAK: 10 │ │   #elif defined(FLOAT)
// BREAK: 11 │ │       switch (1.0) {}
// BREAK: 12 │ │   #elif defined(NONCONSTANT)
// BREAK: 13 │ │       switch (x) { case x: ; }
// BREAK: 14 │ │   #elif defined(INDIRECT)
// BREAK: 15 │ │       goto *1;
// BREAK: 16 │ │   #endif
// BREAK: 17 │ ╰─▶ }
// BREAK: 18 │
// BREAK: ╰────
// SLATE-FILECHECK-END BREAK
// SLATE-FILECHECK-BEGIN CONTINUE
// CONTINUE: Error:   × semantic analysis failed
// CONTINUE: Error:
// CONTINUE: × unsupported in numeric IR lowering: continue outside loop
// CONTINUE: ╭─[tests/fixtures/error/ir_control_context.c:1:1]
// CONTINUE: 1 │ ╭─▶ void bad(int x) {
// CONTINUE: 2 │ │   #if defined(BREAK)
// CONTINUE: 3 │ │       break;
// CONTINUE: 4 │ │   #elif defined(CONTINUE)
// CONTINUE: 5 │ │       switch (x) { default: continue; }
// CONTINUE: 6 │ │   #elif defined(CASE)
// CONTINUE: 7 │ │       case 1: ;
// CONTINUE: 8 │ │   #elif defined(DEFAULT)
// CONTINUE: 9 │ │       default: ;
// CONTINUE: 10 │ │   #elif defined(FLOAT)
// CONTINUE: 11 │ │       switch (1.0) {}
// CONTINUE: 12 │ │   #elif defined(NONCONSTANT)
// CONTINUE: 13 │ │       switch (x) { case x: ; }
// CONTINUE: 14 │ │   #elif defined(INDIRECT)
// CONTINUE: 15 │ │       goto *1;
// CONTINUE: 16 │ │   #endif
// CONTINUE: 17 │ ╰─▶ }
// CONTINUE: 18 │
// CONTINUE: ╰────
// SLATE-FILECHECK-END CONTINUE
// SLATE-FILECHECK-BEGIN CASE
// CASE: Error:   × semantic analysis failed
// CASE: Error:
// CASE: × unsupported in numeric IR lowering: case or default outside switch
// CASE: ╭─[tests/fixtures/error/ir_control_context.c:1:1]
// CASE: 1 │ ╭─▶ void bad(int x) {
// CASE: 2 │ │   #if defined(BREAK)
// CASE: 3 │ │       break;
// CASE: 4 │ │   #elif defined(CONTINUE)
// CASE: 5 │ │       switch (x) { default: continue; }
// CASE: 6 │ │   #elif defined(CASE)
// CASE: 7 │ │       case 1: ;
// CASE: 8 │ │   #elif defined(DEFAULT)
// CASE: 9 │ │       default: ;
// CASE: 10 │ │   #elif defined(FLOAT)
// CASE: 11 │ │       switch (1.0) {}
// CASE: 12 │ │   #elif defined(NONCONSTANT)
// CASE: 13 │ │       switch (x) { case x: ; }
// CASE: 14 │ │   #elif defined(INDIRECT)
// CASE: 15 │ │       goto *1;
// CASE: 16 │ │   #endif
// CASE: 17 │ ╰─▶ }
// CASE: 18 │
// CASE: ╰────
// SLATE-FILECHECK-END CASE
// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: case or default outside switch
// DEFAULT: ╭─[tests/fixtures/error/ir_control_context.c:1:1]
// DEFAULT: 1 │ ╭─▶ void bad(int x) {
// DEFAULT: 2 │ │   #if defined(BREAK)
// DEFAULT: 3 │ │       break;
// DEFAULT: 4 │ │   #elif defined(CONTINUE)
// DEFAULT: 5 │ │       switch (x) { default: continue; }
// DEFAULT: 6 │ │   #elif defined(CASE)
// DEFAULT: 7 │ │       case 1: ;
// DEFAULT: 8 │ │   #elif defined(DEFAULT)
// DEFAULT: 9 │ │       default: ;
// DEFAULT: 10 │ │   #elif defined(FLOAT)
// DEFAULT: 11 │ │       switch (1.0) {}
// DEFAULT: 12 │ │   #elif defined(NONCONSTANT)
// DEFAULT: 13 │ │       switch (x) { case x: ; }
// DEFAULT: 14 │ │   #elif defined(INDIRECT)
// DEFAULT: 15 │ │       goto *1;
// DEFAULT: 16 │ │   #endif
// DEFAULT: 17 │ ╰─▶ }
// DEFAULT: 18 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FLOAT
// FLOAT: Error:   × semantic analysis failed
// FLOAT: Error:
// FLOAT: × unsupported in numeric IR lowering: noninteger switch discriminant
// FLOAT: ╭─[tests/fixtures/error/ir_control_context.c:1:1]
// FLOAT: 1 │ ╭─▶ void bad(int x) {
// FLOAT: 2 │ │   #if defined(BREAK)
// FLOAT: 3 │ │       break;
// FLOAT: 4 │ │   #elif defined(CONTINUE)
// FLOAT: 5 │ │       switch (x) { default: continue; }
// FLOAT: 6 │ │   #elif defined(CASE)
// FLOAT: 7 │ │       case 1: ;
// FLOAT: 8 │ │   #elif defined(DEFAULT)
// FLOAT: 9 │ │       default: ;
// FLOAT: 10 │ │   #elif defined(FLOAT)
// FLOAT: 11 │ │       switch (1.0) {}
// FLOAT: 12 │ │   #elif defined(NONCONSTANT)
// FLOAT: 13 │ │       switch (x) { case x: ; }
// FLOAT: 14 │ │   #elif defined(INDIRECT)
// FLOAT: 15 │ │       goto *1;
// FLOAT: 16 │ │   #endif
// FLOAT: 17 │ ╰─▶ }
// FLOAT: 18 │
// FLOAT: ╰────
// SLATE-FILECHECK-END FLOAT
// SLATE-FILECHECK-BEGIN NONCONSTANT
// NONCONSTANT: Error:   × semantic analysis failed
// NONCONSTANT: Error:
// NONCONSTANT: × unsupported in numeric IR lowering: nonconstant case expression
// NONCONSTANT: ╭─[tests/fixtures/error/ir_control_context.c:1:1]
// NONCONSTANT: 1 │ ╭─▶ void bad(int x) {
// NONCONSTANT: 2 │ │   #if defined(BREAK)
// NONCONSTANT: 3 │ │       break;
// NONCONSTANT: 4 │ │   #elif defined(CONTINUE)
// NONCONSTANT: 5 │ │       switch (x) { default: continue; }
// NONCONSTANT: 6 │ │   #elif defined(CASE)
// NONCONSTANT: 7 │ │       case 1: ;
// NONCONSTANT: 8 │ │   #elif defined(DEFAULT)
// NONCONSTANT: 9 │ │       default: ;
// NONCONSTANT: 10 │ │   #elif defined(FLOAT)
// NONCONSTANT: 11 │ │       switch (1.0) {}
// NONCONSTANT: 12 │ │   #elif defined(NONCONSTANT)
// NONCONSTANT: 13 │ │       switch (x) { case x: ; }
// NONCONSTANT: 14 │ │   #elif defined(INDIRECT)
// NONCONSTANT: 15 │ │       goto *1;
// NONCONSTANT: 16 │ │   #endif
// NONCONSTANT: 17 │ ╰─▶ }
// NONCONSTANT: 18 │
// NONCONSTANT: ╰────
// SLATE-FILECHECK-END NONCONSTANT
// SLATE-FILECHECK-BEGIN INDIRECT
// INDIRECT: Error:   × semantic analysis failed
// INDIRECT: Error:
// INDIRECT: × unsupported in numeric IR lowering: nonpointer computed goto
// INDIRECT: ╭─[tests/fixtures/error/ir_control_context.c:1:1]
// INDIRECT: 1 │ ╭─▶ void bad(int x) {
// INDIRECT: 2 │ │   #if defined(BREAK)
// INDIRECT: 3 │ │       break;
// INDIRECT: 4 │ │   #elif defined(CONTINUE)
// INDIRECT: 5 │ │       switch (x) { default: continue; }
// INDIRECT: 6 │ │   #elif defined(CASE)
// INDIRECT: 7 │ │       case 1: ;
// INDIRECT: 8 │ │   #elif defined(DEFAULT)
// INDIRECT: 9 │ │       default: ;
// INDIRECT: 10 │ │   #elif defined(FLOAT)
// INDIRECT: 11 │ │       switch (1.0) {}
// INDIRECT: 12 │ │   #elif defined(NONCONSTANT)
// INDIRECT: 13 │ │       switch (x) { case x: ; }
// INDIRECT: 14 │ │   #elif defined(INDIRECT)
// INDIRECT: 15 │ │       goto *1;
// INDIRECT: 16 │ │   #endif
// INDIRECT: 17 │ ╰─▶ }
// INDIRECT: 18 │
// INDIRECT: ╰────
// SLATE-FILECHECK-END INDIRECT
