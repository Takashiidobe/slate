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
// DEFAULT-NEXT:     global %0 b: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 c: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 u: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 w: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo(%7 c: u16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16: u16 [synthetic] = read<u16>(%7);
// DEFAULT-NEXT:         let %17: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%16))), from_bool<i32, reason=promotion>(overflow_add<bool>(neg<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%7)))), neg<i32, overflow=ub>(const<i32>(1)), deref(addr_of<ptr<u16>>(%0)))))));
// DEFAULT-NEXT:         write<u16>(%7, read<u16>(%17));
// DEFAULT-NEXT:         let %18: u16 [synthetic] = read<u16>(%7);
// DEFAULT-NEXT:         let %19: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%18))), const<i32>(1))));
// DEFAULT-NEXT:         write<u16>(%7, read<u16>(%19));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u16>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @bar(%9 b: u16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20: u16 [synthetic] = read<u16>(%9);
// DEFAULT-NEXT:         let %21: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%20))), and<i32>(neg<i32, overflow=ub>(const<i32>(14)), const<i32>(15)))));
// DEFAULT-NEXT:         write<u16>(%9, read<u16>(%21));
// DEFAULT-NEXT:         write<u16>(%9, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%9))), neg<i32, overflow=ub>(not<i32>(const<i32>(1)))))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u16>(%9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @baz(%11 e: u16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %22: u16 [synthetic] = read<u16>(%11);
// DEFAULT-NEXT:         let %23: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%22))), const<i32>(1))));
// DEFAULT-NEXT:         write<u16>(%11, read<u16>(%23));
// DEFAULT-NEXT:         let %24: u16 [synthetic] = read<u16>(%11);
// DEFAULT-NEXT:         let %25: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%24))), from_bool<i32, reason=promotion>(overflow_add<bool>(const<i64>(8719476735), read<i32>(%2), deref(addr_of<ptr<i32>>(%3)))))));
// DEFAULT-NEXT:         write<u16>(%11, read<u16>(%25));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u16>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @qux(%13 e: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u16>(%1, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(1)))));
// DEFAULT-NEXT:         let %26: u16 [synthetic] = read<u16>(%1);
// DEFAULT-NEXT:         let %27: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%26)))), read<u32>(%13)));
// DEFAULT-NEXT:         write<u16>(%1, read<u16>(%27));
// DEFAULT-NEXT:         write<u16>(%1, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1))), and<i32>(neg<i32, overflow=ub>(const<i32>(15)), const<i32>(5))))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1))), read<i32>(%4)), read<i32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%6, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(const<i32>(65535)))), const<i32>(32767))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%8, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(5)))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u16) -> i32>(%10, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=unknown>(not<i32>(const<i32>(0))))), const<i32>(32767))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32) -> i32>(%12, reinterpret<u32, reason=arg, fits=always>(const<i32>(2))), const<i32>(32766))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
