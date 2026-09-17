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
// BREAK: Error:   × unsupported in numeric IR lowering: break outside loop or switch
// SLATE-FILECHECK-END BREAK
// SLATE-FILECHECK-BEGIN CONTINUE
// CONTINUE: Error:   × unsupported in numeric IR lowering: continue outside loop
// SLATE-FILECHECK-END CONTINUE
// SLATE-FILECHECK-BEGIN CASE
// CASE: Error:   × unsupported in numeric IR lowering: case or default outside switch
// SLATE-FILECHECK-END CASE
// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: case or default outside switch
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FLOAT
// FLOAT: Error:   × unsupported in numeric IR lowering: noninteger switch discriminant
// SLATE-FILECHECK-END FLOAT
// SLATE-FILECHECK-BEGIN NONCONSTANT
// NONCONSTANT: Error:   × unsupported in numeric IR lowering: nonconstant case expression
// SLATE-FILECHECK-END NONCONSTANT
// SLATE-FILECHECK-BEGIN INDIRECT
// INDIRECT: Error:   × unsupported in numeric IR lowering: nonpointer computed goto
// SLATE-FILECHECK-END INDIRECT
