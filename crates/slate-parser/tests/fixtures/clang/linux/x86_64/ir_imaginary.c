// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

double _Imaginary global;
unsigned long sizes[6] = {
    sizeof(float _Imaginary), _Alignof(float _Imaginary),
    sizeof(double _Imaginary), _Alignof(double _Imaginary),
    sizeof(long double _Imaginary), _Alignof(long double _Imaginary),
};

double _Complex literal(void) {
    return 2.0i;
}

float _Complex literal_float(void) {
    return 3.0fj;
}

long double _Complex literal_long(void) {
    return 1.5Li;
}

int _Complex literal_int(void) {
    return 3i;
}

void literal_int_suffixes(void) {
    (void)3ui;
    (void)3iu;
    (void)0x10j;
    (void)3lI;
    (void)5000000000i;
    (void)3ULLi;
}

double _Imaginary from_real(double x) {
    return x;
}

double to_real(double _Imaginary y) {
    return y;
}

double _Complex to_complex(double _Imaginary y) {
    return y;
}

double _Imaginary from_complex(double _Complex z) {
    return z;
}

double _Imaginary from_int_complex(int _Complex z) {
    return z;
}

double _Imaginary widen(float _Imaginary y) {
    return y;
}

float _Imaginary narrow(double _Imaginary y) {
    return y;
}

double _Imaginary real_times_imaginary(double x, double _Imaginary y) {
    return x * y;
}

double imaginary_times_imaginary(double _Imaginary y, float _Imaginary v) {
    return y * v;
}

double imaginary_over_imaginary(double _Imaginary y, double _Imaginary v) {
    return y / v;
}

double _Imaginary real_over_imaginary(int x, double _Imaginary y) {
    return x / y;
}

double _Imaginary imaginary_sum(double _Imaginary y, double _Imaginary v) {
    return y + v;
}

double _Complex real_plus_imaginary(double x, double _Imaginary y) {
    return x + y;
}

double _Complex imaginary_minus_real(double _Imaginary y, double x) {
    return y - x;
}

double _Complex complex_times_imaginary(double _Complex z, double _Imaginary y) {
    return z * y;
}

double _Complex complex_plus_imaginary(float _Complex z, double _Imaginary y) {
    return z + y;
}

int imaginary_equal(double _Imaginary y, double _Imaginary v) {
    return y == v;
}

int real_equal_imaginary(double x, double _Imaginary y) {
    return x == y;
}

double _Imaginary negate(double _Imaginary y) {
    return -y;
}

int truth(double _Imaginary y) {
    if (y)
        return !y;
    return 0;
}

double _Complex choose(int c, double _Imaginary y, double x) {
    return c ? y : x;
}

void assign(double _Imaginary *out, double _Imaginary y) {
    *out *= 2.0;
    *out = y;
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
// IR-NEXT:     global %[[VALUE_global:[0-9]+]] global: imaginary<f64> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: array<u64, 6> [storage=static] [align=16] = aggregate<array<u64, 6>, zero_fill=false>(index0 = const<u64>(4), index1 = const<u64>(4), index2 = const<u64>(8), index3 = const<u64>(8), index4 = const<u64>(16), index5 = const<u64>(16)) [linkage=external];
// IR-NEXT:     fn %[[VALUE_literal:[0-9]+]] @literal() -> complex<f64> [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_literal_float:[0-9]+]] @literal_float() -> complex<f32> [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_literal_long:[0-9]+]] @literal_long() -> complex<f80> [linkage=external] [abi=sysv64() -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// IR-NEXT:         return aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1.5));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_literal_int:[0-9]+]] @literal_int() -> complex<i32> [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(3));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_literal_int_suffixes:[0-9]+]] @literal_int_suffixes() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         aggregate<complex<u32>, zero_fill=false>(index0 = const<u32>(0), index1 = const<u32>(3));
// IR-NEXT:         aggregate<complex<u32>, zero_fill=false>(index0 = const<u32>(0), index1 = const<u32>(3));
// IR-NEXT:         aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(16));
// IR-NEXT:         aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(3));
// IR-NEXT:         aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(5000000000));
// IR-NEXT:         aggregate<complex<u64>, zero_fill=false>(index0 = const<u64>(0), index1 = const<u64>(3));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_from_real:[0-9]+]] @from_real(%[[VALUE_x:[0-9]+]] x: f64) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return real_to_imaginary<imaginary<f64>, reason=return>(read<f64>(%[[VALUE_x]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_to_real:[0-9]+]] @to_real(%[[VALUE_y:[0-9]+]] y: imaginary<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return imaginary_to_real<f64, reason=return>(read<imaginary<f64>>(%[[VALUE_y]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_to_complex:[0-9]+]] @to_complex(%[[VALUE_y_2:[0-9]+]] y: imaginary<f64>) -> complex<f64> [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return imaginary_to_complex<complex<f64>, reason=return>(read<imaginary<f64>>(%[[VALUE_y_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_from_complex:[0-9]+]] @from_complex(%[[VALUE_z:[0-9]+]] z: complex<f64>) -> imaginary<f64> [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_imaginary<imaginary<f64>, reason=return>(read<complex<f64>>(%[[VALUE_z]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_from_int_complex:[0-9]+]] @from_int_complex(%[[VALUE_z_2:[0-9]+]] z: complex<i32>) -> imaginary<f64> [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_imaginary<imaginary<f64>, reason=return>(complex_convert<complex<f64>, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(read<complex<i32>>(%[[VALUE_z_2]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_y_3:[0-9]+]] y: imaginary<f32>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return imaginary_convert<imaginary<f64>, reason=return>(read<imaginary<f32>>(%[[VALUE_y_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_narrow:[0-9]+]] @narrow(%[[VALUE_y_4:[0-9]+]] y: imaginary<f64>) -> imaginary<f32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return imaginary_convert<imaginary<f32>, reason=return, rounding=nearest_even, exceptions=ignore>(read<imaginary<f64>>(%[[VALUE_y_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_real_times_imaginary:[0-9]+]] @real_times_imaginary(%[[VALUE_x_2:[0-9]+]] x: f64, %[[VALUE_y_5:[0-9]+]] y: imaginary<f64>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<imaginary<f64>, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x_2]]), read<imaginary<f64>>(%[[VALUE_y_5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_imaginary_times_imaginary:[0-9]+]] @imaginary_times_imaginary(%[[VALUE_y_6:[0-9]+]] y: imaginary<f64>, %[[VALUE_v:[0-9]+]] v: imaginary<f32>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<imaginary<f64>>(%[[VALUE_y_6]]), imaginary_convert<imaginary<f64>, reason=usual_arith>(read<imaginary<f32>>(%[[VALUE_v]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_imaginary_over_imaginary:[0-9]+]] @imaginary_over_imaginary(%[[VALUE_y_7:[0-9]+]] y: imaginary<f64>, %[[VALUE_v_2:[0-9]+]] v: imaginary<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<imaginary<f64>>(%[[VALUE_y_7]]), read<imaginary<f64>>(%[[VALUE_v_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_real_over_imaginary:[0-9]+]] @real_over_imaginary(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_8:[0-9]+]] y: imaginary<f64>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<imaginary<f64>, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_x_3]])), read<imaginary<f64>>(%[[VALUE_y_8]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_imaginary_sum:[0-9]+]] @imaginary_sum(%[[VALUE_y_9:[0-9]+]] y: imaginary<f64>, %[[VALUE_v_3:[0-9]+]] v: imaginary<f64>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<imaginary<f64>, rounding=nearest_even, exceptions=ignore, contract=on>(read<imaginary<f64>>(%[[VALUE_y_9]]), read<imaginary<f64>>(%[[VALUE_v_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_real_plus_imaginary:[0-9]+]] @real_plus_imaginary(%[[VALUE_x_4:[0-9]+]] x: f64, %[[VALUE_y_10:[0-9]+]] y: imaginary<f64>) -> complex<f64> [linkage=external] [abi=sysv64(scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<f64>(%[[VALUE_x_4]]), read<imaginary<f64>>(%[[VALUE_y_10]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_imaginary_minus_real:[0-9]+]] @imaginary_minus_real(%[[VALUE_y_11:[0-9]+]] y: imaginary<f64>, %[[VALUE_x_5:[0-9]+]] x: f64) -> complex<f64> [linkage=external] [abi=sysv64(scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<imaginary<f64>>(%[[VALUE_y_11]]), read<f64>(%[[VALUE_x_5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_times_imaginary:[0-9]+]] @complex_times_imaginary(%[[VALUE_z_3:[0-9]+]] z: complex<f64>, %[[VALUE_y_12:[0-9]+]] y: imaginary<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_z_3]]), read<imaginary<f64>>(%[[VALUE_y_12]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_plus_imaginary:[0-9]+]] @complex_plus_imaginary(%[[VALUE_z_4:[0-9]+]] z: complex<f32>, %[[VALUE_y_13:[0-9]+]] y: imaginary<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%[[VALUE_z_4]])), read<imaginary<f64>>(%[[VALUE_y_13]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_imaginary_equal:[0-9]+]] @imaginary_equal(%[[VALUE_y_14:[0-9]+]] y: imaginary<f64>, %[[VALUE_v_4:[0-9]+]] v: imaginary<f64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<imaginary<f64>, exceptions=ignore>(read<imaginary<f64>>(%[[VALUE_y_14]]), read<imaginary<f64>>(%[[VALUE_v_4]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_real_equal_imaginary:[0-9]+]] @real_equal_imaginary(%[[VALUE_x_6:[0-9]+]] x: f64, %[[VALUE_y_15:[0-9]+]] y: imaginary<f64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<f64, exceptions=ignore>(read<f64>(%[[VALUE_x_6]]), read<imaginary<f64>>(%[[VALUE_y_15]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_negate:[0-9]+]] @negate(%[[VALUE_y_16:[0-9]+]] y: imaginary<f64>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<imaginary<f64>>(read<imaginary<f64>>(%[[VALUE_y_16]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_truth:[0-9]+]] @truth(%[[VALUE_y_17:[0-9]+]] y: imaginary<f64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<imaginary<f64>, exceptions=ignore>(read<imaginary<f64>>(%[[VALUE_y_17]]), const<imaginary<f64>>(0.0))
// IR-NEXT:             return from_bool<i32, reason=return>(not<bool>(ne<imaginary<f64>, exceptions=ignore>(read<imaginary<f64>>(%[[VALUE_y_17]]), const<imaginary<f64>>(0.0))));
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_choose:[0-9]+]] @choose(%[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_y_18:[0-9]+]] y: imaginary<f64>, %[[VALUE_x_7:[0-9]+]] x: f64) -> complex<f64> [linkage=external] [abi=sysv64(scalar, scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<complex<f64>>(ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0)), imaginary_to_complex<complex<f64>, reason=usual_arith>(read<imaginary<f64>>(%[[VALUE_y_18]])), real_to_complex<complex<f64>, reason=usual_arith>(read<f64>(%[[VALUE_x_7]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_assign:[0-9]+]] @assign(%[[VALUE_out:[0-9]+]] out: ptr<imaginary<f64>>, %[[VALUE_y_19:[0-9]+]] y: imaginary<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<imaginary<f64>> [synthetic] = read<ptr<imaginary<f64>>>(%[[VALUE_out]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: imaginary<f64> [synthetic] = read<imaginary<f64>>(deref(read<ptr<imaginary<f64>>>(%[[VALUE0]])));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: imaginary<f64> [synthetic] = mul<imaginary<f64>, rounding=nearest_even, exceptions=ignore, contract=on>(read<imaginary<f64>>(%[[VALUE1]]), const<f64>(2.0));
// IR-NEXT:         write<imaginary<f64>>(deref(read<ptr<imaginary<f64>>>(%[[VALUE0]])), read<imaginary<f64>>(%[[VALUE2]]));
// IR-NEXT:         write<imaginary<f64>>(deref(read<ptr<imaginary<f64>>>(%[[VALUE_out]])), read<imaginary<f64>>(%[[VALUE_y_19]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
