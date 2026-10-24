#pragma GCC optimize ("Ofast")
#ifdef __FAST_MATH__
int ofast_fast;
#endif
#ifdef __OPTIMIZE__
int ofast_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int ofast_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int ofast_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int ofast_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int ofast_no_zeros;
#endif

#pragma GCC optimize ("-Ofast")
#ifdef __FAST_MATH__
int dash_ofast_fast;
#endif
#ifdef __OPTIMIZE__
int dash_ofast_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int dash_ofast_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int dash_ofast_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int dash_ofast_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int dash_ofast_no_zeros;
#endif

#pragma GCC optimize ("fast-math")
#ifdef __FAST_MATH__
int fast_math_fast;
#endif
#ifdef __OPTIMIZE__
int fast_math_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int fast_math_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int fast_math_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int fast_math_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int fast_math_no_zeros;
#endif

#pragma GCC optimize ("Ofast")
#pragma GCC optimize ("O2")
#ifdef __FAST_MATH__
int ofast_then_o2_fast;
#endif
#ifdef __OPTIMIZE__
int ofast_then_o2_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int ofast_then_o2_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int ofast_then_o2_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int ofast_then_o2_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int ofast_then_o2_no_zeros;
#endif

#pragma GCC optimize ("Ofast,O1")
#ifdef __FAST_MATH__
int list_ofast_o1_fast;
#endif
#ifdef __OPTIMIZE__
int list_ofast_o1_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int list_ofast_o1_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int list_ofast_o1_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int list_ofast_o1_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int list_ofast_o1_no_zeros;
#endif

#pragma GCC optimize ("O2,Ofast")
#ifdef __FAST_MATH__
int list_o2_ofast_fast;
#endif
#ifdef __OPTIMIZE__
int list_o2_ofast_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int list_o2_ofast_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int list_o2_ofast_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int list_o2_ofast_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int list_o2_ofast_no_zeros;
#endif

#pragma GCC optimize ("O2","Ofast","O3")
#ifdef __FAST_MATH__
int list_o2_ofast_o3_fast;
#endif
#ifdef __OPTIMIZE__
int list_o2_ofast_o3_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int list_o2_ofast_o3_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int list_o2_ofast_o3_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int list_o2_ofast_o3_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int list_o2_ofast_o3_no_zeros;
#endif

#pragma GCC optimize ("Ofast","O1","Ofast")
#ifdef __FAST_MATH__
int list_ofast_ofast_fast;
#endif
#ifdef __OPTIMIZE__
int list_ofast_ofast_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int list_ofast_ofast_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int list_ofast_ofast_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int list_ofast_ofast_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int list_ofast_ofast_no_zeros;
#endif

#pragma GCC optimize ("fast-math,O2")
#ifdef __FAST_MATH__
int list_fast_math_o2_fast;
#endif
#ifdef __OPTIMIZE__
int list_fast_math_o2_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int list_fast_math_o2_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int list_fast_math_o2_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int list_fast_math_o2_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int list_fast_math_o2_no_zeros;
#endif

#pragma GCC optimize ("Ofast","no-fast-math")
#ifdef __FAST_MATH__
int list_ofast_no_fast_math_fast;
#endif
#ifdef __OPTIMIZE__
int list_ofast_no_fast_math_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int list_ofast_no_fast_math_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int list_ofast_no_fast_math_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int list_ofast_no_fast_math_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int list_ofast_no_fast_math_no_zeros;
#endif

#pragma GCC optimize ("Ofast","reciprocal-math")
#ifdef __FAST_MATH__
int list_ofast_recip_fast;
#endif
#ifdef __OPTIMIZE__
int list_ofast_recip_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int list_ofast_recip_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int list_ofast_recip_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int list_ofast_recip_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int list_ofast_recip_no_zeros;
#endif

#pragma GCC optimize ("fast-math")
#pragma GCC optimize ("no-math-errno")
#ifdef __FAST_MATH__
int separate_fast_then_errno_fast;
#endif
#ifdef __OPTIMIZE__
int separate_fast_then_errno_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int separate_fast_then_errno_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int separate_fast_then_errno_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int separate_fast_then_errno_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int separate_fast_then_errno_no_zeros;
#endif

#pragma GCC optimize ("reciprocal-math")
#pragma GCC optimize ("O2")
#ifdef __FAST_MATH__
int separate_recip_then_o2_fast;
#endif
#ifdef __OPTIMIZE__
int separate_recip_then_o2_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int separate_recip_then_o2_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int separate_recip_then_o2_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int separate_recip_then_o2_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int separate_recip_then_o2_no_zeros;
#endif

#pragma GCC optimize ("O2")
#pragma GCC optimize ("reciprocal-math")
#ifdef __FAST_MATH__
int separate_o2_then_recip_fast;
#endif
#ifdef __OPTIMIZE__
int separate_o2_then_recip_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int separate_o2_then_recip_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int separate_o2_then_recip_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int separate_o2_then_recip_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int separate_o2_then_recip_no_zeros;
#endif

#pragma GCC push_options
#pragma GCC optimize ("Ofast")
#pragma GCC pop_options
#ifdef __FAST_MATH__
int push_ofast_pop_fast;
#endif
#ifdef __OPTIMIZE__
int push_ofast_pop_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int push_ofast_pop_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int push_ofast_pop_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int push_ofast_pop_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int push_ofast_pop_no_zeros;
#endif

#pragma GCC optimize ("Ofast")
#pragma GCC reset_options
#ifdef __FAST_MATH__
int ofast_reset_fast;
#endif
#ifdef __OPTIMIZE__
int ofast_reset_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int ofast_reset_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int ofast_reset_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int ofast_reset_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int ofast_reset_no_zeros;
#endif

#pragma GCC optimize ("Ofast")
#pragma GCC optimize 3
#ifdef __FAST_MATH__
int number_after_ofast_fast;
#endif
#ifdef __OPTIMIZE__
int number_after_ofast_opt;
#endif
#ifdef __NO_MATH_ERRNO__
int number_after_ofast_no_errno;
#endif
#ifdef __RECIPROCAL_MATH__
int number_after_ofast_recip;
#endif
#ifdef __ASSOCIATIVE_MATH__
int number_after_ofast_assoc;
#endif
#ifdef __NO_SIGNED_ZEROS__
int number_after_ofast_no_zeros;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES O0
// SLATE-FILECHECK-PREFIX-ARGS O0 -O0
// SLATE-FILECHECK-DEFINES O2
// SLATE-FILECHECK-PREFIX-ARGS O2 -O2
// SLATE-FILECHECK-DEFINES ASSOC
// SLATE-FILECHECK-PREFIX-ARGS ASSOC -fassociative-math

// SLATE-FILECHECK-DEFINES NOTRAP
// SLATE-FILECHECK-PREFIX-ARGS NOTRAP -fassociative-math -fno-signed-zeros -fno-trapping-math

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
// O0-NEXT:     global %[[VALUE_ofast_fast:[0-9]+]] ofast_fast: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_ofast_opt:[0-9]+]] ofast_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_ofast_no_errno:[0-9]+]] ofast_no_errno: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_ofast_recip:[0-9]+]] ofast_recip: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_ofast_assoc:[0-9]+]] ofast_assoc: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_ofast_no_zeros:[0-9]+]] ofast_no_zeros: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_dash_ofast_fast:[0-9]+]] dash_ofast_fast: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_dash_ofast_opt:[0-9]+]] dash_ofast_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_dash_ofast_no_errno:[0-9]+]] dash_ofast_no_errno: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_dash_ofast_recip:[0-9]+]] dash_ofast_recip: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_dash_ofast_assoc:[0-9]+]] dash_ofast_assoc: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_dash_ofast_no_zeros:[0-9]+]] dash_ofast_no_zeros: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_fast_math_fast:[0-9]+]] fast_math_fast: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_fast_math_no_errno:[0-9]+]] fast_math_no_errno: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_fast_math_recip:[0-9]+]] fast_math_recip: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_fast_math_assoc:[0-9]+]] fast_math_assoc: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_fast_math_no_zeros:[0-9]+]] fast_math_no_zeros: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_ofast_then_o2_opt:[0-9]+]] ofast_then_o2_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_o1_opt:[0-9]+]] list_ofast_o1_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_o2_ofast_fast:[0-9]+]] list_o2_ofast_fast: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_o2_ofast_opt:[0-9]+]] list_o2_ofast_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_o2_ofast_no_errno:[0-9]+]] list_o2_ofast_no_errno: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_o2_ofast_recip:[0-9]+]] list_o2_ofast_recip: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_o2_ofast_assoc:[0-9]+]] list_o2_ofast_assoc: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_o2_ofast_no_zeros:[0-9]+]] list_o2_ofast_no_zeros: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_o2_ofast_o3_opt:[0-9]+]] list_o2_ofast_o3_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_ofast_fast:[0-9]+]] list_ofast_ofast_fast: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_ofast_opt:[0-9]+]] list_ofast_ofast_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_ofast_no_errno:[0-9]+]] list_ofast_ofast_no_errno: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_ofast_recip:[0-9]+]] list_ofast_ofast_recip: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_ofast_assoc:[0-9]+]] list_ofast_ofast_assoc: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_ofast_no_zeros:[0-9]+]] list_ofast_ofast_no_zeros: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_fast_math_o2_fast:[0-9]+]] list_fast_math_o2_fast: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_fast_math_o2_opt:[0-9]+]] list_fast_math_o2_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_fast_math_o2_no_errno:[0-9]+]] list_fast_math_o2_no_errno: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_fast_math_o2_recip:[0-9]+]] list_fast_math_o2_recip: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_fast_math_o2_assoc:[0-9]+]] list_fast_math_o2_assoc: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_fast_math_o2_no_zeros:[0-9]+]] list_fast_math_o2_no_zeros: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_no_fast_math_opt:[0-9]+]] list_ofast_no_fast_math_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_recip_fast:[0-9]+]] list_ofast_recip_fast: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_recip_opt:[0-9]+]] list_ofast_recip_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_recip_no_errno:[0-9]+]] list_ofast_recip_no_errno: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_recip_recip:[0-9]+]] list_ofast_recip_recip: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_recip_assoc:[0-9]+]] list_ofast_recip_assoc: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_ofast_recip_no_zeros:[0-9]+]] list_ofast_recip_no_zeros: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_separate_fast_then_errno_no_errno:[0-9]+]] separate_fast_then_errno_no_errno: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_separate_recip_then_o2_opt:[0-9]+]] separate_recip_then_o2_opt: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_separate_o2_then_recip_recip:[0-9]+]] separate_o2_then_recip_recip: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_push_ofast_pop_recip:[0-9]+]] push_ofast_pop_recip: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_number_after_ofast_opt:[0-9]+]] number_after_ofast_opt: i32 [storage=static] [linkage=external];
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
// O2-NEXT:     global %[[VALUE_ofast_fast:[0-9]+]] ofast_fast: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_ofast_opt:[0-9]+]] ofast_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_ofast_no_errno:[0-9]+]] ofast_no_errno: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_ofast_recip:[0-9]+]] ofast_recip: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_ofast_assoc:[0-9]+]] ofast_assoc: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_ofast_no_zeros:[0-9]+]] ofast_no_zeros: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_dash_ofast_fast:[0-9]+]] dash_ofast_fast: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_dash_ofast_opt:[0-9]+]] dash_ofast_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_dash_ofast_no_errno:[0-9]+]] dash_ofast_no_errno: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_dash_ofast_recip:[0-9]+]] dash_ofast_recip: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_dash_ofast_assoc:[0-9]+]] dash_ofast_assoc: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_dash_ofast_no_zeros:[0-9]+]] dash_ofast_no_zeros: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_fast_math_fast:[0-9]+]] fast_math_fast: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_fast_math_opt:[0-9]+]] fast_math_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_fast_math_no_errno:[0-9]+]] fast_math_no_errno: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_fast_math_recip:[0-9]+]] fast_math_recip: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_fast_math_assoc:[0-9]+]] fast_math_assoc: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_fast_math_no_zeros:[0-9]+]] fast_math_no_zeros: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_ofast_then_o2_opt:[0-9]+]] ofast_then_o2_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_o1_opt:[0-9]+]] list_ofast_o1_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_o2_ofast_fast:[0-9]+]] list_o2_ofast_fast: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_o2_ofast_opt:[0-9]+]] list_o2_ofast_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_o2_ofast_no_errno:[0-9]+]] list_o2_ofast_no_errno: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_o2_ofast_recip:[0-9]+]] list_o2_ofast_recip: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_o2_ofast_assoc:[0-9]+]] list_o2_ofast_assoc: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_o2_ofast_no_zeros:[0-9]+]] list_o2_ofast_no_zeros: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_o2_ofast_o3_opt:[0-9]+]] list_o2_ofast_o3_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_ofast_fast:[0-9]+]] list_ofast_ofast_fast: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_ofast_opt:[0-9]+]] list_ofast_ofast_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_ofast_no_errno:[0-9]+]] list_ofast_ofast_no_errno: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_ofast_recip:[0-9]+]] list_ofast_ofast_recip: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_ofast_assoc:[0-9]+]] list_ofast_ofast_assoc: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_ofast_no_zeros:[0-9]+]] list_ofast_ofast_no_zeros: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_fast_math_o2_fast:[0-9]+]] list_fast_math_o2_fast: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_fast_math_o2_opt:[0-9]+]] list_fast_math_o2_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_fast_math_o2_no_errno:[0-9]+]] list_fast_math_o2_no_errno: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_fast_math_o2_recip:[0-9]+]] list_fast_math_o2_recip: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_fast_math_o2_assoc:[0-9]+]] list_fast_math_o2_assoc: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_fast_math_o2_no_zeros:[0-9]+]] list_fast_math_o2_no_zeros: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_no_fast_math_opt:[0-9]+]] list_ofast_no_fast_math_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_recip_fast:[0-9]+]] list_ofast_recip_fast: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_recip_opt:[0-9]+]] list_ofast_recip_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_recip_no_errno:[0-9]+]] list_ofast_recip_no_errno: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_recip_recip:[0-9]+]] list_ofast_recip_recip: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_recip_assoc:[0-9]+]] list_ofast_recip_assoc: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_ofast_recip_no_zeros:[0-9]+]] list_ofast_recip_no_zeros: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_separate_fast_then_errno_opt:[0-9]+]] separate_fast_then_errno_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_separate_fast_then_errno_no_errno:[0-9]+]] separate_fast_then_errno_no_errno: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_separate_recip_then_o2_opt:[0-9]+]] separate_recip_then_o2_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_separate_o2_then_recip_opt:[0-9]+]] separate_o2_then_recip_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_separate_o2_then_recip_recip:[0-9]+]] separate_o2_then_recip_recip: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_push_ofast_pop_opt:[0-9]+]] push_ofast_pop_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_push_ofast_pop_recip:[0-9]+]] push_ofast_pop_recip: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_ofast_reset_opt:[0-9]+]] ofast_reset_opt: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_number_after_ofast_opt:[0-9]+]] number_after_ofast_opt: i32 [storage=static] [linkage=external];
// O2-NEXT: }
// SLATE-FILECHECK-END O2
// SLATE-FILECHECK-BEGIN ASSOC
// ASSOC: module {
// ASSOC-NEXT:     target "x86_64-unknown-linux-gnu" {
// ASSOC-NEXT:         endian = little;
// ASSOC-NEXT:         pointer [size=8, align=8];
// ASSOC-NEXT:         stack_alignment = 16;
// ASSOC-NEXT:         long_double = f80;
// ASSOC-NEXT:         storage bool [size=1, align=1];
// ASSOC-NEXT:         storage i8, u8 [size=1, align=1];
// ASSOC-NEXT:         storage i16, u16 [size=2, align=2];
// ASSOC-NEXT:         storage i32, u32 [size=4, align=4];
// ASSOC-NEXT:         storage i64, u64 [size=8, align=8];
// ASSOC-NEXT:         storage i128, u128 [size=16, align=16];
// ASSOC-NEXT:         storage bf16 [size=2, align=2];
// ASSOC-NEXT:         storage f16 [size=2, align=2];
// ASSOC-NEXT:         storage f32 [size=4, align=4];
// ASSOC-NEXT:         storage f64 [size=8, align=8];
// ASSOC-NEXT:         storage f80 [size=16, align=16];
// ASSOC-NEXT:         storage f128 [size=16, align=16];
// ASSOC-NEXT:         storage d32 [size=4, align=4];
// ASSOC-NEXT:         storage d64 [size=8, align=8];
// ASSOC-NEXT:         storage d128 [size=16, align=16];
// ASSOC-NEXT:     }
// ASSOC-NEXT:     global %[[VALUE_ofast_fast:[0-9]+]] ofast_fast: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_ofast_opt:[0-9]+]] ofast_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_ofast_no_errno:[0-9]+]] ofast_no_errno: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_ofast_recip:[0-9]+]] ofast_recip: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_ofast_assoc:[0-9]+]] ofast_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_ofast_no_zeros:[0-9]+]] ofast_no_zeros: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_dash_ofast_fast:[0-9]+]] dash_ofast_fast: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_dash_ofast_opt:[0-9]+]] dash_ofast_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_dash_ofast_no_errno:[0-9]+]] dash_ofast_no_errno: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_dash_ofast_recip:[0-9]+]] dash_ofast_recip: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_dash_ofast_assoc:[0-9]+]] dash_ofast_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_dash_ofast_no_zeros:[0-9]+]] dash_ofast_no_zeros: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_fast_math_fast:[0-9]+]] fast_math_fast: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_fast_math_no_errno:[0-9]+]] fast_math_no_errno: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_fast_math_recip:[0-9]+]] fast_math_recip: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_fast_math_assoc:[0-9]+]] fast_math_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_fast_math_no_zeros:[0-9]+]] fast_math_no_zeros: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_ofast_then_o2_opt:[0-9]+]] ofast_then_o2_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_ofast_then_o2_assoc:[0-9]+]] ofast_then_o2_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_o1_opt:[0-9]+]] list_ofast_o1_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_o1_assoc:[0-9]+]] list_ofast_o1_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_o2_ofast_fast:[0-9]+]] list_o2_ofast_fast: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_o2_ofast_opt:[0-9]+]] list_o2_ofast_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_o2_ofast_no_errno:[0-9]+]] list_o2_ofast_no_errno: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_o2_ofast_recip:[0-9]+]] list_o2_ofast_recip: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_o2_ofast_assoc:[0-9]+]] list_o2_ofast_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_o2_ofast_no_zeros:[0-9]+]] list_o2_ofast_no_zeros: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_o2_ofast_o3_opt:[0-9]+]] list_o2_ofast_o3_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_o2_ofast_o3_assoc:[0-9]+]] list_o2_ofast_o3_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_ofast_fast:[0-9]+]] list_ofast_ofast_fast: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_ofast_opt:[0-9]+]] list_ofast_ofast_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_ofast_no_errno:[0-9]+]] list_ofast_ofast_no_errno: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_ofast_recip:[0-9]+]] list_ofast_ofast_recip: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_ofast_assoc:[0-9]+]] list_ofast_ofast_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_ofast_no_zeros:[0-9]+]] list_ofast_ofast_no_zeros: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_fast_math_o2_fast:[0-9]+]] list_fast_math_o2_fast: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_fast_math_o2_opt:[0-9]+]] list_fast_math_o2_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_fast_math_o2_no_errno:[0-9]+]] list_fast_math_o2_no_errno: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_fast_math_o2_recip:[0-9]+]] list_fast_math_o2_recip: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_fast_math_o2_assoc:[0-9]+]] list_fast_math_o2_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_fast_math_o2_no_zeros:[0-9]+]] list_fast_math_o2_no_zeros: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_no_fast_math_opt:[0-9]+]] list_ofast_no_fast_math_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_recip_fast:[0-9]+]] list_ofast_recip_fast: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_recip_opt:[0-9]+]] list_ofast_recip_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_recip_no_errno:[0-9]+]] list_ofast_recip_no_errno: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_recip_recip:[0-9]+]] list_ofast_recip_recip: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_recip_assoc:[0-9]+]] list_ofast_recip_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_list_ofast_recip_no_zeros:[0-9]+]] list_ofast_recip_no_zeros: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_separate_fast_then_errno_no_errno:[0-9]+]] separate_fast_then_errno_no_errno: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_separate_fast_then_errno_assoc:[0-9]+]] separate_fast_then_errno_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_separate_recip_then_o2_opt:[0-9]+]] separate_recip_then_o2_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_separate_recip_then_o2_assoc:[0-9]+]] separate_recip_then_o2_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_separate_o2_then_recip_recip:[0-9]+]] separate_o2_then_recip_recip: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_separate_o2_then_recip_assoc:[0-9]+]] separate_o2_then_recip_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_push_ofast_pop_recip:[0-9]+]] push_ofast_pop_recip: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_push_ofast_pop_assoc:[0-9]+]] push_ofast_pop_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_number_after_ofast_opt:[0-9]+]] number_after_ofast_opt: i32 [storage=static] [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_number_after_ofast_assoc:[0-9]+]] number_after_ofast_assoc: i32 [storage=static] [linkage=external];
// ASSOC-NEXT: }
// SLATE-FILECHECK-END ASSOC
// SLATE-FILECHECK-BEGIN NOTRAP
// NOTRAP: module {
// NOTRAP-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOTRAP-NEXT:         endian = little;
// NOTRAP-NEXT:         pointer [size=8, align=8];
// NOTRAP-NEXT:         stack_alignment = 16;
// NOTRAP-NEXT:         long_double = f80;
// NOTRAP-NEXT:         storage bool [size=1, align=1];
// NOTRAP-NEXT:         storage i8, u8 [size=1, align=1];
// NOTRAP-NEXT:         storage i16, u16 [size=2, align=2];
// NOTRAP-NEXT:         storage i32, u32 [size=4, align=4];
// NOTRAP-NEXT:         storage i64, u64 [size=8, align=8];
// NOTRAP-NEXT:         storage i128, u128 [size=16, align=16];
// NOTRAP-NEXT:         storage bf16 [size=2, align=2];
// NOTRAP-NEXT:         storage f16 [size=2, align=2];
// NOTRAP-NEXT:         storage f32 [size=4, align=4];
// NOTRAP-NEXT:         storage f64 [size=8, align=8];
// NOTRAP-NEXT:         storage f80 [size=16, align=16];
// NOTRAP-NEXT:         storage f128 [size=16, align=16];
// NOTRAP-NEXT:         storage d32 [size=4, align=4];
// NOTRAP-NEXT:         storage d64 [size=8, align=8];
// NOTRAP-NEXT:         storage d128 [size=16, align=16];
// NOTRAP-NEXT:     }
// NOTRAP-NEXT:     global %[[VALUE_ofast_fast:[0-9]+]] ofast_fast: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_opt:[0-9]+]] ofast_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_no_errno:[0-9]+]] ofast_no_errno: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_recip:[0-9]+]] ofast_recip: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_assoc:[0-9]+]] ofast_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_no_zeros:[0-9]+]] ofast_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_dash_ofast_fast:[0-9]+]] dash_ofast_fast: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_dash_ofast_opt:[0-9]+]] dash_ofast_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_dash_ofast_no_errno:[0-9]+]] dash_ofast_no_errno: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_dash_ofast_recip:[0-9]+]] dash_ofast_recip: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_dash_ofast_assoc:[0-9]+]] dash_ofast_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_dash_ofast_no_zeros:[0-9]+]] dash_ofast_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_fast_math_fast:[0-9]+]] fast_math_fast: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_fast_math_no_errno:[0-9]+]] fast_math_no_errno: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_fast_math_recip:[0-9]+]] fast_math_recip: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_fast_math_assoc:[0-9]+]] fast_math_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_fast_math_no_zeros:[0-9]+]] fast_math_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_then_o2_opt:[0-9]+]] ofast_then_o2_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_then_o2_assoc:[0-9]+]] ofast_then_o2_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_then_o2_no_zeros:[0-9]+]] ofast_then_o2_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_o1_opt:[0-9]+]] list_ofast_o1_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_o1_assoc:[0-9]+]] list_ofast_o1_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_o1_no_zeros:[0-9]+]] list_ofast_o1_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_o2_ofast_fast:[0-9]+]] list_o2_ofast_fast: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_o2_ofast_opt:[0-9]+]] list_o2_ofast_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_o2_ofast_no_errno:[0-9]+]] list_o2_ofast_no_errno: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_o2_ofast_recip:[0-9]+]] list_o2_ofast_recip: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_o2_ofast_assoc:[0-9]+]] list_o2_ofast_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_o2_ofast_no_zeros:[0-9]+]] list_o2_ofast_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_o2_ofast_o3_opt:[0-9]+]] list_o2_ofast_o3_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_o2_ofast_o3_assoc:[0-9]+]] list_o2_ofast_o3_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_o2_ofast_o3_no_zeros:[0-9]+]] list_o2_ofast_o3_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_ofast_fast:[0-9]+]] list_ofast_ofast_fast: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_ofast_opt:[0-9]+]] list_ofast_ofast_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_ofast_no_errno:[0-9]+]] list_ofast_ofast_no_errno: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_ofast_recip:[0-9]+]] list_ofast_ofast_recip: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_ofast_assoc:[0-9]+]] list_ofast_ofast_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_ofast_no_zeros:[0-9]+]] list_ofast_ofast_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_fast_math_o2_fast:[0-9]+]] list_fast_math_o2_fast: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_fast_math_o2_opt:[0-9]+]] list_fast_math_o2_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_fast_math_o2_no_errno:[0-9]+]] list_fast_math_o2_no_errno: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_fast_math_o2_recip:[0-9]+]] list_fast_math_o2_recip: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_fast_math_o2_assoc:[0-9]+]] list_fast_math_o2_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_fast_math_o2_no_zeros:[0-9]+]] list_fast_math_o2_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_no_fast_math_opt:[0-9]+]] list_ofast_no_fast_math_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_recip_fast:[0-9]+]] list_ofast_recip_fast: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_recip_opt:[0-9]+]] list_ofast_recip_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_recip_no_errno:[0-9]+]] list_ofast_recip_no_errno: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_recip_recip:[0-9]+]] list_ofast_recip_recip: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_recip_assoc:[0-9]+]] list_ofast_recip_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_list_ofast_recip_no_zeros:[0-9]+]] list_ofast_recip_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_separate_fast_then_errno_no_errno:[0-9]+]] separate_fast_then_errno_no_errno: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_separate_fast_then_errno_assoc:[0-9]+]] separate_fast_then_errno_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_separate_fast_then_errno_no_zeros:[0-9]+]] separate_fast_then_errno_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_separate_recip_then_o2_opt:[0-9]+]] separate_recip_then_o2_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_separate_recip_then_o2_assoc:[0-9]+]] separate_recip_then_o2_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_separate_recip_then_o2_no_zeros:[0-9]+]] separate_recip_then_o2_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_separate_o2_then_recip_recip:[0-9]+]] separate_o2_then_recip_recip: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_separate_o2_then_recip_assoc:[0-9]+]] separate_o2_then_recip_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_separate_o2_then_recip_no_zeros:[0-9]+]] separate_o2_then_recip_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_push_ofast_pop_recip:[0-9]+]] push_ofast_pop_recip: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_push_ofast_pop_assoc:[0-9]+]] push_ofast_pop_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_push_ofast_pop_no_zeros:[0-9]+]] push_ofast_pop_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_reset_assoc:[0-9]+]] ofast_reset_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_ofast_reset_no_zeros:[0-9]+]] ofast_reset_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_number_after_ofast_opt:[0-9]+]] number_after_ofast_opt: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_number_after_ofast_assoc:[0-9]+]] number_after_ofast_assoc: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT:     global %[[VALUE_number_after_ofast_no_zeros:[0-9]+]] number_after_ofast_no_zeros: i32 [storage=static] [linkage=external];
// NOTRAP-NEXT: }
// SLATE-FILECHECK-END NOTRAP
