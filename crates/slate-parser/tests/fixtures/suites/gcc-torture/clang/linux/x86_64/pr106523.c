/* PR tree-optimization/106523 */

__attribute__((noipa)) unsigned char f7(unsigned char x, unsigned int y) {
  unsigned int t = x;
  return (t << y) | (t >> ((-y) & 7));
}

int main() {
  if (__CHAR_BIT__ != 8 || __SIZEOF_INT__ != 4)
    return 0;

  volatile unsigned char x = 152;
  volatile unsigned int  y = 19;
  if (f7(x, y) != 4)
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
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_x:[0-9]+]] x: u8, %[[VALUE_y:[0-9]+]] y: u32) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: u32 [storage=automatic] = widen<u32, reason=assign>(read<u8>(%[[VALUE_x]]));
// DEFAULT-NEXT:         return truncate<u8, reason=return, fits=unknown>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_t]]), read<u32>(%[[VALUE_y]])), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_t]]), and<u32>(neg<u32, overflow=wrap>(read<u32>(%[[VALUE_y]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(7))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(const<i32>(8), const<i32>(8)), ne<i32>(const<i32>(4), const<i32>(4)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: volatile u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(152)));
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: volatile u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(19));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u32) -> u8>(%[[VALUE_f7]], read<u8, volatile>(%[[VALUE_x_2]]), read<u32, volatile>(%[[VALUE_y_2]])))), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
