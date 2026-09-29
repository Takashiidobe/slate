/* PR tree-optimization/78675 */

long int a;

__attribute__((noinline, noclone)) long int foo(long int x) {
  long int b;
  while (a < 1) {
    b = a && x;
    ++a;
  }
  return b;
}

int main() {
  if (foo(0) != 0)
    __builtin_abort();
  a = 0;
  if (foo(1) != 0)
    __builtin_abort();
  a = 0;
  if (foo(25) != 0)
    __builtin_abort();
  a = -64;
  if (foo(0) != 0)
    __builtin_abort();
  a = -64;
  if (foo(1) != 0)
    __builtin_abort();
  a = -64;
  if (foo(25) != 0)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i64) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i64 [storage=automatic];
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] lt<i64>(read<i64>(%[[VALUE_a]]), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_b]], from_bool<i64, reason=assign>(logical_and<bool>(ne<i64>(read<i64>(%[[VALUE_a]]), const<i64>(0)), ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(0)))));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_a]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE1]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_a]], read<i64>(%[[VALUE2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_b]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(0))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64>(%[[VALUE_a]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64>(%[[VALUE_a]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(25))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64>(%[[VALUE_a]], widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(64))));
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(0))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64>(%[[VALUE_a]], widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(64))));
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i64>(%[[VALUE_a]], widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(64))));
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE_foo]], widen<i64, reason=arg>(const<i32>(25))), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
