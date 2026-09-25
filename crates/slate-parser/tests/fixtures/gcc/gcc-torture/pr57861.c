/* PR rtl-optimization/57861 */

extern void  abort(void);
short        a = 1, f;
int          b, c, d, *g = &b, h, i, j;
unsigned int e;

static int foo(char p) {
  int k;
  for (c = 0; c < 2; c++) {
    i = (j = 0) || p;
    k = i * p;
    if (e < k) {
      short *l = &f;
      a        = d && h;
      *l       = 0;
    }
  }
  return 0;
}

int main() {
  *g = foo(a);
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
// DEFAULT-NEXT:     global %1 a: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %2 f: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 g: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%3) [linkage=external];
// DEFAULT-NEXT:     global %7 h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 e: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @foo(%12 p: i8) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 k: i32 [storage=automatic];
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%18));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(%8, from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i8>(read<i8>(%12), const<i8>(0)))));
// DEFAULT-NEXT:                     write<i32>(%13, mul<i32, overflow=ub>(read<i32>(%8), widen<i32, reason=promotion>(read<i8>(%12))));
// DEFAULT-NEXT:                     if lt<u32>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%13)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %14 l: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%2);
// DEFAULT-NEXT:                             write<i16>(%1, from_bool<i16, reason=assign>(logical_and<bool>(ne<i32>(read<i32>(%5), const<i32>(0)), ne<i32>(read<i32>(%7), const<i32>(0)))));
// DEFAULT-NEXT:                             write<i16>(deref(read<ptr<i16>>(%14)), truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%6)), call<i32, signature=fn(i8) -> i32>(%11, truncate<i8, reason=arg, fits=unknown>(read<i16>(%1))));
// DEFAULT-NEXT:         call<i32, signature=fn(i8) -> i32>(%11, truncate<i8, reason=arg, fits=unknown>(read<i16>(%1)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
