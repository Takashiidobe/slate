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
// DEFAULT-NEXT:     type @type[[TYPE_u8:[0-9]+]] u8 = u8;
// DEFAULT-NEXT:     type @type[[TYPE_u16:[0-9]+]] u16 = u16;
// DEFAULT-NEXT:     type @type[[TYPE_u32:[0-9]+]] u32 = u32;
// DEFAULT-NEXT:     type @type[[TYPE_u64:[0-9]+]] u64 = u64;
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_u8_0:[0-9]+]] u8_0: u8, %[[VALUE_u16_0:[0-9]+]] u16_0: u16, %[[VALUE_u64_0:[0-9]+]] u64_0: u64, %[[VALUE_u8_1:[0-9]+]] u8_1: u8, %[[VALUE_u16_1:[0-9]+]] u16_1: u16, %[[VALUE_u64_1:[0-9]+]] u64_1: u64, %[[VALUE_u64_2:[0-9]+]] u64_2: u64, %[[VALUE_u8_3:[0-9]+]] u8_3: u8, %[[VALUE_u64_3:[0-9]+]] u64_3: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_u64_1]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(30512))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_u64_1]], read<u64>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_u64_3]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%[[VALUE2]]), read<u64>(%[[VALUE_u64_3]]));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_u64_3]], read<u64>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_u16_1]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(or<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE4]]))))), read<u64>(%[[VALUE_u64_3]])));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_u16_1]], read<u16>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_u64_3]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%[[VALUE6]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_u64_3]], read<u64>(%[[VALUE7]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_u8_3]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: u8 [synthetic] = truncate<u8, reason=assign, fits=unknown>(div<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE8]]))))), read<u64>(%[[VALUE_u64_2]])));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_u8_3]], read<u8>(%[[VALUE9]]));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_u8_0]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE10]]))), const<i32>(3))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_u8_0]], read<u8>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_u64_3]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: u64 [synthetic] = rem<u64, by_zero=ub>(read<u64>(%[[VALUE12]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_u8_0]]))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_u64_3]], read<u64>(%[[VALUE13]]));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_u8_0]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE14]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_u8_0]], read<u8>(%[[VALUE15]]));
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(%[[VALUE_u8_0]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(%[[VALUE_u16_0]])))))),
// DEFAULT-SAME: read<u64>(%[[VALUE_u64_0]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion,
// DEFAULT-SAME: fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_u8_1]])))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64,
// DEFAULT-SAME: reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_u16_1]])))))),
// DEFAULT-SAME: read<u64>(%[[VALUE_u64_1]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion,
// DEFAULT-SAME: fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_u8_3]])))))),
// DEFAULT-SAME: read<u64>(%[[VALUE_u64_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(call<u64, signature=fn(u8, u16, u64, u8, u16, u64, u64, u8, u64) -> u64>(%[[VALUE_foo]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_x]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(30519)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
