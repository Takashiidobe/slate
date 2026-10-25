// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir /fp:except /fp:precise /fp:fast
double mul_add(double a, double b, double c) { return a * b + c; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid compiler argument `/fp:fast`: incompatible with /fp:except
// SLATE-FILECHECK-END DEFAULT
