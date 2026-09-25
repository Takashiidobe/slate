/* On H8/300 port, NOTICE_UPDATE_CC had a bug that causes the final
   pass to remove test insns that should be kept.  */

void abort(void);
void exit(int);

unsigned short test1(unsigned short w) {
  if ((w & 0xff00) == 0) {
    if (w == 0)
      w = 2;
  }
  return w;
}

unsigned long test2(unsigned long w) {
  if ((w & 0xffff0000) == 0) {
    if (w == 0)
      w = 2;
  }
  return w;
}

int test3(unsigned short a) {
  if (a & 1)
    return 1;
  else if (a)
    return 1;
  else
    return 0;
}

int main() {
  if (test1(1) != 1)
    abort();

  if (test2(1) != 1)
    abort();

  if (test3(2) != 1)
    abort();

  exit(0);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @test1(%3 w: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%3))), const<i32>(65280)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%3))), const<i32>(0))
// DEFAULT-NEXT:                     write<u16>(%3, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u16>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @test2(%5 w: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<u64>(and<u64>(read<u64>(%5), widen<u64, reason=usual_arith>(const<u32>(4294901760))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<u64>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:                     write<u64>(%5, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u64>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test3(%7 a: u16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%7))), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<u16>(read<u16>(%7), const<u16>(0))
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%2, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1)))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%4, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%6, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(2)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
