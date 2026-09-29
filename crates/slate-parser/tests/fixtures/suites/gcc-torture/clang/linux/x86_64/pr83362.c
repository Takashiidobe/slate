typedef __UINT8_TYPE__  u8;
typedef __UINT32_TYPE__ u32;

u32 a, b, d, e;
u8  c;

static u32 __attribute__((noinline, noclone)) foo(u32 p) {
  do {
    e /= 0xfff;
    if (p > c)
      d = 0;
    e -= 3;
    e *= b <= a;
  } while (e >= 88030);
  return e;
}

int main(void) {
  u32 x = foo(1164);
  if (x != 0xfd)
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
// DEFAULT-NEXT:     type @type[[TYPE_u32:[0-9]+]] u32 = u32;
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: u32) -> u32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = div<u32, by_zero=ub>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4095)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_e]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 if gt<u32>(read<u32>(%[[VALUE_p]]), reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_c]])))))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_d]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%[[VALUE3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_e]], read<u32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: u32 [synthetic] = mul<u32, overflow=wrap>(read<u32>(%[[VALUE5]]), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(le<u32>(read<u32>(%[[VALUE_b]]), read<u32>(%[[VALUE_a]])))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_e]], read<u32>(%[[VALUE6]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ge<u32>(read<u32>(%[[VALUE_e]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(88030)));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_e]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1164)));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_x]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(253)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
