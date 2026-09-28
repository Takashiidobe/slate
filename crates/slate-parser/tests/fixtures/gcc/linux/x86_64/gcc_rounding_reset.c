// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -frounding-math -fno-rounding-math

void rounding(void) {
#ifdef __ROUNDING_MATH__
    __ROUNDING_MATH__;
#else
    0;
#endif
    1.0 + 2.0;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: const<i32>(0)
// CHECK-NEXT: add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(1.0), const<f64>(2.0))
// SLATE-FILECHECK-END CHECK
