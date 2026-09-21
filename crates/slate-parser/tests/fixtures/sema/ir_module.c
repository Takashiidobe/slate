// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

static short narrow(void) {
    return 65537;
}

double widen(void) {
    return 2;
}

int arithmetic(void) {
    1 + 2;
    {
        return 3 * 4;
    }
}

void empty(void) {
    return;
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
// IR-NEXT:     fn %0 @narrow() -> i16 [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return truncate<i16, reason=return, fits=unknown>(const<i32>(65537));
// IR-NEXT:     }
// IR-NEXT:     fn %1 @widen() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return int_to_float<f64, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn %2 @arithmetic() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         add<i32, overflow=ub>(const<i32>(1), const<i32>(2));
// IR-NEXT:         {
// IR-NEXT:             return mul<i32, overflow=ub>(const<i32>(3), const<i32>(4));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %3 @empty() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         return;
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
