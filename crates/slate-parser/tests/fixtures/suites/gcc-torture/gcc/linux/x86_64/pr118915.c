/* PR tree-optimization/118915 */

int a;

int foo(int c, int d, int e, int f) {
  if (!d || !e)
    return -22;
  if (c > 16)
    return -22;
  if (!f)
    return -22;
  return 2;
}

int main() {
  if (foo(a + 21, a + 6, a + 34, a + 26) != -22)
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_d:[0-9]+]] d: i32, %[[VALUE_e:[0-9]+]] e: i32, %[[VALUE_f:[0-9]+]] f: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))), not<bool>(ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0))))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(22));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_c]]), const<i32>(16))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(22));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_f]]), const<i32>(0)))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(22));
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, i32, i32) -> i32>(%[[VALUE_foo]], add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), const<i32>(21)), add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), const<i32>(6)), add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), const<i32>(34)), add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), const<i32>(26))), neg<i32, overflow=ub>(const<i32>(22)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
