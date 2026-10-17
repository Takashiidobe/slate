void add(void) {
    1.0 + 2.0;
}

// SLATE-FILECHECK-ARGS --dump-ir-expressions
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES FAST
// SLATE-FILECHECK-PREFIX-ARGS FAST -ffast-math
// SLATE-FILECHECK-DEFINES SNAN
// SLATE-FILECHECK-PREFIX-ARGS SNAN -ffast-math -fsignaling-nans

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(1.0), const<f64>(2.0))
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FAST
// FAST: add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(const<f64>(1.0), const<f64>(2.0))
// SLATE-FILECHECK-END FAST
// SLATE-FILECHECK-BEGIN SNAN
// SNAN: add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(1.0), const<f64>(2.0))
// SLATE-FILECHECK-END SNAN
