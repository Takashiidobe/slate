// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -ftrapping-math

void operations(void) {
    1 + (2 + 3);
    1U + 2U;
    1.0f + 2.0f;
    1.0L + 2.0L;
    1.0 < 2.0;
    1.0f == 2.0f;
    !1.0;
    1 < 2;
    (bool)0.5;
    (float)true;
    (int)0.5;
    (float)1.5;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: add<i32, overflow=ub>(const<i32>(1), add<i32, overflow=ub>(const<i32>(2), const<i32>(3)))
// CHECK-NEXT: add<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
// CHECK-NEXT: add<f32, rounding=nearest_even, exceptions=observable>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: add<f80, rounding=nearest_even, exceptions=observable>(const<f80>(1), const<f80>(2))
// CHECK-NEXT: lt<f64, exceptions=observable>(const<f64>(1.0), const<f64>(2.0))
// CHECK-NEXT: eq<f32, exceptions=observable>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: not<bool>(ne<f64, exceptions=observable>(const<f64>(1.0), const<f64>(0.0)))
// CHECK-NEXT: lt<i32>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: ne<f64, reason=explicit, exceptions=observable>(const<f64>(0.5), const<f64>(0.0))
// CHECK-NEXT: int_to_float<f32, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=explicit>(const<bool>(true)))
// CHECK-NEXT: float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(const<f64>(0.5))
// CHECK-NEXT: float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))
// SLATE-FILECHECK-END CHECK
