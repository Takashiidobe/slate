/* PR rtl-optimization/119071 */

int a, b;

int main() {
  int c = 0;
  if (a + 2)
    c = 1;
  int d = (1 + c - 2 + c == 1) - 1;
  b     = ((d + 1) << d) + d;
  if (b != 1)
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
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_c]], const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic] = sub<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), read<i32>(%[[VALUE_c]])), const<i32>(2)), read<i32>(%[[VALUE_c]])), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], add<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_d]]), const<i32>(1)), read<i32>(%[[VALUE_d]])), read<i32>(%[[VALUE_d]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
