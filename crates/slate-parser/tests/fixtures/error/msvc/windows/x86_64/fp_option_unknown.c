// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir /fp:FAST
double mul_add(double a, double b, double c) { return a * b + c; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/fp:FAST`: expected one of precise, strict,
// SLATE-FILECHECK-END DEFAULT
