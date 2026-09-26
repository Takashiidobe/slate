/* PR rtl-optimization/89634 */

static unsigned long *foo(unsigned long *x) { return x + (1 + *x); }

__attribute__((noipa)) unsigned long bar(unsigned long *x) {
  unsigned long c, d = 1, e, *f, g, h = 0, i;
  for (e = *x - 1; e > 0; e--) {
    f = foo(x + 1);
    for (i = 1; i < e; i++)
      f = foo(f);
    c = *f;
    if (c == 2)
      d *= 2;
    else {
      i = (c - 1) / 2 - 1;
      g = (2 * i + 1) * (d + 1) + (2 * d + 1);
      if (g > h)
        h = g;
      d *= c;
    }
  }
  return h;
}

int main() {
  unsigned long a[18] = {4,    2,   -200, 200, 2,   -400,
                         400,  3,   -600, 0,   600, 5,
                         -100, -66, 0,    66,  100, __LONG_MAX__ / 8 + 1};
  if (bar(a) != 17)
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: ptr<u64>) -> ptr<u64> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%1), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))), read<u64>(deref(read<ptr<u64>>(%1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(%3 x: ptr<u64>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 c: u64 [storage=automatic];
// DEFAULT-NEXT:         let %5 d: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %6 e: u64 [storage=automatic];
// DEFAULT-NEXT:         let %7 f: ptr<u64> [storage=automatic];
// DEFAULT-NEXT:         let %8 g: u64 [storage=automatic];
// DEFAULT-NEXT:         let %9 h: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %10 i: u64 [storage=automatic];
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%6, sub<u64, overflow=wrap>(read<u64>(deref(read<ptr<u64>>(%3))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:             condition: gt<u64>(read<u64>(%6), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: u64 [synthetic] = read<u64>(%6);
// DEFAULT-NEXT:                 let %17: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%6, read<u64>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<u64>>(%7, call<ptr<u64>, signature=fn(ptr<u64>) -> ptr<u64>>(%0, ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%3), const<i32>(1))));
// DEFAULT-NEXT:                     call<ptr<u64>, signature=fn(ptr<u64>) -> ptr<u64>>(%0, ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%3), const<i32>(1)));
// DEFAULT-NEXT:                     for %14
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<u64>(%10, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:                         condition: lt<u64>(read<u64>(%10), read<u64>(%6))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %18: u64 [synthetic] = read<u64>(%10);
// DEFAULT-NEXT:                             let %19: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%18), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                             write<u64>(%10, read<u64>(%19));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<ptr<u64>>(%7, call<ptr<u64>, signature=fn(ptr<u64>) -> ptr<u64>>(%0, read<ptr<u64>>(%7)));
// DEFAULT-NEXT:                             call<ptr<u64>, signature=fn(ptr<u64>) -> ptr<u64>>(%0, read<ptr<u64>>(%7));
// DEFAULT-NEXT:                     write<u64>(%4, read<u64>(deref(read<ptr<u64>>(%7))));
// DEFAULT-NEXT:                     if eq<u64>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                         let %20: u64 [synthetic] = read<u64>(%5);
// DEFAULT-NEXT:                         let %21: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%20), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:                         write<u64>(%5, read<u64>(%21));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<u64>(%10, sub<u64, overflow=wrap>(div<u64, by_zero=ub>(sub<u64, overflow=wrap>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                             write<u64>(%8, add<u64, overflow=wrap>(mul<u64, overflow=wrap>(add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), read<u64>(%10)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), add<u64, overflow=wrap>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), read<u64>(%5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:                             if gt<u64>(read<u64>(%8), read<u64>(%9))
// DEFAULT-NEXT:                                 write<u64>(%9, read<u64>(%8));
// DEFAULT-NEXT:                             let %22: u64 [synthetic] = read<u64>(%5);
// DEFAULT-NEXT:                             let %23: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%22), read<u64>(%4));
// DEFAULT-NEXT:                             write<u64>(%5, read<u64>(%23));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<u64>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 a: array<u64, 18> [storage=automatic] [align=16] = aggregate<array<u64, 18>, zero_fill=false>(index0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(4))), index1 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))), index2 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(200)))), index3 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(200))), index4 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))), index5 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(400)))), index6 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(400))), index7 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))), index8 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(600)))), index9 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), index10 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(600))), index11 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))), index12 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(100)))), index13 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(66)))), index14 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), index15 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(66))), index16 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(100))), index17 = reinterpret<u64, reason=assign, fits=unknown>(add<i64, overflow=ub>(div<i64, by_zero=ub, min_by_neg_one=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(8))), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<u64>) -> u64>(%2, array_decay<ptr<u64>, length=Some(18)>(%12)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(17))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
