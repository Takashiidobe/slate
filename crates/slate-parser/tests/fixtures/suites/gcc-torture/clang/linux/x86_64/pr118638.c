/* PR rtl-optimization/118638 */

__attribute__((noipa)) int foo(int x) {
  int a  = x != -3, b, c;
  a     *= 3;
  b      = 2 * x - 9;
  a      = a + b;
  a      = ~a;
  c      = a & 1;
  return -c;
}

int main() {
  if (foo(0) != -1)
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = from_bool<i32, reason=assign>(ne<i32>(read<i32>(%[[VALUE_x]]), neg<i32, overflow=ub>(const<i32>(3))));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], sub<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_x]])), const<i32>(9)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], not<i32>(read<i32>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], and<i32>(read<i32>(%[[VALUE_a]]), const<i32>(1)));
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(read<i32>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
