#ifdef __OPTIMIZE__
int start_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int start_optimize_size;
#endif
#ifdef __NO_INLINE__
int start_no_inline;
#endif

#pragma GCC optimize ("O2")
#ifdef __OPTIMIZE__
int o2_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int o2_optimize_size;
#endif
#ifdef __NO_INLINE__
int o2_no_inline;
#endif

#pragma GCC optimize ("Os")
#ifdef __OPTIMIZE__
int os_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int os_optimize_size;
#endif
#ifdef __NO_INLINE__
int os_no_inline;
#endif

#pragma GCC optimize ("O0")
#ifdef __OPTIMIZE__
int o0_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int o0_optimize_size;
#endif
#ifdef __NO_INLINE__
int o0_no_inline;
#endif

#pragma GCC optimize 3
#ifdef __OPTIMIZE__
int number_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int number_optimize_size;
#endif
#ifdef __NO_INLINE__
int number_no_inline;
#endif

#pragma GCC optimize ("s")
#ifdef __OPTIMIZE__
int size_name_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int size_name_optimize_size;
#endif
#ifdef __NO_INLINE__
int size_name_no_inline;
#endif

#pragma GCC optimize ("Os,O1")
#ifdef __OPTIMIZE__
int list_last_wins_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int list_last_wins_optimize_size;
#endif
#ifdef __NO_INLINE__
int list_last_wins_no_inline;
#endif

#pragma GCC optimize ("-O0")
#ifdef __OPTIMIZE__
int dash_o_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int dash_o_optimize_size;
#endif
#ifdef __NO_INLINE__
int dash_o_no_inline;
#endif

#pragma GCC push_options
#pragma GCC optimize ("Oz")
#ifdef __OPTIMIZE__
int push_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int push_optimize_size;
#endif
#ifdef __NO_INLINE__
int push_no_inline;
#endif

#ifdef __OPTIMIZE__
int in_push_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int in_push_optimize_size;
#endif
#ifdef __NO_INLINE__
int in_push_no_inline;
#endif

#pragma GCC pop_options
#ifdef __OPTIMIZE__
int pop_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int pop_optimize_size;
#endif
#ifdef __NO_INLINE__
int pop_no_inline;
#endif

#pragma GCC optimize ("Os")
#pragma GCC reset_options
#ifdef __OPTIMIZE__
int reset_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int reset_optimize_size;
#endif
#ifdef __NO_INLINE__
int reset_no_inline;
#endif

#pragma GCC optimize ("no-math-errno")
#ifdef __OPTIMIZE__
int math_only_optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int math_only_optimize_size;
#endif
#ifdef __NO_INLINE__
int math_only_no_inline;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES O0
// SLATE-FILECHECK-PREFIX-ARGS O0 -O0
// SLATE-FILECHECK-DEFINES O2
// SLATE-FILECHECK-PREFIX-ARGS O2 -O2
// SLATE-FILECHECK-DEFINES OS
// SLATE-FILECHECK-PREFIX-ARGS OS -Os

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
// O0-NEXT:     global %[[VALUE_start_no_inline:[0-9]+]] start_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_o2_optimize:[0-9]+]] o2_optimize: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_o2_no_inline:[0-9]+]] o2_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_os_optimize:[0-9]+]] os_optimize: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_os_optimize_size:[0-9]+]] os_optimize_size: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_os_no_inline:[0-9]+]] os_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_o0_no_inline:[0-9]+]] o0_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_number_optimize:[0-9]+]] number_optimize: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_number_no_inline:[0-9]+]] number_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_size_name_optimize:[0-9]+]] size_name_optimize: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_size_name_optimize_size:[0-9]+]] size_name_optimize_size: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_size_name_no_inline:[0-9]+]] size_name_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_last_wins_optimize:[0-9]+]] list_last_wins_optimize: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_list_last_wins_no_inline:[0-9]+]] list_last_wins_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_dash_o_no_inline:[0-9]+]] dash_o_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_push_optimize:[0-9]+]] push_optimize: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_push_optimize_size:[0-9]+]] push_optimize_size: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_push_no_inline:[0-9]+]] push_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_in_push_optimize:[0-9]+]] in_push_optimize: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_in_push_optimize_size:[0-9]+]] in_push_optimize_size: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_in_push_no_inline:[0-9]+]] in_push_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_pop_no_inline:[0-9]+]] pop_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_reset_no_inline:[0-9]+]] reset_no_inline: i32 [storage=static] [linkage=external];
// O0-NEXT:     global %[[VALUE_math_only_no_inline:[0-9]+]] math_only_no_inline: i32 [storage=static] [linkage=external];
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
// O2-NEXT:     global %[[VALUE_start_optimize:[0-9]+]] start_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_o2_optimize:[0-9]+]] o2_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_os_optimize:[0-9]+]] os_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_os_optimize_size:[0-9]+]] os_optimize_size: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_number_optimize:[0-9]+]] number_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_size_name_optimize:[0-9]+]] size_name_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_size_name_optimize_size:[0-9]+]] size_name_optimize_size: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_list_last_wins_optimize:[0-9]+]] list_last_wins_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_push_optimize:[0-9]+]] push_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_push_optimize_size:[0-9]+]] push_optimize_size: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_in_push_optimize:[0-9]+]] in_push_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_in_push_optimize_size:[0-9]+]] in_push_optimize_size: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_reset_optimize:[0-9]+]] reset_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT:     global %[[VALUE_math_only_optimize:[0-9]+]] math_only_optimize: i32 [storage=static] [linkage=external];
// O2-NEXT: }
// SLATE-FILECHECK-END O2
// SLATE-FILECHECK-BEGIN OS
// OS: module {
// OS-NEXT:     target "x86_64-unknown-linux-gnu" {
// OS-NEXT:         endian = little;
// OS-NEXT:         pointer [size=8, align=8];
// OS-NEXT:         stack_alignment = 16;
// OS-NEXT:         long_double = f80;
// OS-NEXT:         storage bool [size=1, align=1];
// OS-NEXT:         storage i8, u8 [size=1, align=1];
// OS-NEXT:         storage i16, u16 [size=2, align=2];
// OS-NEXT:         storage i32, u32 [size=4, align=4];
// OS-NEXT:         storage i64, u64 [size=8, align=8];
// OS-NEXT:         storage i128, u128 [size=16, align=16];
// OS-NEXT:         storage bf16 [size=2, align=2];
// OS-NEXT:         storage f16 [size=2, align=2];
// OS-NEXT:         storage f32 [size=4, align=4];
// OS-NEXT:         storage f64 [size=8, align=8];
// OS-NEXT:         storage f80 [size=16, align=16];
// OS-NEXT:         storage f128 [size=16, align=16];
// OS-NEXT:         storage d32 [size=4, align=4];
// OS-NEXT:         storage d64 [size=8, align=8];
// OS-NEXT:         storage d128 [size=16, align=16];
// OS-NEXT:     }
// OS-NEXT:     global %[[VALUE_start_optimize:[0-9]+]] start_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_start_optimize_size:[0-9]+]] start_optimize_size: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_o2_optimize:[0-9]+]] o2_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_os_optimize:[0-9]+]] os_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_os_optimize_size:[0-9]+]] os_optimize_size: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_number_optimize:[0-9]+]] number_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_size_name_optimize:[0-9]+]] size_name_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_size_name_optimize_size:[0-9]+]] size_name_optimize_size: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_list_last_wins_optimize:[0-9]+]] list_last_wins_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_push_optimize:[0-9]+]] push_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_push_optimize_size:[0-9]+]] push_optimize_size: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_in_push_optimize:[0-9]+]] in_push_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_in_push_optimize_size:[0-9]+]] in_push_optimize_size: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_reset_optimize:[0-9]+]] reset_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_reset_optimize_size:[0-9]+]] reset_optimize_size: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_math_only_optimize:[0-9]+]] math_only_optimize: i32 [storage=static] [linkage=external];
// OS-NEXT:     global %[[VALUE_math_only_optimize_size:[0-9]+]] math_only_optimize_size: i32 [storage=static] [linkage=external];
// OS-NEXT: }
// SLATE-FILECHECK-END OS
