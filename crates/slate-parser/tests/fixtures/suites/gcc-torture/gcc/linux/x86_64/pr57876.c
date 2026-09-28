/* PR rtl-optimization/57876 */

extern void abort(void);
int         a, b = 1, c, *d = &c, f, *g, h, j;
static int  e;

int main() {
  int i;
  for (i = 0; i < 2; i++) {
    long long k = b;
    int       l;
    for (f = 0; f < 8; f++) {
      int *m = &e;
      j      = *d;
      h      = a * j - 1;
      *m     = (h == 0) < k;
      g      = &l;
    }
  }
  if (e != 1)
    abort();
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
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%3) [linkage=external];
// DEFAULT-NEXT:     global %5 f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 g: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 e: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%11), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%11, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12 k: i64 [storage=automatic] = widen<i64, reason=assign>(read<i32>(%2));
// DEFAULT-NEXT:                     let %13 l: i32 [storage=automatic];
// DEFAULT-NEXT:                     for %16
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%5), const<i32>(8))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %19: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                             let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%5, read<i32>(%20));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %14 m: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%9);
// DEFAULT-NEXT:                                 write<i32>(%8, read<i32>(deref(read<ptr<i32>>(%4))));
// DEFAULT-NEXT:                                 write<i32>(%7, sub<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%1), read<i32>(%8)), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i32>(deref(read<ptr<i32>>(%14)), from_bool<i32, reason=assign>(lt<i64>(widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%7), const<i32>(0)))), read<i64>(%12))));
// DEFAULT-NEXT:                                 write<ptr<i32>>(%6, addr_of<ptr<i32>>(%13));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%9), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
