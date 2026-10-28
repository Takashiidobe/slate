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
// SLATE-FILECHECK-DEFINES VERSION
// SLATE-FILECHECK-PREFIX-ARGS VERSION -fopenmp -fopenmp-version=45
// SLATE-FILECHECK-DEFINES UNKNOWN_VERSION
// SLATE-FILECHECK-PREFIX-ARGS UNKNOWN_VERSION -fopenmp -fopenmp-version=99
// SLATE-FILECHECK-DEFINES LIBGOMP
// SLATE-FILECHECK-PREFIX-ARGS LIBGOMP -fopenmp=libgomp
// SLATE-FILECHECK-DEFINES LIBGOMP_THEN_OPENMP
// SLATE-FILECHECK-PREFIX-ARGS LIBGOMP_THEN_OPENMP -fopenmp=libgomp -fopenmp
// SLATE-FILECHECK-DEFINES LIBIOMP5
// SLATE-FILECHECK-PREFIX-ARGS LIBIOMP5 -fopenmp=libiomp5

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
// OPENMP-NEXT:     global %[[VALUE_openmp:[0-9]+]] openmp: i32 [storage=static] = const<i32>(202011) [linkage=external];
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
// OPENMP-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..read<i32>(%[[VALUE_count]])
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
// NEGATED-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..read<i32>(%[[VALUE_count]])
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
// SIMD-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..read<i32>(%[[VALUE_count]])
// SIMD-NEXT:             body:
// SIMD-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// SIMD-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// SIMD-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// SIMD-NEXT:         return read<i32>(%[[VALUE_total]]);
// SIMD-NEXT:     }
// SIMD-NEXT: }
// SLATE-FILECHECK-END SIMD
// SLATE-FILECHECK-BEGIN VERSION
// VERSION: module {
// VERSION-NEXT:     target "x86_64-unknown-linux-gnu" {
// VERSION-NEXT:         endian = little;
// VERSION-NEXT:         pointer [size=8, align=8];
// VERSION-NEXT:         stack_alignment = 16;
// VERSION-NEXT:         long_double = f80;
// VERSION-NEXT:         storage bool [size=1, align=1];
// VERSION-NEXT:         storage i8, u8 [size=1, align=1];
// VERSION-NEXT:         storage i16, u16 [size=2, align=2];
// VERSION-NEXT:         storage i32, u32 [size=4, align=4];
// VERSION-NEXT:         storage i64, u64 [size=8, align=8];
// VERSION-NEXT:         storage i128, u128 [size=16, align=16];
// VERSION-NEXT:         storage bf16 [size=2, align=2];
// VERSION-NEXT:         storage f16 [size=2, align=2];
// VERSION-NEXT:         storage f32 [size=4, align=4];
// VERSION-NEXT:         storage f64 [size=8, align=8];
// VERSION-NEXT:         storage f80 [size=16, align=16];
// VERSION-NEXT:         storage f128 [size=16, align=16];
// VERSION-NEXT:         storage d32 [size=4, align=4];
// VERSION-NEXT:         storage d64 [size=8, align=8];
// VERSION-NEXT:         storage d128 [size=16, align=16];
// VERSION-NEXT:     }
// VERSION-NEXT:     global %[[VALUE_openmp:[0-9]+]] openmp: i32 [storage=static] = const<i32>(201511) [linkage=external];
// VERSION-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_count:[0-9]+]] count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// VERSION-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// VERSION-NEXT:         for %[[VALUE0:[0-9]+]]
// VERSION-NEXT:             init:
// VERSION-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// VERSION-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// VERSION-NEXT:             increment: {
// VERSION-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// VERSION-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// VERSION-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// VERSION-NEXT:                 yield void;
// VERSION-NEXT:             }
// VERSION-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..read<i32>(%[[VALUE_count]])
// VERSION-NEXT:             body:
// VERSION-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// VERSION-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// VERSION-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// VERSION-NEXT:         return read<i32>(%[[VALUE_total]]);
// VERSION-NEXT:     }
// VERSION-NEXT: }
// SLATE-FILECHECK-END VERSION
// SLATE-FILECHECK-BEGIN UNKNOWN_VERSION
// UNKNOWN_VERSION: module {
// UNKNOWN_VERSION-NEXT:     target "x86_64-unknown-linux-gnu" {
// UNKNOWN_VERSION-NEXT:         endian = little;
// UNKNOWN_VERSION-NEXT:         pointer [size=8, align=8];
// UNKNOWN_VERSION-NEXT:         stack_alignment = 16;
// UNKNOWN_VERSION-NEXT:         long_double = f80;
// UNKNOWN_VERSION-NEXT:         storage bool [size=1, align=1];
// UNKNOWN_VERSION-NEXT:         storage i8, u8 [size=1, align=1];
// UNKNOWN_VERSION-NEXT:         storage i16, u16 [size=2, align=2];
// UNKNOWN_VERSION-NEXT:         storage i32, u32 [size=4, align=4];
// UNKNOWN_VERSION-NEXT:         storage i64, u64 [size=8, align=8];
// UNKNOWN_VERSION-NEXT:         storage i128, u128 [size=16, align=16];
// UNKNOWN_VERSION-NEXT:         storage bf16 [size=2, align=2];
// UNKNOWN_VERSION-NEXT:         storage f16 [size=2, align=2];
// UNKNOWN_VERSION-NEXT:         storage f32 [size=4, align=4];
// UNKNOWN_VERSION-NEXT:         storage f64 [size=8, align=8];
// UNKNOWN_VERSION-NEXT:         storage f80 [size=16, align=16];
// UNKNOWN_VERSION-NEXT:         storage f128 [size=16, align=16];
// UNKNOWN_VERSION-NEXT:         storage d32 [size=4, align=4];
// UNKNOWN_VERSION-NEXT:         storage d64 [size=8, align=8];
// UNKNOWN_VERSION-NEXT:         storage d128 [size=16, align=16];
// UNKNOWN_VERSION-NEXT:     }
// UNKNOWN_VERSION-NEXT:     global %[[VALUE_openmp:[0-9]+]] openmp: i32 [storage=static] = const<i32>(202011) [linkage=external];
// UNKNOWN_VERSION-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_count:[0-9]+]] count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// UNKNOWN_VERSION-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// UNKNOWN_VERSION-NEXT:         for %[[VALUE0:[0-9]+]]
// UNKNOWN_VERSION-NEXT:             init:
// UNKNOWN_VERSION-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// UNKNOWN_VERSION-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// UNKNOWN_VERSION-NEXT:             increment: {
// UNKNOWN_VERSION-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// UNKNOWN_VERSION-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// UNKNOWN_VERSION-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// UNKNOWN_VERSION-NEXT:                 yield void;
// UNKNOWN_VERSION-NEXT:             }
// UNKNOWN_VERSION-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..read<i32>(%[[VALUE_count]])
// UNKNOWN_VERSION-NEXT:             body:
// UNKNOWN_VERSION-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// UNKNOWN_VERSION-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// UNKNOWN_VERSION-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// UNKNOWN_VERSION-NEXT:         return read<i32>(%[[VALUE_total]]);
// UNKNOWN_VERSION-NEXT:     }
// UNKNOWN_VERSION-NEXT: }
// SLATE-FILECHECK-END UNKNOWN_VERSION
// SLATE-FILECHECK-BEGIN LIBGOMP
// LIBGOMP: module {
// LIBGOMP-NEXT:     target "x86_64-unknown-linux-gnu" {
// LIBGOMP-NEXT:         endian = little;
// LIBGOMP-NEXT:         pointer [size=8, align=8];
// LIBGOMP-NEXT:         stack_alignment = 16;
// LIBGOMP-NEXT:         long_double = f80;
// LIBGOMP-NEXT:         storage bool [size=1, align=1];
// LIBGOMP-NEXT:         storage i8, u8 [size=1, align=1];
// LIBGOMP-NEXT:         storage i16, u16 [size=2, align=2];
// LIBGOMP-NEXT:         storage i32, u32 [size=4, align=4];
// LIBGOMP-NEXT:         storage i64, u64 [size=8, align=8];
// LIBGOMP-NEXT:         storage i128, u128 [size=16, align=16];
// LIBGOMP-NEXT:         storage bf16 [size=2, align=2];
// LIBGOMP-NEXT:         storage f16 [size=2, align=2];
// LIBGOMP-NEXT:         storage f32 [size=4, align=4];
// LIBGOMP-NEXT:         storage f64 [size=8, align=8];
// LIBGOMP-NEXT:         storage f80 [size=16, align=16];
// LIBGOMP-NEXT:         storage f128 [size=16, align=16];
// LIBGOMP-NEXT:         storage d32 [size=4, align=4];
// LIBGOMP-NEXT:         storage d64 [size=8, align=8];
// LIBGOMP-NEXT:         storage d128 [size=16, align=16];
// LIBGOMP-NEXT:     }
// LIBGOMP-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_count:[0-9]+]] count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// LIBGOMP-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// LIBGOMP-NEXT:         for %[[VALUE0:[0-9]+]]
// LIBGOMP-NEXT:             init:
// LIBGOMP-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// LIBGOMP-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// LIBGOMP-NEXT:             increment: {
// LIBGOMP-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// LIBGOMP-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// LIBGOMP-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// LIBGOMP-NEXT:                 yield void;
// LIBGOMP-NEXT:             }
// LIBGOMP-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..read<i32>(%[[VALUE_count]])
// LIBGOMP-NEXT:             body:
// LIBGOMP-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// LIBGOMP-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// LIBGOMP-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// LIBGOMP-NEXT:         return read<i32>(%[[VALUE_total]]);
// LIBGOMP-NEXT:     }
// LIBGOMP-NEXT: }
// SLATE-FILECHECK-END LIBGOMP
// SLATE-FILECHECK-BEGIN LIBGOMP_THEN_OPENMP
// LIBGOMP_THEN_OPENMP: module {
// LIBGOMP_THEN_OPENMP-NEXT:     target "x86_64-unknown-linux-gnu" {
// LIBGOMP_THEN_OPENMP-NEXT:         endian = little;
// LIBGOMP_THEN_OPENMP-NEXT:         pointer [size=8, align=8];
// LIBGOMP_THEN_OPENMP-NEXT:         stack_alignment = 16;
// LIBGOMP_THEN_OPENMP-NEXT:         long_double = f80;
// LIBGOMP_THEN_OPENMP-NEXT:         storage bool [size=1, align=1];
// LIBGOMP_THEN_OPENMP-NEXT:         storage i8, u8 [size=1, align=1];
// LIBGOMP_THEN_OPENMP-NEXT:         storage i16, u16 [size=2, align=2];
// LIBGOMP_THEN_OPENMP-NEXT:         storage i32, u32 [size=4, align=4];
// LIBGOMP_THEN_OPENMP-NEXT:         storage i64, u64 [size=8, align=8];
// LIBGOMP_THEN_OPENMP-NEXT:         storage i128, u128 [size=16, align=16];
// LIBGOMP_THEN_OPENMP-NEXT:         storage bf16 [size=2, align=2];
// LIBGOMP_THEN_OPENMP-NEXT:         storage f16 [size=2, align=2];
// LIBGOMP_THEN_OPENMP-NEXT:         storage f32 [size=4, align=4];
// LIBGOMP_THEN_OPENMP-NEXT:         storage f64 [size=8, align=8];
// LIBGOMP_THEN_OPENMP-NEXT:         storage f80 [size=16, align=16];
// LIBGOMP_THEN_OPENMP-NEXT:         storage f128 [size=16, align=16];
// LIBGOMP_THEN_OPENMP-NEXT:         storage d32 [size=4, align=4];
// LIBGOMP_THEN_OPENMP-NEXT:         storage d64 [size=8, align=8];
// LIBGOMP_THEN_OPENMP-NEXT:         storage d128 [size=16, align=16];
// LIBGOMP_THEN_OPENMP-NEXT:     }
// LIBGOMP_THEN_OPENMP-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_count:[0-9]+]] count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// LIBGOMP_THEN_OPENMP-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// LIBGOMP_THEN_OPENMP-NEXT:         for %[[VALUE0:[0-9]+]]
// LIBGOMP_THEN_OPENMP-NEXT:             init:
// LIBGOMP_THEN_OPENMP-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// LIBGOMP_THEN_OPENMP-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// LIBGOMP_THEN_OPENMP-NEXT:             increment: {
// LIBGOMP_THEN_OPENMP-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// LIBGOMP_THEN_OPENMP-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// LIBGOMP_THEN_OPENMP-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// LIBGOMP_THEN_OPENMP-NEXT:                 yield void;
// LIBGOMP_THEN_OPENMP-NEXT:             }
// LIBGOMP_THEN_OPENMP-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..read<i32>(%[[VALUE_count]])
// LIBGOMP_THEN_OPENMP-NEXT:             body:
// LIBGOMP_THEN_OPENMP-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// LIBGOMP_THEN_OPENMP-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// LIBGOMP_THEN_OPENMP-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// LIBGOMP_THEN_OPENMP-NEXT:         return read<i32>(%[[VALUE_total]]);
// LIBGOMP_THEN_OPENMP-NEXT:     }
// LIBGOMP_THEN_OPENMP-NEXT: }
// SLATE-FILECHECK-END LIBGOMP_THEN_OPENMP
// SLATE-FILECHECK-BEGIN LIBIOMP5
// LIBIOMP5: module {
// LIBIOMP5-NEXT:     target "x86_64-unknown-linux-gnu" {
// LIBIOMP5-NEXT:         endian = little;
// LIBIOMP5-NEXT:         pointer [size=8, align=8];
// LIBIOMP5-NEXT:         stack_alignment = 16;
// LIBIOMP5-NEXT:         long_double = f80;
// LIBIOMP5-NEXT:         storage bool [size=1, align=1];
// LIBIOMP5-NEXT:         storage i8, u8 [size=1, align=1];
// LIBIOMP5-NEXT:         storage i16, u16 [size=2, align=2];
// LIBIOMP5-NEXT:         storage i32, u32 [size=4, align=4];
// LIBIOMP5-NEXT:         storage i64, u64 [size=8, align=8];
// LIBIOMP5-NEXT:         storage i128, u128 [size=16, align=16];
// LIBIOMP5-NEXT:         storage bf16 [size=2, align=2];
// LIBIOMP5-NEXT:         storage f16 [size=2, align=2];
// LIBIOMP5-NEXT:         storage f32 [size=4, align=4];
// LIBIOMP5-NEXT:         storage f64 [size=8, align=8];
// LIBIOMP5-NEXT:         storage f80 [size=16, align=16];
// LIBIOMP5-NEXT:         storage f128 [size=16, align=16];
// LIBIOMP5-NEXT:         storage d32 [size=4, align=4];
// LIBIOMP5-NEXT:         storage d64 [size=8, align=8];
// LIBIOMP5-NEXT:         storage d128 [size=16, align=16];
// LIBIOMP5-NEXT:     }
// LIBIOMP5-NEXT:     global %[[VALUE_openmp:[0-9]+]] openmp: i32 [storage=static] = const<i32>(202011) [linkage=external];
// LIBIOMP5-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_values:[0-9]+]] values: ptr<const i32>, %[[VALUE_count:[0-9]+]] count: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// LIBIOMP5-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// LIBIOMP5-NEXT:         for %[[VALUE0:[0-9]+]]
// LIBIOMP5-NEXT:             init:
// LIBIOMP5-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// LIBIOMP5-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// LIBIOMP5-NEXT:             increment: {
// LIBIOMP5-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// LIBIOMP5-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE1]]), const<i32>(1));
// LIBIOMP5-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// LIBIOMP5-NEXT:                 yield void;
// LIBIOMP5-NEXT:             }
// LIBIOMP5-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..read<i32>(%[[VALUE_count]])
// LIBIOMP5-NEXT:             body:
// LIBIOMP5-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// LIBIOMP5-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE3]]), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false>(read<ptr<const i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE_i]])))));
// LIBIOMP5-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// LIBIOMP5-NEXT:         return read<i32>(%[[VALUE_total]]);
// LIBIOMP5-NEXT:     }
// LIBIOMP5-NEXT: }
// SLATE-FILECHECK-END LIBIOMP5
