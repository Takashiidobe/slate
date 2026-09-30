/* { dg-do run } */
/* PR tree-optimization/109386 */

static inline float foo(float x, float y) {
  float u = __builtin_fabsf(x);
  float v = __builtin_fabsf(y);
  if (!(u >= v)) {
    if (__builtin_isinf(v))
      return v;
    if (__builtin_isinf(u))
      return u;
  }
  return 42.0f;
}

int main() {
  if (!__builtin_isinf(foo(__builtin_inff(), __builtin_nanf(""))))
    __builtin_abort();
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabsf:[0-9]+]] @__builtin_fabsf(%[[VALUE0:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: f32, %[[VALUE_y:[0-9]+]] y: f32) -> f32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: f32 [storage=automatic] = call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_fabsf]], read<f32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: f32 [storage=automatic] = call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_fabsf]], read<f32>(%[[VALUE_y]]));
// DEFAULT-NEXT:         if not<bool>(ge<f32, exceptions=observable>(read<f32>(%[[VALUE_u]]), read<f32>(%[[VALUE_v]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if float_class<bool, test=infinite>(read<f32>(%[[VALUE_v]]))
// DEFAULT-NEXT:                     return read<f32>(%[[VALUE_v]]);
// DEFAULT-NEXT:                 if float_class<bool, test=infinite>(read<f32>(%[[VALUE_u]]))
// DEFAULT-NEXT:                     return read<f32>(%[[VALUE_u]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<f32>(42.0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff:[0-9]+]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf:[0-9]+]] @__builtin_nanf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=infinite>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_foo]], call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]), call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
