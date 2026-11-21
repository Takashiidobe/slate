#ifdef _OPENMP
int openmp = _OPENMP;
#endif
#ifdef _REENTRANT
int reentrant = _REENTRANT;
#endif

int sum(const int *values, int count) {
  int total = 0;
#pragma omp simd reduction(+ : total)
  for (int i = 0; i < count; i++)
    total += values[i];
  return total;
}

// SLATE-FILECHECK-ARGS --compact-ir
// SLATE-FILECHECK-DEFINES OPENMP
// SLATE-FILECHECK-PREFIX-ARGS OPENMP -fopenmp
// SLATE-FILECHECK-DEFINES NEGATED
// SLATE-FILECHECK-PREFIX-ARGS NEGATED -fopenmp -fno-openmp
// SLATE-FILECHECK-DEFINES SIMD
// SLATE-FILECHECK-PREFIX-ARGS SIMD -fopenmp-simd
// SLATE-FILECHECK-DEFINES SIMD_THEN_OPENMP
// SLATE-FILECHECK-PREFIX-ARGS SIMD_THEN_OPENMP -fopenmp-simd -fopenmp

// SLATE-FILECHECK-BEGIN OPENMP
// OPENMP: module {
// OPENMP-NEXT:     target "x86_64-unknown-linux-gnu" {
// OPENMP-NEXT:         endian = little;
// OPENMP-NEXT:         pointer [size=8, align=8];
// OPENMP-NEXT:         stack_alignment = 16;
// OPENMP-NEXT:         long_double = f80;
// OPENMP-NEXT:         storage bool [size=1, align=1];
// OPENMP-NEXT:         storage i8, u8 [size=1, align=1];
// OPENMP-NEXT:         storage i16, u16 [size=2, align=2];
// OPENMP-NEXT:         storage i32, u32 [size=4, align=4];
// OPENMP-NEXT:         storage i64, u64 [size=8, align=8];
// OPENMP-NEXT:         storage i128, u128 [size=16, align=16];
// OPENMP-NEXT:         storage bf16 [size=2, align=2];
// OPENMP-NEXT:         storage f16 [size=2, align=2];
// OPENMP-NEXT:         storage f32 [size=4, align=4];
// OPENMP-NEXT:         storage f64 [size=8, align=8];
// OPENMP-NEXT:         storage f80 [size=16, align=16];
// OPENMP-NEXT:         storage f128 [size=16, align=16];
// OPENMP-NEXT:         storage d32 [size=4, align=4];
// OPENMP-NEXT:         storage d64 [size=8, align=8];
// OPENMP-NEXT:         storage d128 [size=16, align=16];
// OPENMP-NEXT:     }
// OPENMP-NEXT:     global %[[VALUE_openmp:[0-9]+]] openmp: i32 [storage=static] = const<i32>(202111) [linkage=external];
// OPENMP-NEXT:     global %[[VALUE_reentrant:[0-9]+]] reentrant: i32 [storage=static] = const<i32>(1) [linkage=external];
// OPENMP-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_count:[0-9]+]] count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// OPENMP-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// OPENMP-NEXT:         for %[[VALUE0:[0-9]+]]
// OPENMP-NEXT:             init:
// OPENMP-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// OPENMP-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// OPENMP-NEXT:             increment: {
// OPENMP-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// OPENMP-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// OPENMP-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// OPENMP-NEXT:                 yield void;
// OPENMP-NEXT:             }
// OPENMP-NEXT:             body:
// OPENMP-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// OPENMP-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// OPENMP-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// OPENMP-NEXT:         return read<i32>(%[[VALUE_total]]);
// OPENMP-NEXT:     }
// OPENMP-NEXT: }
// SLATE-FILECHECK-END OPENMP
// SLATE-FILECHECK-BEGIN NEGATED
// NEGATED: module {
// NEGATED-NEXT:     target "x86_64-unknown-linux-gnu" {
// NEGATED-NEXT:         endian = little;
// NEGATED-NEXT:         pointer [size=8, align=8];
// NEGATED-NEXT:         stack_alignment = 16;
// NEGATED-NEXT:         long_double = f80;
// NEGATED-NEXT:         storage bool [size=1, align=1];
// NEGATED-NEXT:         storage i8, u8 [size=1, align=1];
// NEGATED-NEXT:         storage i16, u16 [size=2, align=2];
// NEGATED-NEXT:         storage i32, u32 [size=4, align=4];
// NEGATED-NEXT:         storage i64, u64 [size=8, align=8];
// NEGATED-NEXT:         storage i128, u128 [size=16, align=16];
// NEGATED-NEXT:         storage bf16 [size=2, align=2];
// NEGATED-NEXT:         storage f16 [size=2, align=2];
// NEGATED-NEXT:         storage f32 [size=4, align=4];
// NEGATED-NEXT:         storage f64 [size=8, align=8];
// NEGATED-NEXT:         storage f80 [size=16, align=16];
// NEGATED-NEXT:         storage f128 [size=16, align=16];
// NEGATED-NEXT:         storage d32 [size=4, align=4];
// NEGATED-NEXT:         storage d64 [size=8, align=8];
// NEGATED-NEXT:         storage d128 [size=16, align=16];
// NEGATED-NEXT:     }
// NEGATED-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_count:[0-9]+]] count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// NEGATED-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// NEGATED-NEXT:         for %[[VALUE0:[0-9]+]]
// NEGATED-NEXT:             init:
// NEGATED-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// NEGATED-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// NEGATED-NEXT:             increment: {
// NEGATED-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// NEGATED-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// NEGATED-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// NEGATED-NEXT:                 yield void;
// NEGATED-NEXT:             }
// NEGATED-NEXT:             body:
// NEGATED-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// NEGATED-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// NEGATED-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// NEGATED-NEXT:         return read<i32>(%[[VALUE_total]]);
// NEGATED-NEXT:     }
// NEGATED-NEXT: }
// SLATE-FILECHECK-END NEGATED
// SLATE-FILECHECK-BEGIN SIMD
// SIMD: module {
// SIMD-NEXT:     target "x86_64-unknown-linux-gnu" {
// SIMD-NEXT:         endian = little;
// SIMD-NEXT:         pointer [size=8, align=8];
// SIMD-NEXT:         stack_alignment = 16;
// SIMD-NEXT:         long_double = f80;
// SIMD-NEXT:         storage bool [size=1, align=1];
// SIMD-NEXT:         storage i8, u8 [size=1, align=1];
// SIMD-NEXT:         storage i16, u16 [size=2, align=2];
// SIMD-NEXT:         storage i32, u32 [size=4, align=4];
// SIMD-NEXT:         storage i64, u64 [size=8, align=8];
// SIMD-NEXT:         storage i128, u128 [size=16, align=16];
// SIMD-NEXT:         storage bf16 [size=2, align=2];
// SIMD-NEXT:         storage f16 [size=2, align=2];
// SIMD-NEXT:         storage f32 [size=4, align=4];
// SIMD-NEXT:         storage f64 [size=8, align=8];
// SIMD-NEXT:         storage f80 [size=16, align=16];
// SIMD-NEXT:         storage f128 [size=16, align=16];
// SIMD-NEXT:         storage d32 [size=4, align=4];
// SIMD-NEXT:         storage d64 [size=8, align=8];
// SIMD-NEXT:         storage d128 [size=16, align=16];
// SIMD-NEXT:     }
// SIMD-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_count:[0-9]+]] count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SIMD-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// SIMD-NEXT:         for %[[VALUE0:[0-9]+]]
// SIMD-NEXT:             init:
// SIMD-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// SIMD-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// SIMD-NEXT:             increment: {
// SIMD-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// SIMD-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// SIMD-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// SIMD-NEXT:                 yield void;
// SIMD-NEXT:             }
// SIMD-NEXT:             body:
// SIMD-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// SIMD-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// SIMD-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// SIMD-NEXT:         return read<i32>(%[[VALUE_total]]);
// SIMD-NEXT:     }
// SIMD-NEXT: }
// SLATE-FILECHECK-END SIMD
// SLATE-FILECHECK-BEGIN SIMD_THEN_OPENMP
// SIMD_THEN_OPENMP: module {
// SIMD_THEN_OPENMP-NEXT:     target "x86_64-unknown-linux-gnu" {
// SIMD_THEN_OPENMP-NEXT:         endian = little;
// SIMD_THEN_OPENMP-NEXT:         pointer [size=8, align=8];
// SIMD_THEN_OPENMP-NEXT:         stack_alignment = 16;
// SIMD_THEN_OPENMP-NEXT:         long_double = f80;
// SIMD_THEN_OPENMP-NEXT:         storage bool [size=1, align=1];
// SIMD_THEN_OPENMP-NEXT:         storage i8, u8 [size=1, align=1];
// SIMD_THEN_OPENMP-NEXT:         storage i16, u16 [size=2, align=2];
// SIMD_THEN_OPENMP-NEXT:         storage i32, u32 [size=4, align=4];
// SIMD_THEN_OPENMP-NEXT:         storage i64, u64 [size=8, align=8];
// SIMD_THEN_OPENMP-NEXT:         storage i128, u128 [size=16, align=16];
// SIMD_THEN_OPENMP-NEXT:         storage bf16 [size=2, align=2];
// SIMD_THEN_OPENMP-NEXT:         storage f16 [size=2, align=2];
// SIMD_THEN_OPENMP-NEXT:         storage f32 [size=4, align=4];
// SIMD_THEN_OPENMP-NEXT:         storage f64 [size=8, align=8];
// SIMD_THEN_OPENMP-NEXT:         storage f80 [size=16, align=16];
// SIMD_THEN_OPENMP-NEXT:         storage f128 [size=16, align=16];
// SIMD_THEN_OPENMP-NEXT:         storage d32 [size=4, align=4];
// SIMD_THEN_OPENMP-NEXT:         storage d64 [size=8, align=8];
// SIMD_THEN_OPENMP-NEXT:         storage d128 [size=16, align=16];
// SIMD_THEN_OPENMP-NEXT:     }
// SIMD_THEN_OPENMP-NEXT:     global %[[VALUE_openmp:[0-9]+]] openmp: i32 [storage=static] = const<i32>(202111) [linkage=external];
// SIMD_THEN_OPENMP-NEXT:     global %[[VALUE_reentrant:[0-9]+]] reentrant: i32 [storage=static] = const<i32>(1) [linkage=external];
// SIMD_THEN_OPENMP-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_count:[0-9]+]] count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SIMD_THEN_OPENMP-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// SIMD_THEN_OPENMP-NEXT:         for %[[VALUE0:[0-9]+]]
// SIMD_THEN_OPENMP-NEXT:             init:
// SIMD_THEN_OPENMP-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// SIMD_THEN_OPENMP-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// SIMD_THEN_OPENMP-NEXT:             increment: {
// SIMD_THEN_OPENMP-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// SIMD_THEN_OPENMP-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// SIMD_THEN_OPENMP-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// SIMD_THEN_OPENMP-NEXT:                 yield void;
// SIMD_THEN_OPENMP-NEXT:             }
// SIMD_THEN_OPENMP-NEXT:             body:
// SIMD_THEN_OPENMP-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// SIMD_THEN_OPENMP-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// SIMD_THEN_OPENMP-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// SIMD_THEN_OPENMP-NEXT:         return read<i32>(%[[VALUE_total]]);
// SIMD_THEN_OPENMP-NEXT:     }
// SIMD_THEN_OPENMP-NEXT: }
// SLATE-FILECHECK-END SIMD_THEN_OPENMP
