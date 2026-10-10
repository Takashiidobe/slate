// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-DEFINES C23

#ifdef __FUNCTION__
int function_defined = 1;
#endif
#ifdef __STDC_NO_ATOMICS__
int no_atomics = __STDC_NO_ATOMICS__;
#endif
#ifdef __STDC_NO_COMPLEX__
int no_complex = __STDC_NO_COMPLEX__;
#endif
#ifdef __STDC_NO_VLA__
int no_vla = __STDC_NO_VLA__;
#endif
int integral_bits = _INTEGRAL_MAX_BITS;

// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-pc-windows-msvc" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f64;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage bf16 [size=2, align=2];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:         storage d32 [size=4, align=4];
// C17-NEXT:         storage d64 [size=8, align=8];
// C17-NEXT:         storage d128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     global %[[VALUE_function_defined:[0-9]+]] function_defined: i32 [storage=static] = const<i32>(1) [linkage=external];
// C17-NEXT:     global %[[VALUE_no_atomics:[0-9]+]] no_atomics: i32 [storage=static] = const<i32>(1) [linkage=external];
// C17-NEXT:     global %[[VALUE_no_complex:[0-9]+]] no_complex: i32 [storage=static] = const<i32>(1) [linkage=external];
// C17-NEXT:     global %[[VALUE_no_vla:[0-9]+]] no_vla: i32 [storage=static] = const<i32>(1) [linkage=external];
// C17-NEXT:     global %[[VALUE_integral_bits:[0-9]+]] integral_bits: i32 [storage=static] = const<i32>(64) [linkage=external];
// C17-NEXT: }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-pc-windows-msvc" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f64;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %[[VALUE_function_defined:[0-9]+]] function_defined: i32 [storage=static] = const<i32>(1) [linkage=external];
// C23-NEXT:     global %[[VALUE_no_atomics:[0-9]+]] no_atomics: i32 [storage=static] = const<i32>(1) [linkage=external];
// C23-NEXT:     global %[[VALUE_no_complex:[0-9]+]] no_complex: i32 [storage=static] = const<i32>(1) [linkage=external];
// C23-NEXT:     global %[[VALUE_no_vla:[0-9]+]] no_vla: i32 [storage=static] = const<i32>(1) [linkage=external];
// C23-NEXT:     global %[[VALUE_integral_bits:[0-9]+]] integral_bits: i32 [storage=static] = const<i32>(64) [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
