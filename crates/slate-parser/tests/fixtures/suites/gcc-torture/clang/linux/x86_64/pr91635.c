/* PR target/91635 */

#if __CHAR_BIT__ == 8 && __SIZEOF_SHORT__ == 2 && __SIZEOF_INT__ == 4 &&       \
    __SIZEOF_LONG_LONG__ == 8
unsigned short b, c;
int            u, v, w, x;

__attribute__((noipa)) int foo(unsigned short c) {
  c <<= __builtin_add_overflow(-c, -1, &b);
  c >>= 1;
  return c;
}

__attribute__((noipa)) int bar(unsigned short b) {
  b <<= -14 & 15;
  b   = b >> -~1;
  return b;
}

__attribute__((noipa)) int baz(unsigned short e) {
  e <<= 1;
  e >>= __builtin_add_overflow(8719476735, u, &v);
  return e;
}

__attribute__((noipa)) int qux(unsigned int e) {
  c  = ~1;
  c *= e;
  c  = c >> (-15 & 5);
  return c + w + x;
}
#endif

int main() {
#if __CHAR_BIT__ == 8 && __SIZEOF_SHORT__ == 2 && __SIZEOF_INT__ == 4 &&       \
    __SIZEOF_LONG_LONG__ == 8
  if (foo(0xffff) != 0x7fff)
    __builtin_abort();
  if (bar(5) != 5)
    __builtin_abort();
  if (baz(~0) != 0x7fff)
    __builtin_abort();
  if (qux(2) != 0x7ffe)
    __builtin_abort();
#endif
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
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_c_2:[0-9]+]] c: u16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_c_2]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE0]]))), from_bool<i32, reason=promotion>(overflow_add<bool>(neg<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c_2]])))), neg<i32, overflow=ub>(const<i32>(1)), deref(addr_of<ptr<u16>>(%[[VALUE_b]])))))));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_c_2]], read<u16>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_c_2]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE2]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_c_2]], read<u16>(%[[VALUE3]]));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u16>(%[[VALUE_c_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_b_2:[0-9]+]] b: u16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_b_2]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE4]]))), and<i32>(neg<i32, overflow=ub>(const<i32>(14)), const<i32>(15)))));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_b_2]], read<u16>(%[[VALUE5]]));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_b_2]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_b_2]]))), neg<i32, overflow=ub>(not<i32>(const<i32>(1)))))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u16>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_e:[0-9]+]] e: u16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_e]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE6]]))), const<i32>(1))));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_e]], read<u16>(%[[VALUE7]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_e]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE8]]))), from_bool<i32, reason=promotion>(overflow_add<bool>(const<i64>(8719476735), read<i32>(%[[VALUE_u]]), deref(addr_of<ptr<i32>>(%[[VALUE_v]])))))));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_e]], read<u16>(%[[VALUE9]]));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u16>(%[[VALUE_e]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_e_2:[0-9]+]] e: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u16>(%[[VALUE_c]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE10]])))), read<u32>(%[[VALUE_e_2]])));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_c]], read<u16>(%[[VALUE11]]));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_c]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c]]))), and<i32>(neg<i32, overflow=ub>(const<i32>(15)), const<i32>(5))))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_c]]))), read<i32>(%[[VALUE_w]])), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%[[VALUE_foo]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))), const<i32>(32767))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%[[VALUE_bar]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(5)))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%[[VALUE_baz]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(not<i32>(const<i32>(0))))), const<i32>(32767))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE_qux]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2))), const<i32>(32766))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
