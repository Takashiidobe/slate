// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int add(void) {
    return 1 + 2;
}

long promoted(void) {
    return (short)1 + 2;
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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %0 @add() -> i32 [linkage=external] {
// IR-NEXT:         return add<i32, overflow=undefined>(const<i32>(1), const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn %1 @promoted() -> i64 [linkage=external] {
// IR-NEXT:         return widen<i64, reason=return>(add<i32, overflow=undefined>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(1))), const<i32>(2)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
