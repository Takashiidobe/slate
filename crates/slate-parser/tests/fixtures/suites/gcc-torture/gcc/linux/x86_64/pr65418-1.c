/* PR tree-optimization/65418 */

__attribute__((noinline, noclone)) int foo(int x) {
  if (x == -216 || x == -132 || x == -218 || x == -146)
    return 1;
  return 0;
}

int main() {
  volatile int i;
  for (i = -230; i < -120; i++)
    if (foo(i) != (i == -216 || i == -132 || i == -218 || i == -146))
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(eq<i32>(read<i32>(%[[VALUE_x]]), neg<i32, overflow=ub>(const<i32>(216))), eq<i32>(read<i32>(%[[VALUE_x]]), neg<i32, overflow=ub>(const<i32>(132)))), eq<i32>(read<i32>(%[[VALUE_x]]), neg<i32, overflow=ub>(const<i32>(218)))), eq<i32>(read<i32>(%[[VALUE_x]]), neg<i32, overflow=ub>(const<i32>(146))))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: volatile i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_i]], neg<i32, overflow=ub>(const<i32>(230)));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32, volatile>(%[[VALUE_i]]), neg<i32, overflow=ub>(const<i32>(120)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32, volatile>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32, volatile>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], read<i32, volatile>(%[[VALUE_i]])), from_bool<i32, reason=promotion>(logical_or<bool>(logical_or<bool>(logical_or<bool>(eq<i32>(read<i32, volatile>(%[[VALUE_i]]), neg<i32, overflow=ub>(const<i32>(216))), eq<i32>(read<i32, volatile>(%[[VALUE_i]]), neg<i32, overflow=ub>(const<i32>(132)))), eq<i32>(read<i32, volatile>(%[[VALUE_i]]), neg<i32, overflow=ub>(const<i32>(218)))), eq<i32>(read<i32, volatile>(%[[VALUE_i]]), neg<i32, overflow=ub>(const<i32>(146))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
