// SLATE-FILECHECK-DEFINES O0
// SLATE-FILECHECK-DEFINES O2
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-PREFIX-ARGS O0 -O0
// SLATE-FILECHECK-PREFIX-ARGS O2 -O2

int g;

int scalar(int x, double d, _Bool b) {
  return __builtin_constant_p(x) + __builtin_constant_p(d) + __builtin_constant_p(b);
}

int folded_or_rejected(int *p, int x) {
  return __builtin_constant_p(3) + __builtin_constant_p(p) + __builtin_constant_p(g++) +
         __builtin_constant_p(x = 1);
}

int local_min(void) {
  int y = 2;
  return __builtin_constant_p(y) && y >= 0;
}

// SLATE-FILECHECK-BEGIN O0
// O0: module {
// O0-NEXT:     target "x86_64-unknown-linux-gnu" {
// O0-NEXT:         endian = little;
// O0-NEXT:         pointer [size=8, align=8];
// O0-NEXT:         stack_alignment = 16;
// O0-NEXT:         long_double = f80;
// O0-NEXT:         storage bool [size=1, align=1];
// O0-NEXT:         storage i8, u8 [size=1, align=1];
// O0-NEXT:         storage i16, u16 [size=2, align=2];
// O0-NEXT:         storage i32, u32 [size=4, align=4];
// O0-NEXT:         storage i64, u64 [size=8, align=8];
// O0-NEXT:         storage i128, u128 [size=16, align=16];
// O0-NEXT:         storage bf16 [size=2, align=2];
// O0-NEXT:         storage f16 [size=2, align=2];
// O0-NEXT:         storage f32 [size=4, align=4];
// O0-NEXT:         storage f64 [size=8, align=8];
// O0-NEXT:         storage f80 [size=16, align=16];
// O0-NEXT:         storage f128 [size=16, align=16];
// O0-NEXT:         storage d32 [size=4, align=4];
// O0-NEXT:         storage d64 [size=8, align=8];
// O0-NEXT:         storage d128 [size=16, align=16];
// O0-NEXT:     }
// O0-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// O0-NEXT:     fn %[[VALUE_scalar:[0-9]+]] @scalar(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_b:[0-9]+]] b: bool) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(0), const<i32>(0)), const<i32>(0));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_folded_or_rejected:[0-9]+]] @folded_or_rejected(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(0)), const<i32>(0)), const<i32>(0));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_local_min:[0-9]+]] @local_min() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = const<i32>(2);
// O0-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0))));
// O0-NEXT:     }
// O0-NEXT: }
// SLATE-FILECHECK-END O0
// SLATE-FILECHECK-BEGIN O2
// O2: module {
// O2-NEXT:     target "x86_64-unknown-linux-gnu" {
// O2-NEXT:         endian = little;
// O2-NEXT:         pointer [size=8, align=8];
// O2-NEXT:         stack_alignment = 16;
// O2-NEXT:         long_double = f80;
// O2-NEXT:         storage bool [size=1, align=1];
// O2-NEXT:         storage i8, u8 [size=1, align=1];
// O2-NEXT:         storage i16, u16 [size=2, align=2];
// O2-NEXT:         storage i32, u32 [size=4, align=4];
// O2-NEXT:         storage i64, u64 [size=8, align=8];
// O2-NEXT:         storage i128, u128 [size=16, align=16];
// O2-NEXT:         storage bf16 [size=2, align=2];
// O2-NEXT:         storage f16 [size=2, align=2];
// O2-NEXT:         storage f32 [size=4, align=4];
// O2-NEXT:         storage f64 [size=8, align=8];
// O2-NEXT:         storage f80 [size=16, align=16];
// O2-NEXT:         storage f128 [size=16, align=16];
// O2-NEXT:         storage d32 [size=4, align=4];
// O2-NEXT:         storage d64 [size=8, align=8];
// O2-NEXT:         storage d128 [size=16, align=16];
// O2-NEXT:     }
// O2-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// O2-NEXT:     fn %[[VALUE_scalar:[0-9]+]] @scalar(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_b:[0-9]+]] b: bool) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(intrinsic<bool, llvm.is.constant>(read<i32>(%[[VALUE_x]]))), from_bool<i32, reason=promotion>(intrinsic<bool, llvm.is.constant>(read<f64>(%[[VALUE_d]])))), from_bool<i32, reason=promotion>(intrinsic<bool, llvm.is.constant>(read<bool>(%[[VALUE_b]]))));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_folded_or_rejected:[0-9]+]] @folded_or_rejected(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(0)), const<i32>(0)), const<i32>(0));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_local_min:[0-9]+]] @local_min() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = const<i32>(2);
// O2-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(intrinsic<bool, llvm.is.constant>(read<i32>(%[[VALUE_y]])), ge<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0))));
// O2-NEXT:     }
// O2-NEXT: }
// SLATE-FILECHECK-END O2
