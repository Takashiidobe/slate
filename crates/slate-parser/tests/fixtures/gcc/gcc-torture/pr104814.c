/* PR rtl-optimization/104814 */

short       a = 0;
static long b = 0;
int         c = 7;
char        d = 0;
short      *e = &a;
long        f = 0;

unsigned long foo(unsigned long h, long j) { return j == 0 ? h : h / j; }

int main() {
  long k = f;
  for (; c; --c) {
    for (int i = 0; i < 7; ++i)
      ;
    long m = foo(f, --b);
    d      = ((char)m | *e) <= 43165;
  }
  if (b != -7)
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
// DEFAULT-NEXT:     global %0 a: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %3 d: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %4 e: ptr<i16> [storage=static] = addr_of<ptr<i16>>(%0) [linkage=external];
// DEFAULT-NEXT:     global %5 f: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo(%7 h: u64, %8 j: i64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u64>(eq<i64>(read<i64>(%8), widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%7), div<u64, by_zero=ub>(read<u64>(%7), reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 k: i64 [storage=automatic] = read<i64>(%5);
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%16));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %14
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %11 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%11), const<i32>(7))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %17: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                             let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%11, read<i32>(%18));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                     let %12 m: i64 [storage=automatic];
// DEFAULT-NEXT:                     let %19: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:                     let %20: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%19), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(%1, read<i64>(%20));
// DEFAULT-NEXT:                     write<i64>(%12, reinterpret<i64, reason=assign, fits=unknown>(call<u64, signature=fn(u64, i64) -> u64>(%6, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%5)), read<i64>(%20))));
// DEFAULT-NEXT:                     write<i8>(%3, from_bool<i8, reason=assign>(le<i32>(or<i32>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%12))), widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%4))))), const<i32>(43165))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(7))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
