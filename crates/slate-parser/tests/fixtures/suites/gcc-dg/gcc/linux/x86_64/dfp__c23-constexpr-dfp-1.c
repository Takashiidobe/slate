/* Test C23 constexpr.  Valid code, compilation tests, DFP.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

constexpr _Decimal32 v1 = __DEC32_MIN__;
constexpr _Decimal32 v2 = __DEC32_SUBNORMAL_MIN__;
constexpr _Decimal32 v3 = -__DEC32_MAX__;
constexpr _Decimal64 v4 = __DEC32_MIN__;
constexpr _Decimal64 v5 = __DEC32_SUBNORMAL_MIN__;
constexpr _Decimal64 v6 = -__DEC32_MAX__;
constexpr _Decimal64 v7 = __DEC64_MIN__;
constexpr _Decimal64 v8 = __DEC64_SUBNORMAL_MIN__;
constexpr _Decimal64 v9 = -__DEC64_MAX__;
constexpr _Decimal128 v10 = __DEC32_MIN__;
constexpr _Decimal128 v11 = __DEC32_SUBNORMAL_MIN__;
constexpr _Decimal128 v12 = -__DEC32_MAX__;
constexpr _Decimal128 v13 = __DEC64_MIN__;
constexpr _Decimal128 v14 = __DEC64_SUBNORMAL_MIN__;
constexpr _Decimal128 v15 = -__DEC64_MAX__;
constexpr _Decimal128 v16 = __DEC128_MIN__;
constexpr _Decimal128 v17 = __DEC128_SUBNORMAL_MIN__;
constexpr _Decimal128 v18 = -__DEC128_MAX__;
constexpr _Decimal32 v19 = 1234567L;
constexpr _Decimal32 v20 = -123456700000LL;
constexpr _Decimal64 v21 = 1234567890123456LL;
constexpr _Decimal64 v22 = -123456789012345600LL;
constexpr _Decimal128 v23 = (unsigned long long) -1;
constexpr _Decimal32 v24 = 1e-101DL;
constexpr _Decimal64 v25 = 1e-398DL;
constexpr _Decimal32 v26 = __builtin_infd128 ();
constexpr _Decimal128 v27 = __builtin_infd32 ();
constexpr _Decimal64 v28 = __builtin_nand128 ("");
constexpr _Decimal128 v29 = __builtin_nand32 ("");
constexpr _Decimal32 v30 = __builtin_nansd32 ("");
constexpr _Decimal64 v31 = __builtin_nansd64 ("");
constexpr _Decimal128 v32 = __builtin_nansd128 ("");
constexpr _Decimal32 v33 = {};
constexpr _Decimal64 v34 = {};
constexpr _Decimal128 v35 = {};
constexpr _Decimal32 v36 = 0.0;
constexpr _Decimal32 v37 = 0.0009765625;
constexpr _Decimal64 v38 = 6.103515625e-05;
constexpr _Decimal32 v39 = __builtin_inf ();
constexpr _Decimal32 v40 = __builtin_nan ("");

void
f0 ()
{
  (constexpr _Decimal32) { __DEC32_MIN__ };
  (constexpr _Decimal32) { __DEC32_SUBNORMAL_MIN__ };
  (constexpr _Decimal32) { -__DEC32_MAX__ };
  (constexpr _Decimal64) { __DEC32_MIN__ };
  (constexpr _Decimal64) { __DEC32_SUBNORMAL_MIN__ };
  (constexpr _Decimal64) { -__DEC32_MAX__ };
  (constexpr _Decimal64) { __DEC64_MIN__ };
  (constexpr _Decimal64) { __DEC64_SUBNORMAL_MIN__ };
  (constexpr _Decimal64) { -__DEC64_MAX__ };
  (constexpr _Decimal128) { __DEC32_MIN__ };
  (constexpr _Decimal128) { __DEC32_SUBNORMAL_MIN__ };
  (constexpr _Decimal128) { -__DEC32_MAX__ };
  (constexpr _Decimal128) { __DEC64_MIN__ };
  (constexpr _Decimal128) { __DEC64_SUBNORMAL_MIN__ };
  (constexpr _Decimal128) { -__DEC64_MAX__ };
  (constexpr _Decimal128) { __DEC128_MIN__ };
  (constexpr _Decimal128) { __DEC128_SUBNORMAL_MIN__ };
  (constexpr _Decimal128) { -__DEC128_MAX__ };
  (constexpr _Decimal32) { 1234567L };
  (constexpr _Decimal32) { -123456700000LL };
  (constexpr _Decimal64) { 1234567890123456LL };
  (constexpr _Decimal64) { -123456789012345600LL };
  (constexpr _Decimal128) { (unsigned long long) -1 };
  (constexpr _Decimal32) { 1e-101DL };
  (constexpr _Decimal64) { 1e-398DL };
  (constexpr _Decimal32) { __builtin_infd128 () };
  (constexpr _Decimal128) { __builtin_infd32 () };
  (constexpr _Decimal64) { __builtin_nand128 ("") };
  (constexpr _Decimal128) { __builtin_nand32 ("") };
  (constexpr _Decimal32) { __builtin_nansd32 ("") };
  (constexpr _Decimal64) { __builtin_nansd64 ("") };
  (constexpr _Decimal128) { __builtin_nansd128 ("") };
  (constexpr _Decimal32) {};
  (constexpr _Decimal64) {};
  (constexpr _Decimal128) {};
  (constexpr _Decimal32) { 0.0 };
  (constexpr _Decimal32) { 0.0009765625 };
  (constexpr _Decimal64) { 6.103515625e-05 };
  (constexpr _Decimal32) { __builtin_inf () };
  (constexpr _Decimal32) { __builtin_nan ("") };
}

// SLATE-FILECHECK-STD DEFAULT c23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: d32 [storage=static] [const] [constexpr] = const<d32>(1e-95) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v2:[0-9]+]] v2: d32 [storage=static] [const] [constexpr] = const<d32>(0.000001e-95) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v3:[0-9]+]] v3: d32 [storage=static] [const] [constexpr] = neg<d32>(const<d32>(9.999999e96)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v4:[0-9]+]] v4: d64 [storage=static] [const] [constexpr] = float_widen<d64, reason=assign>(const<d32>(1e-95)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v5:[0-9]+]] v5: d64 [storage=static] [const] [constexpr] = float_widen<d64, reason=assign>(const<d32>(0.000001e-95)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v6:[0-9]+]] v6: d64 [storage=static] [const] [constexpr] = float_widen<d64, reason=assign>(neg<d32>(const<d32>(9.999999e96))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v7:[0-9]+]] v7: d64 [storage=static] [const] [constexpr] = const<d64>(1e-383) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v8:[0-9]+]] v8: d64 [storage=static] [const] [constexpr] = const<d64>(0.000000000000001e-383) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v9:[0-9]+]] v9: d64 [storage=static] [const] [constexpr] = neg<d64>(const<d64>(9.999999999999999e384)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v10:[0-9]+]] v10: d128 [storage=static] [const] [constexpr] = float_widen<d128, reason=assign>(const<d32>(1e-95)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v11:[0-9]+]] v11: d128 [storage=static] [const] [constexpr] = float_widen<d128, reason=assign>(const<d32>(0.000001e-95)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v12:[0-9]+]] v12: d128 [storage=static] [const] [constexpr] = float_widen<d128, reason=assign>(neg<d32>(const<d32>(9.999999e96))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v13:[0-9]+]] v13: d128 [storage=static] [const] [constexpr] = float_widen<d128, reason=assign>(const<d64>(1e-383)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v14:[0-9]+]] v14: d128 [storage=static] [const] [constexpr] = float_widen<d128, reason=assign>(const<d64>(0.000000000000001e-383)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v15:[0-9]+]] v15: d128 [storage=static] [const] [constexpr] = float_widen<d128, reason=assign>(neg<d64>(const<d64>(9.999999999999999e384))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v16:[0-9]+]] v16: d128 [storage=static] [const] [constexpr] = const<d128>(1e-6143) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v17:[0-9]+]] v17: d128 [storage=static] [const] [constexpr] = const<d128>(0.000000000000000000000000000000001e-6143) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v18:[0-9]+]] v18: d128 [storage=static] [const] [constexpr] = neg<d128>(const<d128>(9.999999999999999999999999999999999e6144)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v19:[0-9]+]] v19: d32 [storage=static] [const] [constexpr] = int_to_float<d32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i64>(1234567)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v20:[0-9]+]] v20: d32 [storage=static] [const] [constexpr] = int_to_float<d32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i64, overflow=ub>(const<i64>(123456700000))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v21:[0-9]+]] v21: d64 [storage=static] [const] [constexpr] = int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i64>(1234567890123456)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v22:[0-9]+]] v22: d64 [storage=static] [const] [constexpr] = int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i64, overflow=ub>(const<i64>(123456789012345600))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v23:[0-9]+]] v23: d128 [storage=static] [const] [constexpr] = int_to_float<d128, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v24:[0-9]+]] v24: d32 [storage=static] [const] [constexpr] = float_narrow<d32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<d128>(1e-101)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v25:[0-9]+]] v25: d64 [storage=static] [const] [constexpr] = float_narrow<d64, reason=assign, rounding=nearest_even, exceptions=ignore>(const<d128>(1e-398)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v26:[0-9]+]] v26: d32 [storage=static] [const] [constexpr] = float_narrow<d32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<d128, signature=fn() -> d128>(%[[VALUE___builtin_infd128:[0-9]+]])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v27:[0-9]+]] v27: d128 [storage=static] [const] [constexpr] = float_widen<d128, reason=assign>(call<d32, signature=fn() -> d32>(%[[VALUE___builtin_infd32:[0-9]+]])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v28:[0-9]+]] v28: d64 [storage=static] [const] [constexpr] = float_narrow<d64, reason=assign, rounding=nearest_even, exceptions=ignore>(call<d128, signature=fn(ptr<const i8>) -> d128>(%[[VALUE_nand128:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v29:[0-9]+]] v29: d128 [storage=static] [const] [constexpr] = float_widen<d128, reason=assign>(call<d32, signature=fn(ptr<const i8>) -> d32>(%[[VALUE_nand32:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v30:[0-9]+]] v30: d32 [storage=static] [const] [constexpr] = call<d32, signature=fn(ptr<const i8>) -> d32>(%[[VALUE___builtin_nansd32:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]]))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v31:[0-9]+]] v31: d64 [storage=static] [const] [constexpr] = call<d64, signature=fn(ptr<const i8>) -> d64>(%[[VALUE___builtin_nansd64:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_4]]))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v32:[0-9]+]] v32: d128 [storage=static] [const] [constexpr] = call<d128, signature=fn(ptr<const i8>) -> d128>(%[[VALUE___builtin_nansd128:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_5]]))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v33:[0-9]+]] v33: d32 [storage=static] [const] [constexpr] = aggregate<d32, zero_fill=true>() [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v34:[0-9]+]] v34: d64 [storage=static] [const] [constexpr] = aggregate<d64, zero_fill=true>() [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v35:[0-9]+]] v35: d128 [storage=static] [const] [constexpr] = aggregate<d128, zero_fill=true>() [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v36:[0-9]+]] v36: d32 [storage=static] [const] [constexpr] = float_convert<d32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v37:[0-9]+]] v37: d32 [storage=static] [const] [constexpr] = float_convert<d32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0009765625)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v38:[0-9]+]] v38: d64 [storage=static] [const] [constexpr] = float_convert<d64, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(6.103515625e-5)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v39:[0-9]+]] v39: d32 [storage=static] [const] [constexpr] = float_convert<d32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(inf)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v40:[0-9]+]] v40: d32 [storage=static] [const] [constexpr] = float_convert<d32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(bits=0x7ff8000000000000)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infd128]] @__builtin_infd128() -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infd32]] @__builtin_infd32() -> d32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nand128]] @nand128(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nand32]] @nand32(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> d32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd32]] @__builtin_nansd32(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> d32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd64]] @__builtin_nansd64(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>) -> d64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd128]] @__builtin_nansd128(%[[VALUE4:[0-9]+]] <unnamed>: ptr<const i8>) -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_f0:[0-9]+]] @f0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<d32>(1e-95);
// DEFAULT-NEXT:         const<d32>(0.000001e-95);
// DEFAULT-NEXT:         neg<d32>(const<d32>(9.999999e96));
// DEFAULT-NEXT:         float_widen<d64, reason=assign>(const<d32>(1e-95));
// DEFAULT-NEXT:         float_widen<d64, reason=assign>(const<d32>(0.000001e-95));
// DEFAULT-NEXT:         float_widen<d64, reason=assign>(neg<d32>(const<d32>(9.999999e96)));
// DEFAULT-NEXT:         const<d64>(1e-383);
// DEFAULT-NEXT:         const<d64>(0.000000000000001e-383);
// DEFAULT-NEXT:         neg<d64>(const<d64>(9.999999999999999e384));
// DEFAULT-NEXT:         float_widen<d128, reason=assign>(const<d32>(1e-95));
// DEFAULT-NEXT:         float_widen<d128, reason=assign>(const<d32>(0.000001e-95));
// DEFAULT-NEXT:         float_widen<d128, reason=assign>(neg<d32>(const<d32>(9.999999e96)));
// DEFAULT-NEXT:         float_widen<d128, reason=assign>(const<d64>(1e-383));
// DEFAULT-NEXT:         float_widen<d128, reason=assign>(const<d64>(0.000000000000001e-383));
// DEFAULT-NEXT:         float_widen<d128, reason=assign>(neg<d64>(const<d64>(9.999999999999999e384)));
// DEFAULT-NEXT:         const<d128>(1e-6143);
// DEFAULT-NEXT:         const<d128>(0.000000000000000000000000000000001e-6143);
// DEFAULT-NEXT:         neg<d128>(const<d128>(9.999999999999999999999999999999999e6144));
// DEFAULT-NEXT:         int_to_float<d32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i64>(1234567));
// DEFAULT-NEXT:         int_to_float<d32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(neg<i64, overflow=ub>(const<i64>(123456700000)));
// DEFAULT-NEXT:         int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i64>(1234567890123456));
// DEFAULT-NEXT:         int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(neg<i64, overflow=ub>(const<i64>(123456789012345600)));
// DEFAULT-NEXT:         int_to_float<d128, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         float_narrow<d32, reason=assign, rounding=nearest_even, exceptions=observable>(const<d128>(1e-101));
// DEFAULT-NEXT:         float_narrow<d64, reason=assign, rounding=nearest_even, exceptions=observable>(const<d128>(1e-398));
// DEFAULT-NEXT:         read<d32>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = float_narrow<d32, reason=assign, rounding=nearest_even, exceptions=observable>(call<d128, signature=fn() -> d128>(%[[VALUE___builtin_infd128]])));
// DEFAULT-NEXT:         read<d128>(compound_literal %[[VALUE6:[0-9]+]] [storage=automatic] = float_widen<d128, reason=assign>(call<d32, signature=fn() -> d32>(%[[VALUE___builtin_infd32]])));
// DEFAULT-NEXT:         read<d64>(compound_literal %[[VALUE7:[0-9]+]] [storage=automatic] = float_narrow<d64, reason=assign, rounding=nearest_even, exceptions=observable>(call<d128, signature=fn(ptr<const i8>) -> d128>(%[[VALUE_nand128]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_6]])))));
// DEFAULT-NEXT:         read<d128>(compound_literal %[[VALUE8:[0-9]+]] [storage=automatic] = float_widen<d128, reason=assign>(call<d32, signature=fn(ptr<const i8>) -> d32>(%[[VALUE_nand32]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_7]])))));
// DEFAULT-NEXT:         read<d32>(compound_literal %[[VALUE9:[0-9]+]] [storage=automatic] = call<d32, signature=fn(ptr<const i8>) -> d32>(%[[VALUE___builtin_nansd32]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_8]]))));
// DEFAULT-NEXT:         read<d64>(compound_literal %[[VALUE10:[0-9]+]] [storage=automatic] = call<d64, signature=fn(ptr<const i8>) -> d64>(%[[VALUE___builtin_nansd64]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_9]]))));
// DEFAULT-NEXT:         read<d128>(compound_literal %[[VALUE11:[0-9]+]] [storage=automatic] = call<d128, signature=fn(ptr<const i8>) -> d128>(%[[VALUE___builtin_nansd128]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_10]]))));
// DEFAULT-NEXT:         int_to_float<d32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         int_to_float<d64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         int_to_float<d128, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0));
// DEFAULT-NEXT:         float_convert<d32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(0.0));
// DEFAULT-NEXT:         float_convert<d32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(0.0009765625));
// DEFAULT-NEXT:         float_convert<d64, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(6.103515625e-5));
// DEFAULT-NEXT:         read<d32>(compound_literal %[[VALUE12:[0-9]+]] [storage=automatic] = float_convert<d32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(inf)));
// DEFAULT-NEXT:         read<d32>(compound_literal %[[VALUE13:[0-9]+]] [storage=automatic] = float_convert<d32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(bits=0x7ff8000000000000)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
