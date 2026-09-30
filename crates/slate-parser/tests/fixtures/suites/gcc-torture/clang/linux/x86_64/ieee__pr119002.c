/* { dg-do run } */
/* PR rtl-optimization/119002 */

__attribute__((noipa)) unsigned int foo(void *x, float y, float z) {
  unsigned int a, b;
  float        c, d, e;
  c  = y;
  d  = z;
  a  = c < d;
  d  = y;
  e  = z;
  b  = d >= e;
  a |= b;
  return a;
}

int main() {
  if (foo((void *)0, 0.f, __builtin_nanf("")))
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<void>, %[[VALUE_y:[0-9]+]] y: f32, %[[VALUE_z:[0-9]+]] z: f32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: f32 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(%[[VALUE_c]], read<f32>(%[[VALUE_y]]));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_d]], read<f32>(%[[VALUE_z]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a]], from_bool<u32, reason=assign>(lt<f32, exceptions=ignore>(read<f32>(%[[VALUE_c]]), read<f32>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_d]], read<f32>(%[[VALUE_y]]));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_e]], read<f32>(%[[VALUE_z]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_b]], from_bool<u32, reason=assign>(ge<f32, exceptions=ignore>(read<f32>(%[[VALUE_d]]), read<f32>(%[[VALUE_e]]))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = or<u32>(read<u32>(%[[VALUE0]]), read<u32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a]], read<u32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf:[0-9]+]] @__builtin_nanf(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(ptr<void>, f32, f32) -> u32>(%[[VALUE_foo]], null<ptr<void>>, const<f32>(0.0), call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])))), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
