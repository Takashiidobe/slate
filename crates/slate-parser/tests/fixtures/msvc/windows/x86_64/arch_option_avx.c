// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS /arch:AVX
#if !defined(__AVX__) \
    || defined(__AVX2__) \
    || defined(__AVX512F__) \
    || defined(__AVX512CD__) \
    || defined(__AVX512BW__) \
    || defined(__AVX512DQ__) \
    || defined(__AVX512VL__) \
    || defined(__APX_F__) \
    || defined(__EGPR__) \
    || defined(__PUSH2POP2__) \
    || defined(__PPX__) \
    || defined(__NDD__) \
    || defined(__CCMP__) \
    || defined(__NF__) \
    || defined(__CF__) \
    || defined(__ZU__) \
    || !defined(_NO_PREFETCHW) \
    || defined(__AVX10_VER__)
#error "wrong /arch predefines"
#endif
int arch_option;

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
// DEFAULT-NEXT:     global %[[VALUE_arch_option:[0-9]+]] arch_option: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
