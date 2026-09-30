#include <stdio.h>

typedef int   v4si __attribute__((vector_size(16)));
typedef float v4sf __attribute__((ext_vector_type(4)));
typedef float v2sf __attribute__((ext_vector_type(2)));

int main(void) {
  v4si a = {1, 2, 3, 4};
  v4si b = {5, 6, 7, 8};
  v4si c = a + b;
  c[1]   = 20;
  v4si d = __builtin_shufflevector(c, c, 3, 2, 1, 0);
  v4sf e = __builtin_convertvector(d, v4sf);
  v2sf f = e.lo;
  e.w    = f.x;
  printf("%d %d %d %g %g\n", c[0], c[1], d[0], (double)e.w, (double)f.y);
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
// DEFAULT-NEXT:     type @type[[TYPE_v4si:[0-9]+]] v4si = vector<i32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_v4sf:[0-9]+]] v4sf = vector<f32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_v2sf:[0-9]+]] v2sf = vector<f32, 2>;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 103, 32, 37, 103, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(6), index2 = const<i32>(7), index3 = const<i32>(8));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: vector<i32, 4> [storage=automatic] = add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%[[VALUE_a]]), read<vector<i32, 4>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(lane(%[[VALUE_c]], const<i32>(1)), const<i32>(20));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: vector<i32, 4> [storage=automatic] = shuffle<vector<i32, 4>, mask=[3, 2, 1, 0]>(read<vector<i32, 4>>(%[[VALUE_c]]), read<vector<i32, 4>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: vector<f32, 4> [storage=automatic] = int_to_float<vector<f32, 4>, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<vector<i32, 4>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: vector<f32, 2> [storage=automatic] = read<vector<f32, 2>>(swizzle<lanes=[0, 1]>(%[[VALUE_e]]));
// DEFAULT-NEXT:         write<f32>(lane(%[[VALUE_e]], const<i32>(3)), read<f32>(lane(%[[VALUE_f]], const<i32>(0))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str]])), read<i32>(lane(%[[VALUE_c]], const<i32>(0))), read<i32>(lane(%[[VALUE_c]], const<i32>(1))), read<i32>(lane(%[[VALUE_d]], const<i32>(0))), float_widen<f64, reason=explicit>(read<f32>(lane(%[[VALUE_e]], const<i32>(3)))), float_widen<f64, reason=explicit>(read<f32>(lane(%[[VALUE_f]], const<i32>(1)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
