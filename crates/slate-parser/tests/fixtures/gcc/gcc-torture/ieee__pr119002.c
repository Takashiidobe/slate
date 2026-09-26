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
// DEFAULT-NEXT:     global %12 .str12: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @foo(%1 x: ptr<void>, %2 y: f32, %3 z: f32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %5 b: u32 [storage=automatic];
// DEFAULT-NEXT:         let %6 c: f32 [storage=automatic];
// DEFAULT-NEXT:         let %7 d: f32 [storage=automatic];
// DEFAULT-NEXT:         let %8 e: f32 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(%6, read<f32>(%2));
// DEFAULT-NEXT:         write<f32>(%7, read<f32>(%3));
// DEFAULT-NEXT:         write<u32>(%4, from_bool<u32, reason=assign>(lt<f32, exceptions=ignore>(read<f32>(%6), read<f32>(%7))));
// DEFAULT-NEXT:         write<f32>(%7, read<f32>(%2));
// DEFAULT-NEXT:         write<f32>(%8, read<f32>(%3));
// DEFAULT-NEXT:         write<u32>(%5, from_bool<u32, reason=assign>(ge<f32, exceptions=ignore>(read<f32>(%7), read<f32>(%8))));
// DEFAULT-NEXT:         let %14: u32 [synthetic] = read<u32>(%4);
// DEFAULT-NEXT:         let %15: u32 [synthetic] = or<u32>(read<u32>(%14), read<u32>(%5));
// DEFAULT-NEXT:         write<u32>(%4, read<u32>(%15));
// DEFAULT-NEXT:         return read<u32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @__builtin_nanf(%10 <unnamed>: ptr<const i8>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(ptr<void>, f32, f32) -> u32>(%0, null<ptr<void>>, const<f32>(0.0), call<f32, signature=fn(ptr<const i8>) -> f32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%12)))), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
