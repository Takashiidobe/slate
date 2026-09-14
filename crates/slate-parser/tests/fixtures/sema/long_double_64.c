// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -mlong-double-64

void numbers(void) {
    1.0L + 2.0L;
    1.0000000000000000000000000000000002L;
    0x1.00000000000008001p0L;
    __SIZEOF_LONG_DOUBLE__;
    __LDBL_MANT_DIG__;
    __LDBL_DECIMAL_DIG__;
    __LDBL_EPSILON__;
    __LDBL_MIN__;
    __LDBL_MAX__;
    __LDBL_DENORM_MIN__;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: add<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), const<f64>(2.0))
// CHECK-NEXT: const<f64>(1.0)
// CHECK-NEXT: const<f64>(1.0000000000000002)
// CHECK-NEXT: const<i32>(8)
// CHECK-NEXT: const<i32>(53)
// CHECK-NEXT: const<i32>(17)
// CHECK-NEXT: const<f64>(2.220446049250313e-16)
// CHECK-NEXT: const<f64>(2.2250738585072014e-308)
// CHECK-NEXT: const<f64>(1.7976931348623157e308)
// CHECK-NEXT: const<f64>(5e-324)
// SLATE-FILECHECK-END CHECK
