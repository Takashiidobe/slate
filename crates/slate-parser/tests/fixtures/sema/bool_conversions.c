// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -std=c23

void conversions(void) {
    (bool)0;
    (bool)2;
    (bool)-2;
    (bool)0.0;
    (bool)-0.0;
    (bool)0.5;
    (bool)-0.5f;
    (bool)(0.0 / 0.0);
    (bool)(1.0 / 0.0);
    (bool)true;
    (int)true;
    (unsigned char)false;
    (long long)true;
    (float)true;
    (double)false;
    (long double)(1 < 2);
    (bool)(float)true;
    +true;
    -true;
    ~false;
    true + false;
    true + 2U;
    true + 0.5;
    false < 0.5f;
    true == false;
    true << false;
    true & false;
    (unsigned int)(signed char)-1;
    (signed char)256;
    (int)0.5;
    (float)1.5;
    (double)1.5f;
    (short)true + (unsigned char)false;
    (1 < 2) + 1;
    (1 < 2) == (3 < 4);
    -true;
    1 + 2U;
    1.0f - 2.0;
    1 & 2U;
    1 < 2U;
    (bool)0U;
    (bool)0.25L;
    (bool)(_Float16)0.25;
    (_Float16)true;
    1L + 2U;
    1U + 2L;
    1LL + 2ULL;
    (short)1 << 2ULL;
    (unsigned char)255 + 1;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: ne<i32, reason=explicit>(const<i32>(0), const<i32>(0))
// CHECK-NEXT: ne<i32, reason=explicit>(const<i32>(2), const<i32>(0))
// CHECK-NEXT: ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(2)), const<i32>(0))
// CHECK-NEXT: ne<f64, reason=explicit, exceptions=ignore>(const<f64>(0.0), const<f64>(0.0))
// CHECK-NEXT: ne<f64, reason=explicit, exceptions=ignore>(neg<f64>(const<f64>(0.0)), const<f64>(0.0))
// CHECK-NEXT: ne<f64, reason=explicit, exceptions=ignore>(const<f64>(0.5), const<f64>(0.0))
// CHECK-NEXT: ne<f32, reason=explicit, exceptions=ignore>(neg<f32>(const<f32>(0.5)), const<f32>(0.0))
// CHECK-NEXT: ne<f64, reason=explicit, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0), const<f64>(0.0)), const<f64>(0.0))
// CHECK-NEXT: ne<f64, reason=explicit, exceptions=ignore>(div<f64, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0), const<f64>(0.0)), const<f64>(0.0))
// CHECK-NEXT: const<bool>(true)
// CHECK-NEXT: from_bool<i32, reason=explicit>(const<bool>(true))
// CHECK-NEXT: from_bool<u8, reason=explicit>(const<bool>(false))
// CHECK-NEXT: from_bool<i64, reason=explicit>(const<bool>(true))
// CHECK-NEXT: int_to_float<f32, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=explicit>(const<bool>(true)))
// CHECK-NEXT: int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=explicit>(const<bool>(false)))
// CHECK-NEXT: int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=explicit>(lt<i32>(const<i32>(1), const<i32>(2))))
// CHECK-NEXT: ne<f32, reason=explicit, exceptions=ignore>(int_to_float<f32, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=explicit>(const<bool>(true))), const<f32>(0.0))
// CHECK-NEXT: from_bool<i32, reason=promotion>(const<bool>(true))
// CHECK-NEXT: neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(const<bool>(true)))
// CHECK-NEXT: not<i32>(from_bool<i32, reason=promotion>(const<bool>(false)))
// CHECK-NEXT: add<i32, overflow=ub>(from_bool<i32, reason=promotion>(const<bool>(true)), from_bool<i32, reason=promotion>(const<bool>(false)))
// CHECK-NEXT: add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(const<bool>(true))), const<u32>(2))
// CHECK-NEXT: add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=promotion>(const<bool>(true))), const<f64>(0.5))
// CHECK-NEXT: lt<f32, exceptions=ignore>(int_to_float<f32, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=promotion>(const<bool>(false))), const<f32>(0.5))
// CHECK-NEXT: eq<i32>(from_bool<i32, reason=promotion>(const<bool>(true)), from_bool<i32, reason=promotion>(const<bool>(false)))
// CHECK-NEXT: shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(from_bool<i32, reason=promotion>(const<bool>(true)), from_bool<i32, reason=promotion>(const<bool>(false)))
// CHECK-NEXT: and<i32>(from_bool<i32, reason=promotion>(const<bool>(true)), from_bool<i32, reason=promotion>(const<bool>(false)))
// CHECK-NEXT: reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))
// CHECK-NEXT: truncate<i8, reason=explicit, fits=unknown>(const<i32>(256))
// CHECK-NEXT: float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(0.5))
// CHECK-NEXT: float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))
// CHECK-NEXT: float_widen<f64, reason=explicit>(const<f32>(1.5))
// CHECK-NEXT: add<i32, overflow=ub>(widen<i32, reason=promotion>(from_bool<i16, reason=explicit>(const<bool>(true))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(from_bool<u8, reason=explicit>(const<bool>(false)))))
// CHECK-NEXT: add<i32, overflow=ub>(from_bool<i32, reason=promotion>(lt<i32>(const<i32>(1), const<i32>(2))), const<i32>(1))
// CHECK-NEXT: eq<i32>(from_bool<i32, reason=promotion>(lt<i32>(const<i32>(1), const<i32>(2))), from_bool<i32, reason=promotion>(lt<i32>(const<i32>(3), const<i32>(4))))
// CHECK-NEXT: neg<i32, overflow=ub>(from_bool<i32, reason=promotion>(const<bool>(true)))
// CHECK-NEXT: add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), const<u32>(2))
// CHECK-NEXT: sub<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(const<f32>(1.0)), const<f64>(2.0))
// CHECK-NEXT: and<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), const<u32>(2))
// CHECK-NEXT: lt<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), const<u32>(2))
// CHECK-NEXT: ne<u32, reason=explicit>(const<u32>(0), const<u32>(0))
// CHECK-NEXT: ne<f80, reason=explicit, exceptions=ignore>(const<f80>(0.25), const<f80>(0))
// CHECK-NEXT: ne<f16, reason=explicit, exceptions=ignore>(float_narrow<f16, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(0.25)), const<f16>(0))
// CHECK-NEXT: int_to_float<f16, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=explicit>(const<bool>(true)))
// CHECK-NEXT: add<i64, overflow=ub>(const<i64>(1), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(const<u32>(2))))
// CHECK-NEXT: add<i64, overflow=ub>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(const<u32>(1))), const<i64>(2))
// CHECK-NEXT: add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1)), const<u64>(2))
// CHECK-NEXT: shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<u64>(2))
// CHECK-NEXT: add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255))))), const<i32>(1))
// SLATE-FILECHECK-END CHECK
