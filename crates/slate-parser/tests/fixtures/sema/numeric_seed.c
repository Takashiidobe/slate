// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions

void numbers(void) {
    0;
    2147483648;
    0xffffffff;
    18446744073709551615ULL;
    1 + (2 + 3);
    1U + 2U;
    1L + 2L;
    0.1f;
    0x1.8p+1;
    1.0f + 2.0f;
    1.0 + (2.0 + 3.0);
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: const<i32>(0)
// CHECK-NEXT: const<i64>(2147483648)
// CHECK-NEXT: const<u32>(4294967295)
// CHECK-NEXT: const<u64>(18446744073709551615)
// CHECK-NEXT: add<i32, overflow=undefined>(const<i32>(1), add<i32, overflow=undefined>(const<i32>(2), const<i32>(3)))
// CHECK-NEXT: add<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
// CHECK-NEXT: add<i64, overflow=undefined>(const<i64>(1), const<i64>(2))
// CHECK-NEXT: const<f32>(bits=0x3dcccccd)
// CHECK-NEXT: const<f64>(bits=0x4008000000000000)
// CHECK-NEXT: add<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(bits=0x3f800000), const<f32>(bits=0x40000000))
// CHECK-NEXT: add<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(bits=0x3ff0000000000000), add<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(bits=0x4000000000000000), const<f64>(bits=0x4008000000000000)))
// SLATE-FILECHECK-END CHECK
