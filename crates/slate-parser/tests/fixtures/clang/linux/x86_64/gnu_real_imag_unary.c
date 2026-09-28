double real_double_under(_Complex double c) { return __real__ c; }

double imag_double_under(_Complex double c) { return __imag__ c; }

double real_single_under(_Complex double c) { return __real c; }

double imag_single_under(_Complex double c) { return __imag c; }

int real_array_size[__real__ 5];

int imag_array_size[__imag__ 5];




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
// DEFAULT-NEXT:     global %8 real_array_size: array<i32, 5> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %9 imag_array_size: array<i32, 0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @real_double_under(%1 c: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<f64>(real(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @imag_double_under(%3 c: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<f64>(imag(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @real_single_under(%5 c: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<f64>(real(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @imag_single_under(%7 c: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<f64>(imag(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
