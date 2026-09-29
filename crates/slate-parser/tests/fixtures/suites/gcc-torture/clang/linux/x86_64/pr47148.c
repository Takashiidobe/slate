/* PR tree-optimization/47148 */

static inline unsigned bar(unsigned x, unsigned y) {
  if (y >= 32)
    return x;
  else
    return x >> y;
}

static unsigned a = 1, b = 1;

static inline void foo(unsigned char x, unsigned y) {
  if (!y)
    return;
  unsigned c  = (0x7000U / (x - 2)) ^ a;
  unsigned d  = bar(a, a);
  b          &= ((a - d) && (a - 1)) + c;
}

int main(void) {
  foo(1, 1);
  foo(-1, 1);
  if (b && ((unsigned char)-1) == 255)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: u32, %[[VALUE_y:[0-9]+]] y: u32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ge<u32>(read<u32>(%[[VALUE_y]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(32)))
// DEFAULT-NEXT:             return read<u32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_x]]), read<u32>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: u8, %[[VALUE_y_2:[0-9]+]] y: u32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<u32>(read<u32>(%[[VALUE_y_2]]), const<u32>(0)))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: u32 [storage=automatic] = xor<u32>(div<u32, by_zero=ub>(const<u32>(28672), reinterpret<u32, reason=usual_arith, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x_2]]))), const<i32>(2)))), read<u32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: u32 [storage=automatic] = call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE_bar]], read<u32>(%[[VALUE_a]]), read<u32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = and<u32>(read<u32>(%[[VALUE0]]), add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<u32>(sub<u32, overflow=wrap>(read<u32>(%[[VALUE_a]]), read<u32>(%[[VALUE_d]])), const<u32>(0)), ne<u32>(sub<u32, overflow=wrap>(read<u32>(%[[VALUE_a]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), const<u32>(0))))), read<u32>(%[[VALUE_c]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_b]], read<u32>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u8, u32) -> void>(%[[VALUE_foo]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u8, u32) -> void>(%[[VALUE_foo]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if logical_and<bool>(ne<u32>(read<u32>(%[[VALUE_b]]), const<u32>(0)), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))), const<i32>(255)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
