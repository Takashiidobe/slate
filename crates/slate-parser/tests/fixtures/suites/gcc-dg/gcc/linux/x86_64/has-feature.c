/* { dg-do compile } */
/* { dg-options "" } */
/* Test __has_{feature,extension} for C language features.  */

#if !__has_extension (c_alignas) || !__has_extension (c_alignof)
#error
#endif

#if !__has_extension (c_atomic) || !__has_extension (c_generic_selections)
#error
#endif

#if !__has_extension (c_static_assert) || !__has_extension (c_thread_local)
#error
#endif

#if !__has_extension (cxx_binary_literals)
#error
#endif

#if  __STDC_VERSION__ >= 201112L
/* Have C11 features.  */
#if !__has_feature (c_alignas) || !__has_feature (c_alignof)
#error
#endif

#if !__has_feature (c_atomic) || !__has_feature (c_generic_selections)
#error
#endif

#if !__has_feature (c_static_assert) || !__has_feature (c_thread_local)
#error
#endif

#else
/* Don't have C11 features.  */
#if __has_feature (c_alignas) || __has_feature (c_alignof)
#error
#endif

#if __has_feature (c_atomic) || __has_feature (c_generic_selections)
#error
#endif

#if __has_feature (c_static_assert) || __has_feature (c_thread_local)
#error
#endif

#endif

#if __STDC_VERSION__ >= 202000L
/* Have C2x features.  */
#if !__has_feature (cxx_binary_literals)
#error
#endif

#else
/* Don't have C2x features.  */
#if __has_feature (cxx_binary_literals)
#error
#endif
#endif
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
