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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 e: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%2) [linkage=external];
// DEFAULT-NEXT:     global %4 c: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 g: ptr<i64> [storage=static] = addr_of<ptr<i64>>(%4) [linkage=external];
// DEFAULT-NEXT:     global %6 f: volatile i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo(%8 h: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 j: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%5)), widen<i64, reason=assign>(read<i32>(%1)));
// DEFAULT-NEXT:         write<i32>(%9, truncate<i32, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%1))));
// DEFAULT-NEXT:         return conditional<i32>(eq<i32>(read<i32>(%8), const<i32>(0)), read<i32>(%9), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 h: i32 [storage=automatic] = read<i32>(%0);
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%1), neg<i32, overflow=ub>(const<i32>(20)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     truncate<i32, reason=explicit, fits=unknown>(read<i64, volatile>(%6));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%3)), const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%3)), call<i32, signature=fn(i32) -> i32>(%7, read<i32>(%11)));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%7, read<i32>(%11));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
