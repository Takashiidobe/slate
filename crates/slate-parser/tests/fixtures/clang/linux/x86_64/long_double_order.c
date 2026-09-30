// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -mlong-double-128 -mlong-double-64

void numbers(void) {
    1.0L + 2.0L;
    __LDBL_MANT_DIG__;
    __SIZEOF_LONG_DOUBLE__;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(2.0))
// CHECK-NEXT: const<i32>(53)
// CHECK-NEXT: const<i32>(8)
// SLATE-FILECHECK-END CHECK
