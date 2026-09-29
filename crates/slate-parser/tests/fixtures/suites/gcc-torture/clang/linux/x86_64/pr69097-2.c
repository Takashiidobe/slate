/* PR tree-optimization/69097 */

__attribute__((noinline, noclone)) int f1(int x, int y) { return x % y; }

__attribute__((noinline, noclone)) int f2(int x, int y) { return x % -y; }

__attribute__((noinline, noclone)) int f3(int x, int y) {
  int z = -y;
  return x % z;
}

int main() {
  if (f1(-__INT_MAX__ - 1, 1) != 0 || f2(-__INT_MAX__ - 1, -1) != 0 ||
      f3(-__INT_MAX__ - 1, -1) != 0)
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
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_2]]), neg<i32, overflow=ub>(read<i32>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_3:[0-9]+]] y: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic] = neg<i32, overflow=ub>(read<i32>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:         return rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_z]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f1]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f2]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f3]], sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
