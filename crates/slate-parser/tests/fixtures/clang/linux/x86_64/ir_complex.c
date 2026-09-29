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
// IR-NEXT:     global %[[VALUE_bfloat_complex:[0-9]+]] bfloat_complex: complex<bf16> [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_mixed:[0-9]+]] @mixed(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: f64) -> complex<f64> [linkage=external] [abi=sysv64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_add_real:[0-9]+]] @add_real(%[[VALUE_a_3:[0-9]+]] a: complex<f64>, %[[VALUE_b_3:[0-9]+]] b: f64) -> complex<f64> [linkage=external] [abi=sysv64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_3]]), read<f64>(%[[VALUE_b_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_precision:[0-9]+]] @precision(%[[VALUE_a_4:[0-9]+]] a: complex<f32>, %[[VALUE_b_4:[0-9]+]] b: complex<f64>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_convert<complex<f32>, reason=return, rounding=nearest_even, exceptions=ignore>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%[[VALUE_a_4]])), read<complex<f64>>(%[[VALUE_b_4]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_equal:[0-9]+]] @equal(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> i32 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_a_5]]), read<complex<f64>>(%[[VALUE_b_5]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_equal_real:[0-9]+]] @equal_real(%[[VALUE_a_6:[0-9]+]] a: complex<f64>, %[[VALUE_b_6:[0-9]+]] b: f64) -> i32 [linkage=external] [abi=sysv64(native_c, scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_a_6]]), read<f64>(%[[VALUE_b_6]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_real_part:[0-9]+]] @real_part(%[[VALUE_a_7:[0-9]+]] a: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<f64>(real(%[[VALUE_a_7]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_imag_part:[0-9]+]] @imag_part(%[[VALUE_a_8:[0-9]+]] a: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<f64>(imag(%[[VALUE_a_8]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_result_real:[0-9]+]] @result_real(%[[VALUE_a_9:[0-9]+]] a: complex<f64>, %[[VALUE_b_7:[0-9]+]] b: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_real<f64, reason=explicit>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_9]]), read<complex<f64>>(%[[VALUE_b_7]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_set_parts:[0-9]+]] @set_parts(%[[VALUE_value:[0-9]+]] value: ptr<complex<f64>>, %[[VALUE_x:[0-9]+]] x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<f64>(real(deref(read<ptr<complex<f64>>>(%[[VALUE_value]]))), read<f64>(%[[VALUE_x]]));
// IR-NEXT:         write<f64>(imag(deref(read<ptr<complex<f64>>>(%[[VALUE_value]]))), read<f64>(%[[VALUE_x]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_from_real:[0-9]+]] @from_real(%[[VALUE_a_10:[0-9]+]] a: f64) -> complex<f64> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return real_to_complex<complex<f64>, reason=return>(read<f64>(%[[VALUE_a_10]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_from_integer:[0-9]+]] @from_integer(%[[VALUE_a_11:[0-9]+]] a: i32) -> complex<f64> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return real_to_complex<complex<f64>, reason=return>(int_to_float<f64, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_a_11]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_to_real:[0-9]+]] @to_real(%[[VALUE_a_12:[0-9]+]] a: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_real<f64, reason=explicit>(read<complex<f64>>(%[[VALUE_a_12]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_integer_add:[0-9]+]] @integer_add(%[[VALUE_a_13:[0-9]+]] a: complex<i32>, %[[VALUE_b_8:[0-9]+]] b: complex<i32>) -> complex<i32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<i32>, complex=true, overflow=ub>(read<complex<i32>>(%[[VALUE_a_13]]), read<complex<i32>>(%[[VALUE_b_8]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_integer_div:[0-9]+]] @integer_div(%[[VALUE_a_14:[0-9]+]] a: complex<i32>, %[[VALUE_b_9:[0-9]+]] b: complex<i32>) -> complex<i32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<complex<i32>, complex=true, overflow=ub, by_zero=ub>(read<complex<i32>>(%[[VALUE_a_14]]), read<complex<i32>>(%[[VALUE_b_9]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_integer_to_float:[0-9]+]] @integer_to_float(%[[VALUE_a_15:[0-9]+]] a: complex<i32>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_convert<complex<f64>, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<complex<i32>>(%[[VALUE_a_15]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_float_to_integer:[0-9]+]] @float_to_integer(%[[VALUE_a_16:[0-9]+]] a: complex<f64>) -> complex<i32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_convert<complex<i32>, reason=explicit, out_of_range=ub, exceptions=ignore>(read<complex<f64>>(%[[VALUE_a_16]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_truth:[0-9]+]] @truth(%[[VALUE_a_17:[0-9]+]] a: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<f64>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_a_17]]), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0))), const<f64>(1.0), const<f64>(2.0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_size:[0-9]+]] @complex_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(16);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_wide_complex_size:[0-9]+]] @wide_complex_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(32);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
