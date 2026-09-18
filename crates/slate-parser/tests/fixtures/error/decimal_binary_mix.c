_Decimal64 mixed(_Decimal64 x, double d) { return x + d; }

// SLATE-FILECHECK-ERROR SEMA
// SLATE-FILECHECK-ARGS --dump-ir

// SLATE-FILECHECK-BEGIN SEMA
// SEMA: Error:   × invalid operands to binary expression: d64 + f64
// SLATE-FILECHECK-END SEMA
