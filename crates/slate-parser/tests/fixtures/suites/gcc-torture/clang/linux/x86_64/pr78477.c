/* PR rtl-optimization/78477 */

unsigned       a;
unsigned short b;

unsigned foo(unsigned x) {
  b   = x;
  a >>= (b & 1);
  b   = 1 | (b << 5);
  b >>= 15;
  x   = (unsigned char)b > ((2 - (unsigned char)b) & 1);
  b   = 0;
  return x;
}

int main() {
  if (__CHAR_BIT__ != 8 || sizeof(short) != 2 || sizeof(int) < 4)
    return 0;
  unsigned x = foo(12345);
  if (x != 0)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u16>(%[[VALUE_b]], truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_x]])));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE0]]), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_b]]))), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a]], read<u32>(%[[VALUE1]]));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_b]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(const<i32>(1), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_b]]))), const<i32>(5))))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE2]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_b]], read<u16>(%[[VALUE3]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_x]], from_bool<u32, reason=assign>(gt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u16>(%[[VALUE_b]])))), and<i32>(sub<i32, overflow=ub>(const<i32>(2), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u16>(%[[VALUE_b]]))))), const<i32>(1)))));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_b]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(const<i32>(8), const<i32>(8)), ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))), lt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: u32 [storage=automatic] = call<u32, signature=fn(u32) -> u32>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(12345)));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_x_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
