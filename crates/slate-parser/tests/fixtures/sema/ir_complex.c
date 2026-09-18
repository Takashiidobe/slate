// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

double _Complex add(double _Complex a, double _Complex b) {
    return a + b;
}

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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %0 @add(%1 a: complex<f64>, %2 b: complex<f64>) -> complex<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(read<complex<f64>>(%1), read<complex<f64>>(%2));
// IR-NEXT:     }
// IR-NEXT:     fn %3 @mixed(%4 a: complex<f64>, %5 b: f64) -> complex<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(read<complex<f64>>(%4), read<f64>(%5));
// IR-NEXT:     }
// IR-NEXT:     fn %6 @add_real(%7 a: complex<f64>, %8 b: f64) -> complex<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(read<complex<f64>>(%7), read<f64>(%8));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @precision(%10 a: complex<f32>, %11 b: complex<f64>) -> complex<f32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_convert<complex<f32>, reason=return, rounding=nearest_even, exceptions=ignore>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%10)), read<complex<f64>>(%11)));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @equal(%13 a: complex<f64>, %14 b: complex<f64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<complex<f64>, exceptions=ignore>(read<complex<f64>>(%13), read<complex<f64>>(%14)));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @equal_real(%16 a: complex<f64>, %17 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<complex<f64>, exceptions=ignore>(read<complex<f64>>(%16), read<f64>(%17)));
// IR-NEXT:     }
// IR-NEXT:     fn %18 @real_part(%19 a: complex<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<f64>(real(%19));
// IR-NEXT:     }
// IR-NEXT:     fn %20 @imag_part(%21 a: complex<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<f64>(imag(%21));
// IR-NEXT:     }
// IR-NEXT:     fn %22 @result_real(%23 a: complex<f64>, %24 b: complex<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_real<f64, reason=explicit>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(read<complex<f64>>(%23), read<complex<f64>>(%24)));
// IR-NEXT:     }
// IR-NEXT:     fn %25 @set_parts(%26 value: ptr<complex<f64>>, %27 x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<f64>(real(deref(read<ptr<complex<f64>>>(%26))), read<f64>(%27));
// IR-NEXT:         write<f64>(imag(deref(read<ptr<complex<f64>>>(%26))), read<f64>(%27));
// IR-NEXT:     }
// IR-NEXT:     fn %28 @from_real(%29 a: f64) -> complex<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return real_to_complex<complex<f64>, reason=return>(read<f64>(%29));
// IR-NEXT:     }
// IR-NEXT:     fn %30 @from_integer(%31 a: i32) -> complex<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return real_to_complex<complex<f64>, reason=return>(int_to_float<f64, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%31)));
// IR-NEXT:     }
// IR-NEXT:     fn %32 @to_real(%33 a: complex<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_real<f64, reason=explicit>(read<complex<f64>>(%33));
// IR-NEXT:     }
// IR-NEXT:     fn %34 @integer_add(%35 a: complex<i32>, %36 b: complex<i32>) -> complex<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<i32>, complex=true, overflow=ub>(read<complex<i32>>(%35), read<complex<i32>>(%36));
// IR-NEXT:     }
// IR-NEXT:     fn %37 @integer_div(%38 a: complex<i32>, %39 b: complex<i32>) -> complex<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<complex<i32>, complex=true, overflow=ub, by_zero=ub>(read<complex<i32>>(%38), read<complex<i32>>(%39));
// IR-NEXT:     }
// IR-NEXT:     fn %40 @integer_to_float(%41 a: complex<i32>) -> complex<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_convert<complex<f64>, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<complex<i32>>(%41));
// IR-NEXT:     }
// IR-NEXT:     fn %42 @float_to_integer(%43 a: complex<f64>) -> complex<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_convert<complex<i32>, reason=explicit, out_of_range=ub, exceptions=ignore>(read<complex<f64>>(%43));
// IR-NEXT:     }
// IR-NEXT:     fn %44 @truth(%45 a: complex<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<f64>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%45), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0))), const<f64>(1.0), const<f64>(2.0));
// IR-NEXT:     }
// IR-NEXT:     fn %46 @complex_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(16);
// IR-NEXT:     }
// IR-NEXT:     fn %47 @wide_complex_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(32);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
