#include <limits.h>

/* { dg-options " -fno-trapping-math" } */
void foo (int j)
{
  int i1 = (int)(double)1.0 + INT_MAX; /* { dg-warning "integer overflow" } */
  int i2 = (int)(double)1 + INT_MAX; /* { dg-warning "integer overflow" } */
  int i3 = 1 + INT_MAX; /* { dg-warning "integer overflow" } */
  int i4 = +1 + INT_MAX; /* { dg-warning "integer overflow" } */
  int i5 = (int)((double)1.0 + INT_MAX);
  int i6 = (double)1.0 + INT_MAX; /* { dg-warning "overflow in conversion from .double. to .int. changes value" } */
  int i7 = 0 ? (int)(double)1.0 + INT_MAX : 1;
  int i8 = 1 ? 1 : (int)(double)1.0 + INT_MAX;
  int i9 = j ? (int)(double)1.0 + INT_MAX : 1; /* { dg-warning "integer overflow" } */
  unsigned int i10 = 0 ? (int)(double)1.0 + INT_MAX : 9U;
  unsigned int i11 = 1 ? 9U : (int)(double)1.0 + INT_MAX;
  unsigned int i12 = j ? (int)(double)1.0 + INT_MAX : 9U; /* { dg-warning "integer overflow" } */
  int i13 = 1 || (int)(double)1.0 + INT_MAX < 0;
  int i14 = 0 && (int)(double)1.0 + INT_MAX < 0;
  int i15 = 0 || (int)(double)1.0 + INT_MAX < 0; /* { dg-warning "integer overflow" } */
  int i16 = 1 && (int)(double)1.0 + INT_MAX < 0; /* { dg-warning "integer overflow" } */
  int i17 = j || (int)(double)1.0 + INT_MAX < 0; /* { dg-warning "integer overflow" } */
  int i18 = j && (int)(double)1.0 + INT_MAX < 0; /* { dg-warning "integer overflow" } */
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fno-trapping-math
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_j:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i1:[0-9]+]] i1: i32 [storage=automatic] = add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647));
// DEFAULT-NEXT:         let %[[VALUE_i2:[0-9]+]] i2: i32 [storage=automatic] = add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(2147483647));
// DEFAULT-NEXT:         let %[[VALUE_i3:[0-9]+]] i3: i32 [storage=automatic] = add<i32, overflow=ub>(const<i32>(1), const<i32>(2147483647));
// DEFAULT-NEXT:         let %[[VALUE_i4:[0-9]+]] i4: i32 [storage=automatic] = add<i32, overflow=ub>(const<i32>(1), const<i32>(2147483647));
// DEFAULT-NEXT:         let %[[VALUE_i5:[0-9]+]] i5: i32 [storage=automatic] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(const<f64>(1.0), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2147483647))));
// DEFAULT-NEXT:         let %[[VALUE_i6:[0-9]+]] i6: i32 [storage=automatic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(const<f64>(1.0), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2147483647))));
// DEFAULT-NEXT:         let %[[VALUE_i7:[0-9]+]] i7: i32 [storage=automatic] = conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i8:[0-9]+]] i8: i32 [storage=automatic] = conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(1), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647)));
// DEFAULT-NEXT:         let %[[VALUE_i9:[0-9]+]] i9: i32 [storage=automatic] = conditional<i32>(ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0)), add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_i10:[0-9]+]] i10: u32 [storage=automatic] = conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647))), const<u32>(9));
// DEFAULT-NEXT:         let %[[VALUE_i11:[0-9]+]] i11: u32 [storage=automatic] = conditional<u32>(ne<i32>(const<i32>(1), const<i32>(0)), const<u32>(9), reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647))));
// DEFAULT-NEXT:         let %[[VALUE_i12:[0-9]+]] i12: u32 [storage=automatic] = conditional<u32>(ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=unknown>(add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647))), const<u32>(9));
// DEFAULT-NEXT:         let %[[VALUE_i13:[0-9]+]] i13: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(const<i32>(1), const<i32>(0)), lt<i32>(add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_i14:[0-9]+]] i14: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), lt<i32>(add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_i15:[0-9]+]] i15: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(const<i32>(0), const<i32>(0)), lt<i32>(add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_i16:[0-9]+]] i16: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), lt<i32>(add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_i17:[0-9]+]] i17: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0)), lt<i32>(add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_i18:[0-9]+]] i18: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0)), lt<i32>(add<i32, overflow=ub>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
