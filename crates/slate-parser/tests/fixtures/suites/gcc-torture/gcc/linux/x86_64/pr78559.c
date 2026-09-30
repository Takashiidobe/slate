/* PR rtl-optimization/78559 */

int g = 20;
int d = 0;

short fn2(int p1, int p2) { return p2 >= 2 || 5 >> p2 ? p1 : p1 << p2; }

int main() {
  int result = 0;
lbl_2582:
  if (g) {
    for (int c = -3; c; c++)
      result = fn2(1, g);
  } else {
    for (int i = 0; i < 2; i += 2)
      if (d)
        goto lbl_2582;
  }
  if (result != 1)
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
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] = const<i32>(20) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE_p1:[0-9]+]] p1: i32, %[[VALUE_p2:[0-9]+]] p2: i32) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(conditional<i32>(logical_or<bool>(ge<i32>(read<i32>(%[[VALUE_p2]]), const<i32>(2)), ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(5), read<i32>(%[[VALUE_p2]])), const<i32>(0))), read<i32>(%[[VALUE_p1]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_p1]]), read<i32>(%[[VALUE_p2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         label %[[VALUE_lbl_2582:[0-9]+]] lbl_2582:
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_g]]), const<i32>(0))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(3));
// DEFAULT-NEXT:                         condition: ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_result]], widen<i32, reason=assign>(call<i16, signature=fn(i32, i32) -> i16>(%[[VALUE_fn2]], const<i32>(1), read<i32>(%[[VALUE_g]]))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(2));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%[[VALUE_d]]), const<i32>(0))
// DEFAULT-NEXT:                                 goto %[[VALUE_lbl_2582]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_result]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
