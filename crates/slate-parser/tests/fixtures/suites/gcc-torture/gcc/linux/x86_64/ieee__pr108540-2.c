/* { dg-do run } */
/* PR tree-optimization/108540 */

__attribute__((noipa)) int foo(int x, double d) {
  if (x == 42)
    d = -0.0;
  if (d == 0.0)
    return 42;
  return 12;
}

int main() {
  if (foo(42, 5.0) != 42 || foo(42, 0.0) != 42 || foo(42, -0.0) != 42 ||
      foo(10, 5.0) != 12 || foo(10, 0.0) != 42 || foo(10, -0.0) != 42)
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_d:[0-9]+]] d: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(42))
// DEFAULT-NEXT:             write<f64>(%[[VALUE_d]], neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         if eq<f64, exceptions=observable>(read<f64>(%[[VALUE_d]]), const<f64>(0.0))
// DEFAULT-NEXT:             return const<i32>(42);
// DEFAULT-NEXT:         return const<i32>(12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, f64) -> i32>(%[[VALUE_foo]], const<i32>(42), const<f64>(5.0)), const<i32>(42))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(call<i32, signature=fn(i32, f64) -> i32>(%[[VALUE_foo]], const<i32>(42), const<f64>(0.0)), const<i32>(42)));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(call<i32, signature=fn(i32, f64) -> i32>(%[[VALUE_foo]], const<i32>(42), neg<f64>(const<f64>(0.0))), const<i32>(42)));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i32>(call<i32, signature=fn(i32, f64) -> i32>(%[[VALUE_foo]], const<i32>(10), const<f64>(5.0)), const<i32>(12)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i32>(call<i32, signature=fn(i32, f64) -> i32>(%[[VALUE_foo]], const<i32>(10), const<f64>(0.0)), const<i32>(42)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(call<i32, signature=fn(i32, f64) -> i32>(%[[VALUE_foo]], const<i32>(10), neg<f64>(const<f64>(0.0))), const<i32>(42)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
