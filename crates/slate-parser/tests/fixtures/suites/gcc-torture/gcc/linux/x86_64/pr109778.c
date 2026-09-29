/* PR tree-optimization/109778 */

int a, b, c, d, *e = &c;

static inline unsigned foo(unsigned char x) {
  x = 1 | x << 1;
  x = x >> 4 | x << 4;
  return x;
}

static inline void bar(unsigned x) { *e = 8 > foo(x + 86) - 86; }

int main() {
  d = a && b;
  bar(d + 4);
  if (c != 1)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_c]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: u8) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u8>(%[[VALUE_x]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(const<i32>(1), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x]]))), const<i32>(1))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_x]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x]]))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x]]))), const<i32>(4))))));
// DEFAULT-NEXT:         return widen<u32, reason=return>(read<u8>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: u32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_e]])), from_bool<i32, reason=assign>(gt<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8)), sub<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE_foo]], truncate<u8, reason=arg, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_x_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(86))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(86))))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(gt<u32>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(8)), sub<u32, overflow=wrap>(call<u32, signature=fn(u8) -> u32>(%[[VALUE_foo]], truncate<u8, reason=arg, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_x_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(86))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(86)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_bar]], reinterpret<u32, reason=arg, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_d]]), const<i32>(4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
