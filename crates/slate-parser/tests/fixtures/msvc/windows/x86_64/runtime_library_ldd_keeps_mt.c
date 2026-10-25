// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS /MT /LDd
#if !defined(_DEBUG) \
    || defined(_DLL) \
    || defined(_MSVC_MD) \
    || !defined(_MSVC_MT) \
    || defined(__MSVC_RUNTIME_CHECKS) \
    || defined(_RTC_CONVERSION_CHECKS_ENABLED) \
    || defined(_ISO_VOLATILE) \
    || defined(_GUARDOVERFLOW_CRT_ALLOCATORS) \
    || defined(_KERNEL_MODE) \
    || defined(__SANITIZE_ADDRESS__) \
    || defined(_CONTROL_FLOW_GUARD) \
    || defined(_VC_NODEFAULTLIB) \
    || defined(__ATOM__) \
    || defined(_NO_PREFETCHW) \
    || defined(__FPCVT_IA__)
#error "wrong option predefines"
#endif
int option_macros;

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
// DEFAULT-NEXT:     global %[[VALUE_option_macros:[0-9]+]] option_macros: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
