// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

double _Complex add(double _Complex a, double _Complex b) {
    return a + b;
}

_Complex __bf16 bfloat_complex;

double _Complex mixed(double _Complex a, double b) {
    return a * b;
}

double _Complex add_real(double _Complex a, double b) {
    return a + b;
}

float _Complex precision(float _Complex a, double _Complex b) {
    return a + b;
}

int equal(double _Complex a, double _Complex b) {
    return a == b;
}

int equal_real(double _Complex a, double b) {
    return a == b;
}

double real_part(double _Complex a) {
    return __real__ a;
}

double imag_part(double _Complex a) {
    return __imag__ a;
}

double result_real(double _Complex a, double _Complex b) {
    return __real__ (a + b);
}

void set_parts(double _Complex *value, double x) {
    __real__ *value = x;
    __imag__ *value = x;
}

double _Complex from_real(double a) {
    return a;
}

double _Complex from_integer(int a) {
    return a;
}

double to_real(double _Complex a) {
    return (double)a;
}

int _Complex integer_add(int _Complex a, int _Complex b) {
    return a + b;
}

int _Complex integer_div(int _Complex a, int _Complex b) {
    return a / b;
}

double _Complex integer_to_float(int _Complex a) {
    return (double _Complex)a;
}

int _Complex float_to_integer(double _Complex a) {
    return (int _Complex)a;
}

double truth(double _Complex a) {
    return a ? 1.0 : 2.0;
}

unsigned long complex_size(void) {
    return sizeof(double _Complex);
}

unsigned long wide_complex_size(void) {
    return sizeof(long double _Complex);
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
// IR-NEXT:     global %3 bfloat_complex: complex<bf16> [storage=static] [linkage=external];
// IR-NEXT:     fn %0 @add(%1 a: complex<f64>, %2 b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>, coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%1), read<complex<f64>>(%2));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @mixed(%5 a: complex<f64>, %6 b: f64) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>, scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%5), read<f64>(%6));
// IR-NEXT:     }
// IR-NEXT:     fn %7 @add_real(%8 a: complex<f64>, %9 b: f64) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>, scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%8), read<f64>(%9));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @precision(%11 a: complex<f32>, %12 b: complex<f64>) -> complex<f32> [linkage=external] [abi=sysv64(coerce<pair<f32>>, coerce<f64, f64>) -> coerce<pair<f32>>] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_convert<complex<f32>, reason=return, rounding=nearest_even, exceptions=ignore>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%11)), read<complex<f64>>(%12)));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @equal(%14 a: complex<f64>, %15 b: complex<f64>) -> i32 [linkage=external] [abi=sysv64(coerce<f64, f64>, coerce<f64, f64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<complex<f64>, exceptions=ignore>(read<complex<f64>>(%14), read<complex<f64>>(%15)));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @equal_real(%17 a: complex<f64>, %18 b: f64) -> i32 [linkage=external] [abi=sysv64(coerce<f64, f64>, scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<complex<f64>, exceptions=ignore>(read<complex<f64>>(%17), read<f64>(%18)));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @real_part(%20 a: complex<f64>) -> f64 [linkage=external] [abi=sysv64(coerce<f64, f64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<f64>(real(%20));
// IR-NEXT:     }
// IR-NEXT:     fn %21 @imag_part(%22 a: complex<f64>) -> f64 [linkage=external] [abi=sysv64(coerce<f64, f64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<f64>(imag(%22));
// IR-NEXT:     }
// IR-NEXT:     fn %23 @result_real(%24 a: complex<f64>, %25 b: complex<f64>) -> f64 [linkage=external] [abi=sysv64(coerce<f64, f64>, coerce<f64, f64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_real<f64, reason=explicit>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%24), read<complex<f64>>(%25)));
// IR-NEXT:     }
// IR-NEXT:     fn %26 @set_parts(%27 value: ptr<complex<f64>>, %28 x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<f64>(real(deref(read<ptr<complex<f64>>>(%27))), read<f64>(%28));
// IR-NEXT:         write<f64>(imag(deref(read<ptr<complex<f64>>>(%27))), read<f64>(%28));
// IR-NEXT:     }
// IR-NEXT:     fn %29 @from_real(%30 a: f64) -> complex<f64> [linkage=external] [abi=sysv64(scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return real_to_complex<complex<f64>, reason=return>(read<f64>(%30));
// IR-NEXT:     }
// IR-NEXT:     fn %31 @from_integer(%32 a: i32) -> complex<f64> [linkage=external] [abi=sysv64(scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return real_to_complex<complex<f64>, reason=return>(int_to_float<f64, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%32)));
// IR-NEXT:     }
// IR-NEXT:     fn %33 @to_real(%34 a: complex<f64>) -> f64 [linkage=external] [abi=sysv64(coerce<f64, f64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_real<f64, reason=explicit>(read<complex<f64>>(%34));
// IR-NEXT:     }
// IR-NEXT:     fn %35 @integer_add(%36 a: complex<i32>, %37 b: complex<i32>) -> complex<i32> [linkage=external] [abi=sysv64(coerce<i64>, coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<i32>, complex=true, overflow=ub>(read<complex<i32>>(%36), read<complex<i32>>(%37));
// IR-NEXT:     }
// IR-NEXT:     fn %38 @integer_div(%39 a: complex<i32>, %40 b: complex<i32>) -> complex<i32> [linkage=external] [abi=sysv64(coerce<i64>, coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<complex<i32>, complex=true, overflow=ub, by_zero=ub>(read<complex<i32>>(%39), read<complex<i32>>(%40));
// IR-NEXT:     }
// IR-NEXT:     fn %41 @integer_to_float(%42 a: complex<i32>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<i64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_convert<complex<f64>, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<complex<i32>>(%42));
// IR-NEXT:     }
// IR-NEXT:     fn %43 @float_to_integer(%44 a: complex<f64>) -> complex<i32> [linkage=external] [abi=sysv64(coerce<f64, f64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_convert<complex<i32>, reason=explicit, out_of_range=ub, exceptions=ignore>(read<complex<f64>>(%44));
// IR-NEXT:     }
// IR-NEXT:     fn %45 @truth(%46 a: complex<f64>) -> f64 [linkage=external] [abi=sysv64(coerce<f64, f64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<f64>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%46), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0))), const<f64>(1.0), const<f64>(2.0));
// IR-NEXT:     }
// IR-NEXT:     fn %47 @complex_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(16);
// IR-NEXT:     }
// IR-NEXT:     fn %48 @wide_complex_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(32);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
