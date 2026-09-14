// SLATE-FILECHECK-DEFINES MIXED MIXED
// SLATE-FILECHECK-DEFINES OPERATOR OPERATOR
// SLATE-FILECHECK-DEFINES BITINT BITINT
// SLATE-FILECHECK-DEFINES LONGDOUBLE LONGDOUBLE
// SLATE-FILECHECK-ERROR MIXED
// SLATE-FILECHECK-ERROR OPERATOR
// SLATE-FILECHECK-ERROR BITINT
// SLATE-FILECHECK-ERROR LONGDOUBLE
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
#ifdef LONGDOUBLE
    1.0L;
#endif
}

// SLATE-FILECHECK-BEGIN MIXED
// MIXED: Error:   × addition requires conversions not yet implemented: i32 + u32
// SLATE-FILECHECK-END MIXED
// SLATE-FILECHECK-BEGIN OPERATOR
// OPERATOR: Error:   × unsupported in numeric IR lowering: expression (expected a number or
// SLATE-FILECHECK-END OPERATOR
// SLATE-FILECHECK-BEGIN BITINT
// BITINT: Error:   × unsupported in numeric IR lowering: bit-precise integer literals
// SLATE-FILECHECK-END BITINT
// SLATE-FILECHECK-BEGIN LONGDOUBLE
// LONGDOUBLE: Error:   × unsupported in numeric IR lowering: target-dependent or decimal floating
// SLATE-FILECHECK-END LONGDOUBLE
