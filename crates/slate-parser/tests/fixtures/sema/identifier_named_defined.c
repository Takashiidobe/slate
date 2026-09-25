// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

#define PRESENT 1
#define ID(x) x

#if defined(ABSENT)
#error "defined(ABSENT) must be false"
#elif defined PRESENT && defined(PRESENT) && !defined(ABSENT)
int defined;
#else
#error "#elif must evaluate defined"
#endif

typeof(defined) typed;

int read(void) {
    int copy = defined;
    return ID(defined) + copy;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %0 defined: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %1 typed: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %2 @read() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %3 copy: i32 [storage=automatic] = read<i32>(%0);
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%0), read<i32>(%3));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
