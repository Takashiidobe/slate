/* PR tree-optimization/65215 */

static inline unsigned int foo(unsigned int x) {
  return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24);
}

__attribute__((noinline, noclone)) unsigned long long
bar(unsigned long long *x) {
  return ((unsigned long long)foo(*x) << 32) | foo(*x >> 32);
}

int main() {
  if (__CHAR_BIT__ != 8 || sizeof(unsigned int) != 4 ||
      sizeof(unsigned long long) != 8)
    return 0;
  unsigned long long l =
      foo(0xfeedbea8U) | ((unsigned long long)foo(0xdeadbeefU) << 32);
  if (bar(&l) != 0xfeedbea8deadbeefULL)
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: u32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<u32>(or<u32>(or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_x]]), const<i32>(24)), and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_x]]), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65280)))), and<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_x]]), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16711680)))), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_x]]), const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: ptr<u64>) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_foo]], truncate<u32, reason=arg, fits=unknown>(read<u64>(deref(read<ptr<u64>>(%[[VALUE_x_2]])))))), const<i32>(32)), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_foo]], truncate<u32, reason=arg, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(deref(read<ptr<u64>>(%[[VALUE_x_2]]))), const<i32>(32))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(const<i32>(8), const<i32>(8)), ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: u64 [storage=automatic] = or<u64>(widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_foo]], const<u32>(4276993704))), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(call<u32, signature=fn(u32) -> u32>(%[[VALUE_foo]], const<u32>(3735928559))), const<i32>(32)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<u64>) -> u64>(%[[VALUE_bar]], addr_of<ptr<u64>>(%[[VALUE_l]])), const<u64>(18369548087613832943))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
