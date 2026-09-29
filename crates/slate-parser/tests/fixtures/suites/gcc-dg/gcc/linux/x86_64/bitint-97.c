/* PR middle-end/114209 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-Og -std=c23 -fno-strict-aliasing" } */
/* { dg-add-options float128 } */
/* { dg-require-effective-target float128 } */

typedef signed char V __attribute__((__vector_size__(16)));
typedef _Float128 W __attribute__((__vector_size__(16)));

_Float128
foo (void *p)
{
  signed char c = *(_BitInt(128) *) p;
  _Float128 f = *(_Float128 *) p;
  W w = *(W *) p;
  signed char r = ((union { W a; signed char b[16]; }) w).b[1];
  return r + f;
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
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = vector<i8, 16>;
// DEFAULT-NEXT:     type @type[[TYPE_W:[0-9]+]] W = vector<f128, 1>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 a: vector<f128, 1>;
// DEFAULT-NEXT:         field1 b: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> f128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(read<i128b>(deref(pointer_cast<ptr<i128b>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: f128 [storage=automatic] = read<f128>(deref(pointer_cast<ptr<f128>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: vector<f128, 1> [storage=automatic] = read<vector<f128, 1>>(deref(pointer_cast<ptr<vector<f128, 1>>, reason=explicit>(read<ptr<void>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i8 [storage=automatic] = read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(field1(temporary %[[VALUE0:[0-9]+]] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = read<vector<f128, 1>>(%[[VALUE_w]])))), const<i32>(1))));
// DEFAULT-NEXT:         return add<f128, rounding=nearest_even, exceptions=observable, contract=off>(int_to_float<f128, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_r]]))), read<f128>(%[[VALUE_f]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
