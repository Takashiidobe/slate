/* { dg-do run } */
/* PR tree-optimization/84235 */

int main() {
  double d = 1.0 / 0.0;
  _Bool  b = d == d && (d - d) != (d - d);
  if (!b)
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(0.0));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: bool [storage=automatic] = logical_and<bool>(eq<f64, exceptions=ignore>(read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]])), ne<f64, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]])), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_d]]), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE_b]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
