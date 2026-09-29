/* { dg-do run } */
/* PR tree-optimization/111109 */

/*
   f should return 0 if either fa and fb are a nan.
   Rather than the value of a or b.
*/
__attribute__((noipa)) int f(int a, int b, float fa, float fb) {
  const _Bool c  = fa < fb;
  const _Bool c1 = fa >= fb;
  return (c * a) | (c1 * b);
}

/*
   f1 should return 0 if either fa and fb are a nan.
   Rather than the value of a&1 or b&1.
*/
__attribute__((noipa)) int f1(int a, int b, float fa, float fb) {
  const _Bool c  = fa < fb;
  const _Bool c1 = fa >= fb;
  return (c & a) | (c1 & b);
}

#if __SIZEOF_INT__ == __SIZEOF_FLOAT__
typedef int   v4si __attribute__((vector_size(1 * sizeof(int))));
typedef float v4sf __attribute__((vector_size(1 * sizeof(float))));
/*
   fvf0 should return {0} if either fa and fb are a nan.
   Rather than the value of a or b.
*/
__attribute__((noipa)) v4si vf0(v4si a, v4si b, v4sf fa, v4sf fb) {
  const v4si c  = fa < fb;
  const v4si c1 = fa >= fb;
  return (c & a) | (c1 & b);
}

#endif

int main(void) {
  float a = __builtin_nan("");

  if (f(-1, -1, a, a) != 0)
    __builtin_abort();
  if (f(-1, -1, a, 0) != 0)
    __builtin_abort();
  if (f(-1, -1, 0, a) != 0)
    __builtin_abort();
  if (f(-1, -1, 0, 0) != -1)
    __builtin_abort();

  if (f1(1, 1, a, a) != 0)
    __builtin_abort();
  if (f1(1, 1, a, 0) != 0)
    __builtin_abort();
  if (f1(1, 1, 0, a) != 0)
    __builtin_abort();
  if (f1(1, 1, 0, 0) != 1)
    __builtin_abort();

#if __SIZEOF_INT__ == __SIZEOF_FLOAT__
  v4si b = {-1};
  v4sf c = {a};
  v4sf d = {0.0};
  if (vf0(b, b, c, c)[0] != 0)
    __builtin_abort();
  if (vf0(b, b, c, d)[0] != 0)
    __builtin_abort();
  if (vf0(b, b, d, c)[0] != 0)
    __builtin_abort();
  if (vf0(b, b, d, d)[0] != b[0])
    __builtin_abort();
#endif
}


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
// DEFAULT-NEXT:     type @type[[TYPE_v4si:[0-9]+]] v4si = vector<i32, 1>;
// DEFAULT-NEXT:     type @type[[TYPE_v4sf:[0-9]+]] v4sf = vector<f32, 1>;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_fa:[0-9]+]] fa: f32, %[[VALUE_fb:[0-9]+]] fb: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: bool [storage=automatic] [const] = lt<f32, exceptions=observable>(read<f32>(%[[VALUE_fa]]), read<f32>(%[[VALUE_fb]]));
// DEFAULT-NEXT:         let %[[VALUE_c1:[0-9]+]] c1: bool [storage=automatic] [const] = ge<f32, exceptions=observable>(read<f32>(%[[VALUE_fa]]), read<f32>(%[[VALUE_fb]]));
// DEFAULT-NEXT:         return or<i32>(mul<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_c]])), read<i32>(%[[VALUE_a]])), mul<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_c1]])), read<i32>(%[[VALUE_b]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32, %[[VALUE_fa_2:[0-9]+]] fa: f32, %[[VALUE_fb_2:[0-9]+]] fb: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: bool [storage=automatic] [const] = lt<f32, exceptions=observable>(read<f32>(%[[VALUE_fa_2]]), read<f32>(%[[VALUE_fb_2]]));
// DEFAULT-NEXT:         let %[[VALUE_c1_2:[0-9]+]] c1: bool [storage=automatic] [const] = ge<f32, exceptions=observable>(read<f32>(%[[VALUE_fa_2]]), read<f32>(%[[VALUE_fb_2]]));
// DEFAULT-NEXT:         return or<i32>(and<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_c_2]])), read<i32>(%[[VALUE_a_2]])), and<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_c1_2]])), read<i32>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_vf0:[0-9]+]] @vf0(%[[VALUE_a_3:[0-9]+]] a: vector<i32, 1>, %[[VALUE_b_3:[0-9]+]] b: vector<i32, 1>, %[[VALUE_fa_3:[0-9]+]] fa: vector<f32, 1>, %[[VALUE_fb_3:[0-9]+]] fb: vector<f32, 1>) -> vector<i32, 1> [linkage=external] [abi=sysv64(coerce<i32>, coerce<i32>, coerce<i32>, coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_3:[0-9]+]] c: vector<i32, 1> [storage=automatic] [const] = lt<vector<f32, 1>, result=vector<i32, 1>, exceptions=observable>(read<vector<f32, 1>>(%[[VALUE_fa_3]]), read<vector<f32, 1>>(%[[VALUE_fb_3]]));
// DEFAULT-NEXT:         let %[[VALUE_c1_3:[0-9]+]] c1: vector<i32, 1> [storage=automatic] [const] = ge<vector<f32, 1>, result=vector<i32, 1>, exceptions=observable>(read<vector<f32, 1>>(%[[VALUE_fa_3]]), read<vector<f32, 1>>(%[[VALUE_fb_3]]));
// DEFAULT-NEXT:         return or<vector<i32, 1>, elementwise=true>(and<vector<i32, 1>, elementwise=true>(read<vector<i32, 1>>(%[[VALUE_c_3]]), read<vector<i32, 1>>(%[[VALUE_a_3]])), and<vector<i32, 1>, elementwise=true>(read<vector<i32, 1>>(%[[VALUE_c1_3]]), read<vector<i32, 1>>(%[[VALUE_b_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan:[0-9]+]] @__builtin_nan(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_4:[0-9]+]] a: f32 [storage=automatic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, f32, f32) -> i32>(%[[VALUE_f]], neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1)), read<f32>(%[[VALUE_a_4]]), read<f32>(%[[VALUE_a_4]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, f32, f32) -> i32>(%[[VALUE_f]], neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1)), read<f32>(%[[VALUE_a_4]]), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, f32, f32) -> i32>(%[[VALUE_f]], neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), read<f32>(%[[VALUE_a_4]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, f32, f32) -> i32>(%[[VALUE_f]], neg<i32, overflow=ub>(const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, f32, f32) -> i32>(%[[VALUE_f1]], const<i32>(1), const<i32>(1), read<f32>(%[[VALUE_a_4]]), read<f32>(%[[VALUE_a_4]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, f32, f32) -> i32>(%[[VALUE_f1]], const<i32>(1), const<i32>(1), read<f32>(%[[VALUE_a_4]]), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, f32, f32) -> i32>(%[[VALUE_f1]], const<i32>(1), const<i32>(1), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), read<f32>(%[[VALUE_a_4]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, f32, f32) -> i32>(%[[VALUE_f1]], const<i32>(1), const<i32>(1), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_b_4:[0-9]+]] b: vector<i32, 1> [storage=automatic] = aggregate<vector<i32, 1>, zero_fill=false>(index0 = neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_c_4:[0-9]+]] c: vector<f32, 1> [storage=automatic] = aggregate<vector<f32, 1>, zero_fill=false>(index0 = read<f32>(%[[VALUE_a_4]]));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: vector<f32, 1> [storage=automatic] = aggregate<vector<f32, 1>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(const<f64>(0.0)));
// DEFAULT-NEXT:         if ne<i32>(lane<i32>(call<vector<i32, 1>, signature=fn(vector<i32, 1>, vector<i32, 1>, vector<f32, 1>, vector<f32, 1>) -> vector<i32, 1>, abi=sysv64(coerce<i32>, coerce<i32>, coerce<i32>, coerce<i32>) -> coerce<i32>>(%[[VALUE_vf0]], read<vector<i32, 1>>(%[[VALUE_b_4]]), read<vector<i32, 1>>(%[[VALUE_b_4]]), read<vector<f32, 1>>(%[[VALUE_c_4]]), read<vector<f32, 1>>(%[[VALUE_c_4]])), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(lane<i32>(call<vector<i32, 1>, signature=fn(vector<i32, 1>, vector<i32, 1>, vector<f32, 1>, vector<f32, 1>) -> vector<i32, 1>, abi=sysv64(coerce<i32>, coerce<i32>, coerce<i32>, coerce<i32>) -> coerce<i32>>(%[[VALUE_vf0]], read<vector<i32, 1>>(%[[VALUE_b_4]]), read<vector<i32, 1>>(%[[VALUE_b_4]]), read<vector<f32, 1>>(%[[VALUE_c_4]]), read<vector<f32, 1>>(%[[VALUE_d]])), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(lane<i32>(call<vector<i32, 1>, signature=fn(vector<i32, 1>, vector<i32, 1>, vector<f32, 1>, vector<f32, 1>) -> vector<i32, 1>, abi=sysv64(coerce<i32>, coerce<i32>, coerce<i32>, coerce<i32>) -> coerce<i32>>(%[[VALUE_vf0]], read<vector<i32, 1>>(%[[VALUE_b_4]]), read<vector<i32, 1>>(%[[VALUE_b_4]]), read<vector<f32, 1>>(%[[VALUE_d]]), read<vector<f32, 1>>(%[[VALUE_c_4]])), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(lane<i32>(call<vector<i32, 1>, signature=fn(vector<i32, 1>, vector<i32, 1>, vector<f32, 1>, vector<f32, 1>) -> vector<i32, 1>, abi=sysv64(coerce<i32>, coerce<i32>, coerce<i32>, coerce<i32>) -> coerce<i32>>(%[[VALUE_vf0]], read<vector<i32, 1>>(%[[VALUE_b_4]]), read<vector<i32, 1>>(%[[VALUE_b_4]]), read<vector<f32, 1>>(%[[VALUE_d]]), read<vector<f32, 1>>(%[[VALUE_d]])), const<i32>(0)), read<i32>(lane(%[[VALUE_b_4]], const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
