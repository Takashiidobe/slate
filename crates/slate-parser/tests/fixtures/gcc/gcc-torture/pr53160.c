/* PR rtl-optimization/53160 */

extern void abort(void);

int           a, c = 1, d, e, g;
volatile int  b;
volatile char f;
long          h;
short         i;

void foo(void) {
  for (e = 0; e; ++e)
    ;
}

int main() {
  if (g)
    (void)b;
  foo();
  for (d = 0; d >= 0; d--) {
    short j = f;
    int   k = 0;
    i       = j ? j : j << k;
  }
  h = c == 0 ? 0 : i;
  a = h;
  if (a != 0)
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
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 b: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 f: volatile i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 h: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 i: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             read<i32, volatile>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: ge<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12 j: i16 [storage=automatic] = widen<i16, reason=assign>(read<i8, volatile>(%7));
// DEFAULT-NEXT:                     let %13 k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     write<i16>(%9, truncate<i16, reason=assign, fits=unknown>(conditional<i32>(ne<i16>(read<i16>(%12), const<i16>(0)), widen<i32, reason=promotion>(read<i16>(%12)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%12)), read<i32>(%13)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i64>(%8, widen<i64, reason=assign>(conditional<i32>(eq<i32>(read<i32>(%2), const<i32>(0)), const<i32>(0), widen<i32, reason=promotion>(read<i16>(%9)))));
// DEFAULT-NEXT:         write<i32>(%1, truncate<i32, reason=assign, fits=unknown>(read<i64>(%8)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
