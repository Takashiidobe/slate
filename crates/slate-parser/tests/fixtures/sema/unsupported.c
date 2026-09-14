// SLATE-FILECHECK-DEFINES MIXED MIXED
// SLATE-FILECHECK-DEFINES OPERATOR OPERATOR
// SLATE-FILECHECK-DEFINES BITINT BITINT
// SLATE-FILECHECK-DEFINES DECIMAL DECIMAL
// SLATE-FILECHECK-DEFINES EXTENDED EXTENDED
// SLATE-FILECHECK-ERROR DECIMAL
// SLATE-FILECHECK-ERROR EXTENDED
// SLATE-FILECHECK-ERROR MIXED
// SLATE-FILECHECK-ERROR OPERATOR
// SLATE-FILECHECK-ERROR BITINT
// SLATE-FILECHECK-ARGS --dump-ir-expressions

void unsupported(void) {
#ifdef MIXED
    1 + 2U;
#endif
#ifdef OPERATOR
    1 * 2;
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
// MIXED: Error:   × addition requires conversions not yet implemented: i32 + u32
// SLATE-FILECHECK-END MIXED
// SLATE-FILECHECK-BEGIN OPERATOR
// OPERATOR: Error:   × unsupported in numeric IR lowering: expression (expected a number or
// SLATE-FILECHECK-END OPERATOR
// SLATE-FILECHECK-BEGIN BITINT
// BITINT: Error:   × unsupported in numeric IR lowering: bit-precise integer literals
// SLATE-FILECHECK-END BITINT
