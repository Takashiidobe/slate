/* { dg-do run } */
/* PR tree-optimization/72824 */

typedef float V __attribute__((vector_size(4 * sizeof(float))));

static inline void foo(V *x, V value) {
  int i;
  for (i = 0; i < 32; ++i)
    x[i] = value;
}

int main() {
  V x[32];
  foo(x, (V){0.f, -0.f, 0.f, -0.f});
  if (__builtin_copysignf(1.0, x[3][1]) != -1.0f)
    __builtin_abort();
  return 0;
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
// DEFAULT-NEXT:     type @type0 V = vector<f32, 4>;
// DEFAULT-NEXT:     fn %1 @foo(%2 x: ptr<vector<f32, 4>>, %3 value: vector<f32, 4>) -> void [linkage=internal] [inline=hint] [definition=emitted] [abi=sysv64(scalar, direct) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<vector<f32, 4>>(deref(ptr_offset<ptr<vector<f32, 4>>, subtract=false, element=vector<f32, 4>, overflow=ub>(read<ptr<vector<f32, 4>>>(%2), read<i32>(%4))), read<vector<f32, 4>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 x: array<vector<f32, 4>, 32> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<vector<f32, 4>>, vector<f32, 4>) -> void, abi=sysv64(scalar, direct) -> void>(%1, array_decay<ptr<vector<f32, 4>>, length=Some(32)>(%6), read<vector<f32, 4>>(compound_literal %8 [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0)), index2 = const<f32>(0.0), index3 = neg<f32>(const<f32>(0.0)))));
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(__builtin_copysignf, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), read<f32>(lane(deref(ptr_offset<ptr<vector<f32, 4>>, subtract=false, element=vector<f32, 4>, overflow=ub>(array_decay<ptr<vector<f32, 4>>, length=Some(32)>(%6), const<i32>(3))), const<i32>(1)))), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
