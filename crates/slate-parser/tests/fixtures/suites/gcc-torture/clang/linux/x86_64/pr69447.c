typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

u64 __attribute__((noinline, noclone)) foo(u8 u8_0, u16 u16_0, u64 u64_0,
                                           u8 u8_1, u16 u16_1, u64 u64_1,
                                           u64 u64_2, u8 u8_3, u64 u64_3) {
  u64_1 *= 0x7730;
  u64_3 *= u64_3;
  u16_1 |= u64_3;
  u64_3 -= 2;
  u8_3  /= u64_2;
  u8_0  |= 3;
  u64_3 %= u8_0;
  u8_0  -= 1;
  return u8_0 + u16_0 + u64_0 + u8_1 + u16_1 + u64_1 + u8_3 + u64_3;
}

int main() {
  unsigned x = foo(1, 1, 1, 1, 1, 1, 1, 1, 1);
  if (x != 0x7737)
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
// DEFAULT-NEXT:     type @type0 u8 = u8;
// DEFAULT-NEXT:     type @type1 u16 = u16;
// DEFAULT-NEXT:     type @type2 u32 = u32;
// DEFAULT-NEXT:     type @type3 u64 = u64;
// DEFAULT-NEXT:     fn %4 @foo(%5 u8_0: u8, %6 u16_0: u16, %7 u64_0: u64, %8 u8_1: u8, %9 u16_1: u16, %10 u64_1: u64, %11 u64_2: u64, %12 u8_3: u8, %13 u64_3: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17: u64 [synthetic] = read<u64>(%10);
// DEFAULT-NEXT:         let %18: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%17), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(30512))));
// DEFAULT-NEXT:         write<u64>(%10, read<u64>(%18));
// DEFAULT-NEXT:         let %19: u64 [synthetic] = read<u64>(%13);
// DEFAULT-NEXT:         let %20: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%19), read<u64>(%13));
// DEFAULT-NEXT:         write<u64>(%13, read<u64>(%20));
// DEFAULT-NEXT:         let %21: u16 [synthetic] = read<u16>(%9);
// DEFAULT-NEXT:         let %22: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(or<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%21))))), read<u64>(%13)));
// DEFAULT-NEXT:         write<u16>(%9, read<u16>(%22));
// DEFAULT-NEXT:         let %23: u64 [synthetic] = read<u64>(%13);
// DEFAULT-NEXT:         let %24: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%23), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         write<u64>(%13, read<u64>(%24));
// DEFAULT-NEXT:         let %25: u8 [synthetic] = read<u8>(%12);
// DEFAULT-NEXT:         let %26: u8 [synthetic] = truncate<u8, reason=assign, fits=unknown>(div<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%25))))), read<u64>(%11)));
// DEFAULT-NEXT:         write<u8>(%12, read<u8>(%26));
// DEFAULT-NEXT:         let %27: u8 [synthetic] = read<u8>(%5);
// DEFAULT-NEXT:         let %28: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%27))), const<i32>(3))));
// DEFAULT-NEXT:         write<u8>(%5, read<u8>(%28));
// DEFAULT-NEXT:         let %29: u64 [synthetic] = read<u64>(%13);
// DEFAULT-NEXT:         let %30: u64 [synthetic] = rem<u64, by_zero=ub>(read<u64>(%29), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%5))))));
// DEFAULT-NEXT:         write<u64>(%13, read<u64>(%30));
// DEFAULT-NEXT:         let %31: u8 [synthetic] = read<u8>(%5);
// DEFAULT-NEXT:         let %32: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%31))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%5, read<u8>(%32));
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%5))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%6)))))), read<u64>(%7)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%8)))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%9)))))), read<u64>(%10)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12)))))), read<u64>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 x: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u8, u16, u64, u8, u16, u64, u64, u8, u64) -> u64>(%4, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%15), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(30519)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
