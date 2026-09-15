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
    0.0f;
    0.0;
    1.2345678f;
    1.2345678901234567;
    0x1p-149f;
    0x1p-1074;
    1.5f16;
    1.0000000000000000000000000000000002f128;
    1.0f16 + 2.0f16;
    1.0f128 + 2.0f128;
    1.5L;
    1.0L + 2.0L;
    3 - (2 - 1);
    1U - 2U;
    1L - 2L;
    1.0f - 2.0f;
    1.0 - (2.0 + 3.0);
    1.0f16 - 2.0f16;
    1.0f128 - 2.0f128;
    1.0L - 2.0L;
    2 * (3 / 4);
    7 % 3;
    7U * 3U;
    7U / 3U;
    7U % 3U;
    7L / 3L;
    7L % 3L;
    1.0f * 2.0f;
    1.0f / 2.0f;
    1.0 * (2.0 / 3.0);
    1.0f16 / 2.0f16;
    1.0f128 * 2.0f128;
    1.0L / 2.0L;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: const<i32>(0)
// CHECK-NEXT: const<i64>(2147483648)
// CHECK-NEXT: const<u32>(4294967295)
// CHECK-NEXT: const<u64>(18446744073709551615)
// CHECK-NEXT: add<i32, overflow=undefined>(const<i32>(1), add<i32, overflow=undefined>(const<i32>(2), const<i32>(3)))
// CHECK-NEXT: add<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
// CHECK-NEXT: add<i64, overflow=undefined>(const<i64>(1), const<i64>(2))
// CHECK-NEXT: const<f32>(0.1)
// CHECK-NEXT: const<f64>(3.0)
// CHECK-NEXT: add<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: add<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), add<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0), const<f64>(3.0)))
// CHECK-NEXT: const<f32>(0.0)
// CHECK-NEXT: const<f64>(0.0)
// CHECK-NEXT: const<f32>(1.2345678)
// CHECK-NEXT: const<f64>(1.2345678901234567)
// CHECK-NEXT: const<f32>(1e-45)
// CHECK-NEXT: const<f64>(5e-324)
// CHECK-NEXT: const<f16>(1.5)
// CHECK-NEXT: const<f128>(1.00000000000000000000000000000000019)
// CHECK-NEXT: add<f16, rounding=nearest_even, exceptions=ignore>(const<f16>(1), const<f16>(2))
// CHECK-NEXT: add<f128, rounding=nearest_even, exceptions=ignore>(const<f128>(1), const<f128>(2))
// CHECK-NEXT: const<f80>(1.5)
// CHECK-NEXT: add<f80, rounding=nearest_even, exceptions=ignore>(const<f80>(1), const<f80>(2))
// CHECK-NEXT: sub<i32, overflow=undefined>(const<i32>(3), sub<i32, overflow=undefined>(const<i32>(2), const<i32>(1)))
// CHECK-NEXT: sub<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
// CHECK-NEXT: sub<i64, overflow=undefined>(const<i64>(1), const<i64>(2))
// CHECK-NEXT: sub<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: sub<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), add<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0), const<f64>(3.0)))
// CHECK-NEXT: sub<f16, rounding=nearest_even, exceptions=ignore>(const<f16>(1), const<f16>(2))
// CHECK-NEXT: sub<f128, rounding=nearest_even, exceptions=ignore>(const<f128>(1), const<f128>(2))
// CHECK-NEXT: sub<f80, rounding=nearest_even, exceptions=ignore>(const<f80>(1), const<f80>(2))
// CHECK-NEXT: mul<i32, overflow=undefined>(const<i32>(2), div<i32, overflow=undefined>(const<i32>(3), const<i32>(4)))
// CHECK-NEXT: rem<i32, overflow=undefined>(const<i32>(7), const<i32>(3))
// CHECK-NEXT: mul<u32, overflow=wrap>(const<u32>(7), const<u32>(3))
// CHECK-NEXT: div<u32, overflow=wrap>(const<u32>(7), const<u32>(3))
// CHECK-NEXT: rem<u32, overflow=wrap>(const<u32>(7), const<u32>(3))
// CHECK-NEXT: div<i64, overflow=undefined>(const<i64>(7), const<i64>(3))
// CHECK-NEXT: rem<i64, overflow=undefined>(const<i64>(7), const<i64>(3))
// CHECK-NEXT: mul<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: div<f32, rounding=nearest_even, exceptions=ignore>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: mul<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), div<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0), const<f64>(3.0)))
// CHECK-NEXT: div<f16, rounding=nearest_even, exceptions=ignore>(const<f16>(1), const<f16>(2))
// CHECK-NEXT: mul<f128, rounding=nearest_even, exceptions=ignore>(const<f128>(1), const<f128>(2))
// CHECK-NEXT: div<f80, rounding=nearest_even, exceptions=ignore>(const<f80>(1), const<f80>(2))
// SLATE-FILECHECK-END CHECK
