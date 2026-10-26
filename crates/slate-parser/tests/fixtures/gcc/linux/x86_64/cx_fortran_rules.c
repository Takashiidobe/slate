// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES FORTRAN
// SLATE-FILECHECK-PREFIX-ARGS FORTRAN -fcx-fortran-rules
// SLATE-FILECHECK-DEFINES LIMITED_LAST
// SLATE-FILECHECK-PREFIX-ARGS LIMITED_LAST -fcx-fortran-rules -fcx-limited-range
// SLATE-FILECHECK-DEFINES FORTRAN_LAST
// SLATE-FILECHECK-PREFIX-ARGS FORTRAN_LAST -fcx-limited-range -fcx-fortran-rules
// SLATE-FILECHECK-DEFINES NEGATED
// SLATE-FILECHECK-PREFIX-ARGS NEGATED -fcx-limited-range -fno-cx-fortran-rules
// SLATE-FILECHECK-DEFINES FAST_MATH
// SLATE-FILECHECK-PREFIX-ARGS FAST_MATH -fno-cx-limited-range -ffast-math
// SLATE-FILECHECK-DEFINES FAST_MATH_OFF
// SLATE-FILECHECK-PREFIX-ARGS FAST_MATH_OFF -ffast-math -fno-fast-math
// SLATE-FILECHECK-DEFINES EXPLICIT_KEPT
// SLATE-FILECHECK-PREFIX-ARGS EXPLICIT_KEPT -fcx-fortran-rules -fno-fast-math

#ifdef __STDC_IEC_559_COMPLEX__
int iec_559_complex = __STDC_IEC_559_COMPLEX__;
#else
int iec_559_complex = -1;
#endif
int gcc_iec_559_complex = __GCC_IEC_559_COMPLEX;

double _Complex divide(double _Complex a, double _Complex b) { return a / b; }

double _Complex multiply(double _Complex a, double _Complex b) { return a * b; }

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
// DEFAULT-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gcc_iec_559_complex:[0-9]+]] gcc_iec_559_complex: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FORTRAN
// FORTRAN: module {
// FORTRAN-NEXT:     target "x86_64-unknown-linux-gnu" {
// FORTRAN-NEXT:         endian = little;
// FORTRAN-NEXT:         pointer [size=8, align=8];
// FORTRAN-NEXT:         stack_alignment = 16;
// FORTRAN-NEXT:         long_double = f80;
// FORTRAN-NEXT:         storage bool [size=1, align=1];
// FORTRAN-NEXT:         storage i8, u8 [size=1, align=1];
// FORTRAN-NEXT:         storage i16, u16 [size=2, align=2];
// FORTRAN-NEXT:         storage i32, u32 [size=4, align=4];
// FORTRAN-NEXT:         storage i64, u64 [size=8, align=8];
// FORTRAN-NEXT:         storage i128, u128 [size=16, align=16];
// FORTRAN-NEXT:         storage bf16 [size=2, align=2];
// FORTRAN-NEXT:         storage f16 [size=2, align=2];
// FORTRAN-NEXT:         storage f32 [size=4, align=4];
// FORTRAN-NEXT:         storage f64 [size=8, align=8];
// FORTRAN-NEXT:         storage f80 [size=16, align=16];
// FORTRAN-NEXT:         storage f128 [size=16, align=16];
// FORTRAN-NEXT:         storage d32 [size=4, align=4];
// FORTRAN-NEXT:         storage d64 [size=8, align=8];
// FORTRAN-NEXT:         storage d128 [size=16, align=16];
// FORTRAN-NEXT:     }
// FORTRAN-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// FORTRAN-NEXT:     global %[[VALUE_gcc_iec_559_complex:[0-9]+]] gcc_iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// FORTRAN-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FORTRAN-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=improved>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// FORTRAN-NEXT:     }
// FORTRAN-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FORTRAN-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=improved>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// FORTRAN-NEXT:     }
// FORTRAN-NEXT: }
// SLATE-FILECHECK-END FORTRAN
// SLATE-FILECHECK-BEGIN LIMITED_LAST
// LIMITED_LAST: module {
// LIMITED_LAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// LIMITED_LAST-NEXT:         endian = little;
// LIMITED_LAST-NEXT:         pointer [size=8, align=8];
// LIMITED_LAST-NEXT:         stack_alignment = 16;
// LIMITED_LAST-NEXT:         long_double = f80;
// LIMITED_LAST-NEXT:         storage bool [size=1, align=1];
// LIMITED_LAST-NEXT:         storage i8, u8 [size=1, align=1];
// LIMITED_LAST-NEXT:         storage i16, u16 [size=2, align=2];
// LIMITED_LAST-NEXT:         storage i32, u32 [size=4, align=4];
// LIMITED_LAST-NEXT:         storage i64, u64 [size=8, align=8];
// LIMITED_LAST-NEXT:         storage i128, u128 [size=16, align=16];
// LIMITED_LAST-NEXT:         storage bf16 [size=2, align=2];
// LIMITED_LAST-NEXT:         storage f16 [size=2, align=2];
// LIMITED_LAST-NEXT:         storage f32 [size=4, align=4];
// LIMITED_LAST-NEXT:         storage f64 [size=8, align=8];
// LIMITED_LAST-NEXT:         storage f80 [size=16, align=16];
// LIMITED_LAST-NEXT:         storage f128 [size=16, align=16];
// LIMITED_LAST-NEXT:         storage d32 [size=4, align=4];
// LIMITED_LAST-NEXT:         storage d64 [size=8, align=8];
// LIMITED_LAST-NEXT:         storage d128 [size=16, align=16];
// LIMITED_LAST-NEXT:     }
// LIMITED_LAST-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// LIMITED_LAST-NEXT:     global %[[VALUE_gcc_iec_559_complex:[0-9]+]] gcc_iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// LIMITED_LAST-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// LIMITED_LAST-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=basic>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// LIMITED_LAST-NEXT:     }
// LIMITED_LAST-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// LIMITED_LAST-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=basic>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// LIMITED_LAST-NEXT:     }
// LIMITED_LAST-NEXT: }
// SLATE-FILECHECK-END LIMITED_LAST
// SLATE-FILECHECK-BEGIN FORTRAN_LAST
// FORTRAN_LAST: module {
// FORTRAN_LAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// FORTRAN_LAST-NEXT:         endian = little;
// FORTRAN_LAST-NEXT:         pointer [size=8, align=8];
// FORTRAN_LAST-NEXT:         stack_alignment = 16;
// FORTRAN_LAST-NEXT:         long_double = f80;
// FORTRAN_LAST-NEXT:         storage bool [size=1, align=1];
// FORTRAN_LAST-NEXT:         storage i8, u8 [size=1, align=1];
// FORTRAN_LAST-NEXT:         storage i16, u16 [size=2, align=2];
// FORTRAN_LAST-NEXT:         storage i32, u32 [size=4, align=4];
// FORTRAN_LAST-NEXT:         storage i64, u64 [size=8, align=8];
// FORTRAN_LAST-NEXT:         storage i128, u128 [size=16, align=16];
// FORTRAN_LAST-NEXT:         storage bf16 [size=2, align=2];
// FORTRAN_LAST-NEXT:         storage f16 [size=2, align=2];
// FORTRAN_LAST-NEXT:         storage f32 [size=4, align=4];
// FORTRAN_LAST-NEXT:         storage f64 [size=8, align=8];
// FORTRAN_LAST-NEXT:         storage f80 [size=16, align=16];
// FORTRAN_LAST-NEXT:         storage f128 [size=16, align=16];
// FORTRAN_LAST-NEXT:         storage d32 [size=4, align=4];
// FORTRAN_LAST-NEXT:         storage d64 [size=8, align=8];
// FORTRAN_LAST-NEXT:         storage d128 [size=16, align=16];
// FORTRAN_LAST-NEXT:     }
// FORTRAN_LAST-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// FORTRAN_LAST-NEXT:     global %[[VALUE_gcc_iec_559_complex:[0-9]+]] gcc_iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// FORTRAN_LAST-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FORTRAN_LAST-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=improved>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// FORTRAN_LAST-NEXT:     }
// FORTRAN_LAST-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FORTRAN_LAST-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=improved>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// FORTRAN_LAST-NEXT:     }
// FORTRAN_LAST-NEXT: }
// SLATE-FILECHECK-END FORTRAN_LAST
// SLATE-FILECHECK-BEGIN NEGATED
// NEGATED: module {
// NEGATED-NEXT:     target "x86_64-unknown-linux-gnu" {
// NEGATED-NEXT:         endian = little;
// NEGATED-NEXT:         pointer [size=8, align=8];
// NEGATED-NEXT:         stack_alignment = 16;
// NEGATED-NEXT:         long_double = f80;
// NEGATED-NEXT:         storage bool [size=1, align=1];
// NEGATED-NEXT:         storage i8, u8 [size=1, align=1];
// NEGATED-NEXT:         storage i16, u16 [size=2, align=2];
// NEGATED-NEXT:         storage i32, u32 [size=4, align=4];
// NEGATED-NEXT:         storage i64, u64 [size=8, align=8];
// NEGATED-NEXT:         storage i128, u128 [size=16, align=16];
// NEGATED-NEXT:         storage bf16 [size=2, align=2];
// NEGATED-NEXT:         storage f16 [size=2, align=2];
// NEGATED-NEXT:         storage f32 [size=4, align=4];
// NEGATED-NEXT:         storage f64 [size=8, align=8];
// NEGATED-NEXT:         storage f80 [size=16, align=16];
// NEGATED-NEXT:         storage f128 [size=16, align=16];
// NEGATED-NEXT:         storage d32 [size=4, align=4];
// NEGATED-NEXT:         storage d64 [size=8, align=8];
// NEGATED-NEXT:         storage d128 [size=16, align=16];
// NEGATED-NEXT:     }
// NEGATED-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(1) [linkage=external];
// NEGATED-NEXT:     global %[[VALUE_gcc_iec_559_complex:[0-9]+]] gcc_iec_559_complex: i32 [storage=static] = const<i32>(2) [linkage=external];
// NEGATED-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// NEGATED-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// NEGATED-NEXT:     }
// NEGATED-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// NEGATED-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// NEGATED-NEXT:     }
// NEGATED-NEXT: }
// SLATE-FILECHECK-END NEGATED
// SLATE-FILECHECK-BEGIN FAST_MATH
// FAST_MATH: module {
// FAST_MATH-NEXT:     target "x86_64-unknown-linux-gnu" {
// FAST_MATH-NEXT:         endian = little;
// FAST_MATH-NEXT:         pointer [size=8, align=8];
// FAST_MATH-NEXT:         stack_alignment = 16;
// FAST_MATH-NEXT:         long_double = f80;
// FAST_MATH-NEXT:         storage bool [size=1, align=1];
// FAST_MATH-NEXT:         storage i8, u8 [size=1, align=1];
// FAST_MATH-NEXT:         storage i16, u16 [size=2, align=2];
// FAST_MATH-NEXT:         storage i32, u32 [size=4, align=4];
// FAST_MATH-NEXT:         storage i64, u64 [size=8, align=8];
// FAST_MATH-NEXT:         storage i128, u128 [size=16, align=16];
// FAST_MATH-NEXT:         storage bf16 [size=2, align=2];
// FAST_MATH-NEXT:         storage f16 [size=2, align=2];
// FAST_MATH-NEXT:         storage f32 [size=4, align=4];
// FAST_MATH-NEXT:         storage f64 [size=8, align=8];
// FAST_MATH-NEXT:         storage f80 [size=16, align=16];
// FAST_MATH-NEXT:         storage f128 [size=16, align=16];
// FAST_MATH-NEXT:         storage d32 [size=4, align=4];
// FAST_MATH-NEXT:         storage d64 [size=8, align=8];
// FAST_MATH-NEXT:         storage d128 [size=16, align=16];
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// FAST_MATH-NEXT:     global %[[VALUE_gcc_iec_559_complex:[0-9]+]] gcc_iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// FAST_MATH-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST_MATH-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST_MATH-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT: }
// SLATE-FILECHECK-END FAST_MATH
// SLATE-FILECHECK-BEGIN FAST_MATH_OFF
// FAST_MATH_OFF: module {
// FAST_MATH_OFF-NEXT:     target "x86_64-unknown-linux-gnu" {
// FAST_MATH_OFF-NEXT:         endian = little;
// FAST_MATH_OFF-NEXT:         pointer [size=8, align=8];
// FAST_MATH_OFF-NEXT:         stack_alignment = 16;
// FAST_MATH_OFF-NEXT:         long_double = f80;
// FAST_MATH_OFF-NEXT:         storage bool [size=1, align=1];
// FAST_MATH_OFF-NEXT:         storage i8, u8 [size=1, align=1];
// FAST_MATH_OFF-NEXT:         storage i16, u16 [size=2, align=2];
// FAST_MATH_OFF-NEXT:         storage i32, u32 [size=4, align=4];
// FAST_MATH_OFF-NEXT:         storage i64, u64 [size=8, align=8];
// FAST_MATH_OFF-NEXT:         storage i128, u128 [size=16, align=16];
// FAST_MATH_OFF-NEXT:         storage bf16 [size=2, align=2];
// FAST_MATH_OFF-NEXT:         storage f16 [size=2, align=2];
// FAST_MATH_OFF-NEXT:         storage f32 [size=4, align=4];
// FAST_MATH_OFF-NEXT:         storage f64 [size=8, align=8];
// FAST_MATH_OFF-NEXT:         storage f80 [size=16, align=16];
// FAST_MATH_OFF-NEXT:         storage f128 [size=16, align=16];
// FAST_MATH_OFF-NEXT:         storage d32 [size=4, align=4];
// FAST_MATH_OFF-NEXT:         storage d64 [size=8, align=8];
// FAST_MATH_OFF-NEXT:         storage d128 [size=16, align=16];
// FAST_MATH_OFF-NEXT:     }
// FAST_MATH_OFF-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST_MATH_OFF-NEXT:     global %[[VALUE_gcc_iec_559_complex:[0-9]+]] gcc_iec_559_complex: i32 [storage=static] = const<i32>(2) [linkage=external];
// FAST_MATH_OFF-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST_MATH_OFF-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// FAST_MATH_OFF-NEXT:     }
// FAST_MATH_OFF-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST_MATH_OFF-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// FAST_MATH_OFF-NEXT:     }
// FAST_MATH_OFF-NEXT: }
// SLATE-FILECHECK-END FAST_MATH_OFF
// SLATE-FILECHECK-BEGIN EXPLICIT_KEPT
// EXPLICIT_KEPT: module {
// EXPLICIT_KEPT-NEXT:     target "x86_64-unknown-linux-gnu" {
// EXPLICIT_KEPT-NEXT:         endian = little;
// EXPLICIT_KEPT-NEXT:         pointer [size=8, align=8];
// EXPLICIT_KEPT-NEXT:         stack_alignment = 16;
// EXPLICIT_KEPT-NEXT:         long_double = f80;
// EXPLICIT_KEPT-NEXT:         storage bool [size=1, align=1];
// EXPLICIT_KEPT-NEXT:         storage i8, u8 [size=1, align=1];
// EXPLICIT_KEPT-NEXT:         storage i16, u16 [size=2, align=2];
// EXPLICIT_KEPT-NEXT:         storage i32, u32 [size=4, align=4];
// EXPLICIT_KEPT-NEXT:         storage i64, u64 [size=8, align=8];
// EXPLICIT_KEPT-NEXT:         storage i128, u128 [size=16, align=16];
// EXPLICIT_KEPT-NEXT:         storage bf16 [size=2, align=2];
// EXPLICIT_KEPT-NEXT:         storage f16 [size=2, align=2];
// EXPLICIT_KEPT-NEXT:         storage f32 [size=4, align=4];
// EXPLICIT_KEPT-NEXT:         storage f64 [size=8, align=8];
// EXPLICIT_KEPT-NEXT:         storage f80 [size=16, align=16];
// EXPLICIT_KEPT-NEXT:         storage f128 [size=16, align=16];
// EXPLICIT_KEPT-NEXT:         storage d32 [size=4, align=4];
// EXPLICIT_KEPT-NEXT:         storage d64 [size=8, align=8];
// EXPLICIT_KEPT-NEXT:         storage d128 [size=16, align=16];
// EXPLICIT_KEPT-NEXT:     }
// EXPLICIT_KEPT-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// EXPLICIT_KEPT-NEXT:     global %[[VALUE_gcc_iec_559_complex:[0-9]+]] gcc_iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// EXPLICIT_KEPT-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// EXPLICIT_KEPT-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=improved>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// EXPLICIT_KEPT-NEXT:     }
// EXPLICIT_KEPT-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// EXPLICIT_KEPT-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=improved>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// EXPLICIT_KEPT-NEXT:     }
// EXPLICIT_KEPT-NEXT: }
// SLATE-FILECHECK-END EXPLICIT_KEPT
