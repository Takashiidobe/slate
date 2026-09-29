/* PR rtl-optimization/57281 */

int                a = 1, b, d, *e = &d;
long long          c, *g = &c;
volatile long long f;

int foo(int h) {
  int j = *g = b;
  return h == 0 ? j : 0;
}

int main() {
  int h = a;
  for (; b != -20; b--) {
    (int)f;
    *e = 0;
    *e = foo(h);
  }
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_d]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: ptr<i64> [storage=static] = addr_of<ptr<i64>>(%[[VALUE_c]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: volatile i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_h:[0-9]+]] h: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE_g]])), widen<i64, reason=assign>(read<i32>(%[[VALUE_b]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], truncate<i32, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         return conditional<i32>(eq<i32>(read<i32>(%[[VALUE_h]]), const<i32>(0)), read<i32>(%[[VALUE_j]]), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_h_2:[0-9]+]] h: i32 [storage=automatic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_b]]), neg<i32, overflow=ub>(const<i32>(20)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     truncate<i32, reason=explicit, fits=unknown>(read<i64, volatile>(%[[VALUE_f]]));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE_e]])), const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE_e]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], read<i32>(%[[VALUE_h_2]])));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], read<i32>(%[[VALUE_h_2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
