/* PR tree-optimization/81588 */

__attribute__((noinline, noclone)) int bar(int x) {
  __asm volatile("" : : "g"(x) : "memory");
}

__attribute__((noinline, noclone)) int foo(unsigned x, long long y) {
  if (y < 0)
    return 0;
  if (y < (long long)(4 * x)) {
    bar(y);
    return 1;
  }
  return 0;
}

int main() {
  volatile unsigned  x = 10;
  volatile long long y = -10000;
  if (foo(x, y) != 0)
    __builtin_abort();
  y = -1;
  if (foo(x, y) != 0)
    __builtin_abort();
  y = 0;
  if (foo(x, y) != 1)
    __builtin_abort();
  y = 39;
  if (foo(x, y) != 1)
    __builtin_abort();
  y = 40;
  if (foo(x, y) != 0)
    __builtin_abort();
  y = 10000;
  if (foo(x, y) != 0)
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
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             in 0 "g" [reg | mem | imm | sym] -> reg width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: u32, %[[VALUE_y:[0-9]+]] y: i64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_y]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_y]]), reinterpret<i64, reason=explicit, fits=unknown>(widen<u64, reason=explicit>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(4)), read<u32>(%[[VALUE_x_2]])))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(i32) -> i32>(%[[VALUE_bar]], truncate<i32, reason=arg, fits=unknown>(read<i64>(%[[VALUE_y]])));
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_3:[0-9]+]] x: volatile u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(10));
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: volatile i64 [storage=automatic] = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(10000)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i64) -> i32>(%[[VALUE_foo]], read<u32, volatile>(%[[VALUE_x_3]]), read<i64, volatile>(%[[VALUE_y_2]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64, volatile>(%[[VALUE_y_2]], widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i64) -> i32>(%[[VALUE_foo]], read<u32, volatile>(%[[VALUE_x_3]]), read<i64, volatile>(%[[VALUE_y_2]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64, volatile>(%[[VALUE_y_2]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i64) -> i32>(%[[VALUE_foo]], read<u32, volatile>(%[[VALUE_x_3]]), read<i64, volatile>(%[[VALUE_y_2]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64, volatile>(%[[VALUE_y_2]], widen<i64, reason=assign>(const<i32>(39)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i64) -> i32>(%[[VALUE_foo]], read<u32, volatile>(%[[VALUE_x_3]]), read<i64, volatile>(%[[VALUE_y_2]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64, volatile>(%[[VALUE_y_2]], widen<i64, reason=assign>(const<i32>(40)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i64) -> i32>(%[[VALUE_foo]], read<u32, volatile>(%[[VALUE_x_3]]), read<i64, volatile>(%[[VALUE_y_2]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64, volatile>(%[[VALUE_y_2]], widen<i64, reason=assign>(const<i32>(10000)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i64) -> i32>(%[[VALUE_foo]], read<u32, volatile>(%[[VALUE_x_3]]), read<i64, volatile>(%[[VALUE_y_2]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
