// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS /D_MSC_VER=5 /u /Za
#if !defined(_MSVC_WARNING_LEVEL) \
    || !defined(__STDC_HOSTED__) \
    || !defined(_INTEGRAL_MAX_BITS) \
    || !defined(_MSVC_EXECUTION_CHARACTER_SET) \
    || !defined(_IS_ASSIGNABLE_NOCHECK_SUPPORTED) \
    || !defined(_MSVC_TRADITIONAL) \
    || !defined(__STDC__) \
    || !defined(_MSVC_MT) \
    || defined(_MSC_EXTENSIONS) \
    || defined(_WIN32) \
    || defined(_M_X64) \
    || defined(_MT) \
    || _MSC_VER != 5
#error "wrong /u predefines"
#endif
int undefine_predefined;

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
// DEFAULT-NEXT:     global %[[VALUE_undefine_predefined:[0-9]+]] undefine_predefined: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
