/* Test __has_c_attribute.  Test supported attributes.  */
/* { dg-do preprocess } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#if __has_c_attribute ( nodiscard ) != 202311L
#error "bad result for nodiscard"
#endif

#if __has_c_attribute ( __nodiscard__ ) != 202311L
#error "bad result for __nodiscard__"
#endif

#if __has_c_attribute(maybe_unused) != 202311L
#error "bad result for maybe_unused"
#endif

#if __has_c_attribute(__maybe_unused__) != 202311L
#error "bad result for __maybe_unused__"
#endif

#if __has_c_attribute (deprecated) != 202311L
#error "bad result for deprecated"
#endif

#if __has_c_attribute (__deprecated__) != 202311L
#error "bad result for __deprecated__"
#endif

#if __has_c_attribute (fallthrough) != 202311L
#error "bad result for fallthrough"
#endif

#if __has_c_attribute (__fallthrough__) != 202311L
#error "bad result for __fallthrough__"
#endif

#if __has_c_attribute (noreturn) != 202311L
#error "bad result for noreturn"
#endif

#if __has_c_attribute (__noreturn__) != 202311L
#error "bad result for __noreturn__"
#endif

#if __has_c_attribute (_Noreturn) != 202311L
#error "bad result for _Noreturn"
#endif

#if __has_c_attribute (___Noreturn__) != 202311L
#error "bad result for ___Noreturn__"
#endif
  
#if __has_c_attribute (unsequenced) != 202311L
#error "bad result for unsequenced"
#endif

#if __has_c_attribute (__unsequenced__) != 202311L
#error "bad result for __unsequenced__"
#endif

#if __has_c_attribute (reproducible) != 202311L
#error "bad result for reproducible"
#endif

#if __has_c_attribute (__reproducible__) != 202311L
#error "bad result for __reproducible__"
#endif

/* Macros in the attribute name are expanded.  */
#define foo deprecated
#if __has_c_attribute (foo) != 202311L
#error "bad result for foo"
#endif
// SLATE-FILECHECK-STD DEFAULT c23
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
