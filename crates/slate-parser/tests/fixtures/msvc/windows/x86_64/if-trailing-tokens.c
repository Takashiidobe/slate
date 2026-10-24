// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#if UNDEF(1)
int a_true;
#else
int a_false;
#endif
#if UNDEF(1) || 1
int b_true;
#else
int b_false;
#endif
#if !UNDEF(1)
int c_true;
#else
int c_false;
#endif
#if 1 2
int d_true;
#else
int d_false;
#endif
#if 0 2
int e_true;
#else
int e_false;
#endif
#if 1 || UNDEF(1)
int f_true;
#else
int f_false;
#endif
#if 0 || UNDEF(1)
int g_true;
#else
int g_false;
#endif
#if 1 ? 2 : UNDEF(3)
int h_true;
#else
int h_false;
#endif
#if 1 + 1 UNDEF + 5
int i_true;
#else
int i_false;
#endif
#if 0 ,1
int k_true;
#else
int k_false;
#endif
#if 1 = 0
int l_true;
#else
int l_false;
#endif
#if X[1] || X.y || X++
int m_true;
#else
int m_false;
#endif
#if 1 1)
int n_true;
#else
int n_false;
#endif
#if __has_builtin(__builtin_expect)
int o_true;
#else
int o_false;
#endif
#if 0
#elif UNDEF(1)
int p_true;
#else
int p_false;
#endif

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_a_false:[0-9]+]] a_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b_false:[0-9]+]] b_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c_true:[0-9]+]] c_true: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d_true:[0-9]+]] d_true: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e_false:[0-9]+]] e_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f_true:[0-9]+]] f_true: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g_false:[0-9]+]] g_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h_true:[0-9]+]] h_true: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i_true:[0-9]+]] i_true: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k_false:[0-9]+]] k_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l_true:[0-9]+]] l_true: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m_false:[0-9]+]] m_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n_true:[0-9]+]] n_true: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o_false:[0-9]+]] o_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p_false:[0-9]+]] p_false: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
