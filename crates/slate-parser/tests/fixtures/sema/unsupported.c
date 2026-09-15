// SLATE-FILECHECK-DEFINES MIXED MIXED
// SLATE-FILECHECK-DEFINES MIXED_SUB MIXED_SUB
// SLATE-FILECHECK-DEFINES FLOAT_REM FLOAT_REM
// SLATE-FILECHECK-DEFINES OPERATOR OPERATOR
// SLATE-FILECHECK-DEFINES BITINT BITINT
// SLATE-FILECHECK-DEFINES DECIMAL DECIMAL
// SLATE-FILECHECK-DEFINES EXTENDED EXTENDED
// SLATE-FILECHECK-ERROR DECIMAL
// SLATE-FILECHECK-ERROR EXTENDED
// SLATE-FILECHECK-ERROR MIXED
// SLATE-FILECHECK-ERROR MIXED_SUB
// SLATE-FILECHECK-ERROR FLOAT_REM
// SLATE-FILECHECK-ERROR OPERATOR
// SLATE-FILECHECK-ERROR BITINT
// SLATE-FILECHECK-ARGS --dump-ir-expressions

void unsupported(void) {
#ifdef MIXED
    1 + 2U;
#endif
#ifdef MIXED_SUB
    1.0f - 2.0;
#endif
#ifdef FLOAT_REM
    1.0 % 2.0;
#endif
#ifdef OPERATOR
    1 < 2;
#endif
#ifdef BITINT
    1wb;
#endif
#ifdef DECIMAL
    1.0DF;
#endif
#ifdef EXTENDED
    1.0f64x;
#endif
}

// SLATE-FILECHECK-BEGIN DECIMAL
// DECIMAL: Error:   × unsupported in numeric IR lowering: decimal floating literals
// SLATE-FILECHECK-END DECIMAL
// SLATE-FILECHECK-BEGIN EXTENDED
// EXTENDED: Error:   × unsupported in numeric IR lowering: target-dependent f64x literals
// SLATE-FILECHECK-END EXTENDED
// SLATE-FILECHECK-BEGIN MIXED
// MIXED: Error:   × arithmetic requires conversions not yet implemented: i32 + u32
// SLATE-FILECHECK-END MIXED
// SLATE-FILECHECK-BEGIN MIXED_SUB
// MIXED_SUB: Error:   × arithmetic requires conversions not yet implemented: f32 - f64
// SLATE-FILECHECK-END MIXED_SUB
// SLATE-FILECHECK-BEGIN FLOAT_REM
// FLOAT_REM: Error:   × invalid operands to binary expression: f64 % f64
// SLATE-FILECHECK-END FLOAT_REM
// SLATE-FILECHECK-BEGIN OPERATOR
// OPERATOR: Error:   × unsupported in numeric IR lowering: expression (expected a number or
// SLATE-FILECHECK-END OPERATOR
// SLATE-FILECHECK-BEGIN BITINT
// BITINT: Error:   × unsupported in numeric IR lowering: bit-precise integer literals
// SLATE-FILECHECK-END BITINT
