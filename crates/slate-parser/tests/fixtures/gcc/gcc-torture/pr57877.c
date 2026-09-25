/* PR rtl-optimization/57877 */

extern void abort(void);
int         a, b, *c = &b, e, f = 6, g, h;
short       d;

static unsigned char foo(unsigned long long p1, int *p2) {
  for (; g <= 0; g++) {
    short *i = &d;
    int   *j = &e;
    h        = *c;
    *i       = h;
    *j       = (*i == *p2) < p1;
  }
  return 0;
}

int main() {
  foo(f, &a);
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
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%2) [linkage=external];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %6 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @foo(%10 p1: u64, %11 p2: ptr<i32>) -> u8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12 i: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%8);
// DEFAULT-NEXT:                     let %13 j: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%4);
// DEFAULT-NEXT:                     write<i32>(%7, read<i32>(deref(read<ptr<i32>>(%3))));
// DEFAULT-NEXT:                     write<i16>(deref(read<ptr<i16>>(%12)), truncate<i16, reason=assign, fits=unknown>(read<i32>(%7)));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%13)), from_bool<i32, reason=assign>(lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%12)))), read<i32>(deref(read<ptr<i32>>(%11))))))), read<u64>(%10))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<u8, signature=fn(u64, ptr<i32>) -> u8>(%9, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%5))), addr_of<ptr<i32>>(%1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
