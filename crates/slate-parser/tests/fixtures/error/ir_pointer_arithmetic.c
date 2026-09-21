// SLATE-FILECHECK-DEFINES FLOAT -DFLOAT
// SLATE-FILECHECK-DEFINES INCOMPATIBLE -DINCOMPATIBLE
// SLATE-FILECHECK-DEFINES INCOMPLETE -DINCOMPLETE
// SLATE-FILECHECK-ERROR FLOAT
// SLATE-FILECHECK-ERROR INCOMPATIBLE
// SLATE-FILECHECK-ERROR INCOMPLETE
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef FLOAT
void bad(int *p) { p + 1.0; }
#endif
#ifdef INCOMPATIBLE
void bad(int *p, long *q) { p - q; }
#endif
#ifdef INCOMPLETE
struct S;
void bad(struct S *p) { p++; }
#endif

// SLATE-FILECHECK-BEGIN FLOAT
// FLOAT: Error:   × unsupported in numeric IR lowering: noninteger pointer offset
// SLATE-FILECHECK-END FLOAT
// SLATE-FILECHECK-BEGIN INCOMPATIBLE
// INCOMPATIBLE: Error:   × unsupported in numeric IR lowering: incompatible pointer subtraction
// SLATE-FILECHECK-END INCOMPATIBLE
// SLATE-FILECHECK-BEGIN INCOMPLETE
// INCOMPLETE: Error:   × unsupported in numeric IR lowering: incomplete field type
// SLATE-FILECHECK-END INCOMPLETE
