// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -frounding-math

void operations(void) {
    1 + (2 + 3);
    1U + 2U;
    1.0f + 2.0f;
    1.0L + 2.0L;
    (float)true;
    (float)16777217;
    (float)1.5;
    (int)1.5;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: add<i32, overflow=undefined>(const<i32>(1), add<i32, overflow=undefined>(const<i32>(2), const<i32>(3)))
// CHECK-NEXT: add<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
// CHECK-NEXT: add<f32, rounding=environment, exceptions=ignore>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: add<f80, rounding=environment, exceptions=ignore>(const<f80>(1), const<f80>(2))
// CHECK-NEXT: int_to_float<f32, reason=explicit, exact=true, rounding=environment, exceptions=ignore>(from_bool<i32, reason=explicit>(const<bool>(true)))
// CHECK-NEXT: int_to_float<f32, reason=explicit, exact=false, rounding=environment, exceptions=ignore>(const<i32>(16777217))
// CHECK-NEXT: float_narrow<f32, reason=explicit, rounding=environment, exceptions=ignore>(const<f64>(1.5))
// CHECK-NEXT: float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.5))
// SLATE-FILECHECK-END CHECK
