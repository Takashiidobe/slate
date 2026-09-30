// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -std=c23

int bools(void) {
    1 < 2;
    1U <= 2U;
    1L > 2L;
    1 >= 2;
    1 == 2;
    1 != 2;
    (1 + 2) < 4;
    1.0f < 2.0f;
    1.0 == 2.0;
    1.0L != 2.0L;
    1.0f128 >= 2.0f128;
    !1;
    !0U;
    !0.0;
    !(1 < 2);
    !!1;
    1 && 2;
    1.0 || 0;
    1 < 2 && 3 != 4;
    1 && 2 || 3;
    true;
    false;
    !true;
    true && 1;
    return 1 == 1;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: lt<i32>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: le<u32>(const<u32>(1), const<u32>(2))
// CHECK-NEXT: gt<i64>(const<i64>(1), const<i64>(2))
// CHECK-NEXT: ge<i32>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: eq<i32>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: ne<i32>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: lt<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(2)), const<i32>(4))
// CHECK-NEXT: lt<f32, exceptions=ignore>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: eq<f64, exceptions=ignore>(const<f64>(1.0), const<f64>(2.0))
// CHECK-NEXT: ne<f80, exceptions=ignore>(const<f80>(1), const<f80>(2))
// CHECK-NEXT: ge<f128, exceptions=ignore>(const<f128>(1), const<f128>(2))
// CHECK-NEXT: not<bool>(ne<i32>(const<i32>(1), const<i32>(0)))
// CHECK-NEXT: not<bool>(ne<u32>(const<u32>(0), const<u32>(0)))
// CHECK-NEXT: not<bool>(ne<f64, exceptions=ignore>(const<f64>(0.0), const<f64>(0.0)))
// CHECK-NEXT: not<bool>(lt<i32>(const<i32>(1), const<i32>(2)))
// CHECK-NEXT: not<bool>(not<bool>(ne<i32>(const<i32>(1), const<i32>(0))))
// CHECK-NEXT: logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(const<i32>(2), const<i32>(0)))
// CHECK-NEXT: logical_or<bool>(ne<f64, exceptions=ignore>(const<f64>(1.0), const<f64>(0.0)), ne<i32>(const<i32>(0), const<i32>(0)))
// CHECK-NEXT: logical_and<bool>(lt<i32>(const<i32>(1), const<i32>(2)), ne<i32>(const<i32>(3), const<i32>(4)))
// CHECK-NEXT: logical_or<bool>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(const<i32>(2), const<i32>(0))), ne<i32>(const<i32>(3), const<i32>(0)))
// CHECK-NEXT: const<bool>(true)
// CHECK-NEXT: const<bool>(false)
// CHECK-NEXT: not<bool>(const<bool>(true))
// CHECK-NEXT: logical_and<bool>(const<bool>(true), ne<i32>(const<i32>(1), const<i32>(0)))
// CHECK-NEXT: eq<i32>(const<i32>(1), const<i32>(1))
// SLATE-FILECHECK-END CHECK
