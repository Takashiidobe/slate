/* { dg-options "-fno-builtin-abort" } */

int           a, b, m, n, o, p, s, u, i;
char          c, q, y;
short         d;
unsigned char e;
static int    f, h;
static short  g, r, v;
unsigned      t;

extern void abort();

int fn1(int p1) { return a ? p1 : p1 + a; }

unsigned char fn2(unsigned char p1, int p2) { return p2 >= 2 ? p1 : p1 >> p2; }

static short fn3() {
  int w, x = 0;
  for (; p < 31; p++) {
    s = fn1(c | ((1 && c) == c));
    t = fn2(s, x);
    c = (unsigned)c > -(unsigned)((o = (m = d = t) == p) <= 4UL) && n;
    v = -c;
    y = 1;
    for (; y; y++)
      e = v == 1;
    d = 0;
    for (; h != 2;) {
      for (;;) {
        if (!m)
          abort();
        r = 7 - f;
        x = e = i | r;
        q     = u * g;
        w     = b == q;
        if (w)
          break;
      }
      break;
    }
  }
  return x;
}

int main() {
  fn3();
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 m: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 n: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 o: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 p: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 s: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 u: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 q: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 y: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 e: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 f: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %15 h: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %16 g: i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %17 r: i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %18 v: i16 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %19 t: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %20 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %21 @fn1(%22 p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%0), const<i32>(0)), read<i32>(%22), add<i32, overflow=ub>(read<i32>(%22), read<i32>(%0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @fn2(%24 p1: u8, %25 p2: i32) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(conditional<i32>(ge<i32>(read<i32>(%25), const<i32>(2)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%24))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%24))), read<i32>(%25)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @fn3() -> i16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 w: i32 [storage=automatic];
// DEFAULT-NEXT:         let %28 x: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%35));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%6, call<i32, signature=fn(i32) -> i32>(%21, or<i32>(widen<i32, reason=promotion>(read<i8>(%9)), from_bool<i32, reason=promotion>(eq<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i8>(read<i8>(%9), const<i8>(0)))), widen<i32, reason=promotion>(read<i8>(%9)))))));
// DEFAULT-NEXT:                     call<i32, signature=fn(i32) -> i32>(%21, or<i32>(widen<i32, reason=promotion>(read<i8>(%9)), from_bool<i32, reason=promotion>(eq<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i8>(read<i8>(%9), const<i8>(0)))), widen<i32, reason=promotion>(read<i8>(%9))))));
// DEFAULT-NEXT:                     write<u32>(%19, widen<u32, reason=assign>(call<u8, signature=fn(u8, i32) -> u8>(%23, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%6))), read<i32>(%28))));
// DEFAULT-NEXT:                     widen<u32, reason=assign>(call<u8, signature=fn(u8, i32) -> u8>(%23, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(read<i32>(%6))), read<i32>(%28)));
// DEFAULT-NEXT:                     write<i16>(%12, reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u32>(%19))));
// DEFAULT-NEXT:                     write<i32>(%2, widen<i32, reason=assign>(reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u32>(%19)))));
// DEFAULT-NEXT:                     write<i32>(%4, from_bool<i32, reason=assign>(eq<i32>(widen<i32, reason=assign>(reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u32>(%19)))), read<i32>(%5))));
// DEFAULT-NEXT:                     write<i8>(%9, from_bool<i8, reason=assign>(logical_and<bool>(gt<u32>(reinterpret<u32, reason=explicit, fits=unknown>(widen<i32, reason=explicit>(read<i8>(%9))), neg<u32, overflow=wrap>(from_bool<u32, reason=explicit>(le<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(from_bool<i32, reason=assign>(eq<i32>(widen<i32, reason=assign>(reinterpret<i16, reason=assign, fits=unknown>(truncate<u16, reason=assign, fits=unknown>(read<u32>(%19)))), read<i32>(%5))))), const<u64>(4))))), ne<i32>(read<i32>(%3), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i16>(%18, truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%9)))));
// DEFAULT-NEXT:                     write<i8>(%11, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                     for %31
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: ne<i8>(read<i8>(%11), const<i8>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %36: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:                             let %37: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%36)), const<i32>(1)));
// DEFAULT-NEXT:                             write<i8>(%11, read<i8>(%37));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<u8>(%13, from_bool<u8, reason=assign>(eq<i32>(widen<i32, reason=promotion>(read<i16>(%18)), const<i32>(1))));
// DEFAULT-NEXT:                     write<i16>(%12, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     for %32
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: ne<i32>(read<i32>(%15), const<i32>(2))
// DEFAULT-NEXT:                         increment: omitted
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 for %33
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                     condition: omitted
// DEFAULT-NEXT:                                     increment: omitted
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             if not<bool>(ne<i32>(read<i32>(%2), const<i32>(0)))
// DEFAULT-NEXT:                                                 call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:                                             write<i16>(%17, truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(const<i32>(7), read<i32>(%14))));
// DEFAULT-NEXT:                                             write<u8>(%13, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(read<i32>(%8), widen<i32, reason=promotion>(read<i16>(%17))))));
// DEFAULT-NEXT:                                             write<i32>(%28, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(read<i32>(%8), widen<i32, reason=promotion>(read<i16>(%17))))))));
// DEFAULT-NEXT:                                             write<i8>(%10, truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%7), widen<i32, reason=promotion>(read<i16>(%16)))));
// DEFAULT-NEXT:                                             write<i32>(%27, from_bool<i32, reason=assign>(eq<i32>(read<i32>(%1), widen<i32, reason=promotion>(read<i8>(%10)))));
// DEFAULT-NEXT:                                             if ne<i32>(read<i32>(%27), const<i32>(0))
// DEFAULT-NEXT:                                                 break %33;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                 break %32;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(read<i32>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i16, signature=fn() -> i16>(%26);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
