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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %0 global: imaginary<f64> [storage=static] [linkage=external];
// IR-NEXT:     global %1 sizes: array<u64, 6> [storage=static] = aggregate<array<u64, 6>, zero_fill=false>(index0 = const<u64>(4), index1 = const<u64>(4), index2 = const<u64>(8), index3 = const<u64>(8), index4 = const<u64>(16), index5 = const<u64>(16)) [linkage=external];
// IR-NEXT:     fn %2 @literal() -> complex<f64> [linkage=external] [abi=sysv64() -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0));
// IR-NEXT:     }
// IR-NEXT:     fn %3 @literal_float() -> complex<f32> [linkage=external] [abi=sysv64() -> coerce<pair<f32>>] [fallthrough=ub_if_used] {
// IR-NEXT:         return aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @literal_long() -> complex<f80> [linkage=external] [abi=sysv64() -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// IR-NEXT:         return aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1.5));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @from_real(%6 x: f64) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return real_to_imaginary<imaginary<f64>, reason=return>(read<f64>(%6));
// IR-NEXT:     }
// IR-NEXT:     fn %7 @to_real(%8 y: imaginary<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return imaginary_to_real<f64, reason=return>(read<imaginary<f64>>(%8));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @to_complex(%10 y: imaginary<f64>) -> complex<f64> [linkage=external] [abi=sysv64(scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return imaginary_to_complex<complex<f64>, reason=return>(read<imaginary<f64>>(%10));
// IR-NEXT:     }
// IR-NEXT:     fn %11 @from_complex(%12 z: complex<f64>) -> imaginary<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_imaginary<imaginary<f64>, reason=return>(read<complex<f64>>(%12));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @from_int_complex(%14 z: complex<i32>) -> imaginary<f64> [linkage=external] [abi=sysv64(coerce<i64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_imaginary<imaginary<f64>, reason=return>(complex_convert<complex<f64>, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(read<complex<i32>>(%14)));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @widen(%16 y: imaginary<f32>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return imaginary_convert<imaginary<f64>, reason=return>(read<imaginary<f32>>(%16));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @narrow(%18 y: imaginary<f64>) -> imaginary<f32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return imaginary_convert<imaginary<f32>, reason=return, rounding=nearest_even, exceptions=ignore>(read<imaginary<f64>>(%18));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @real_times_imaginary(%20 x: f64, %21 y: imaginary<f64>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<imaginary<f64>, rounding=nearest_even, exceptions=ignore>(read<f64>(%20), read<imaginary<f64>>(%21));
// IR-NEXT:     }
// IR-NEXT:     fn %22 @imaginary_times_imaginary(%23 y: imaginary<f64>, %24 v: imaginary<f32>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<f64, rounding=nearest_even, exceptions=ignore>(read<imaginary<f64>>(%23), imaginary_convert<imaginary<f64>, reason=usual_arith>(read<imaginary<f32>>(%24)));
// IR-NEXT:     }
// IR-NEXT:     fn %25 @imaginary_over_imaginary(%26 y: imaginary<f64>, %27 v: imaginary<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<f64, rounding=nearest_even, exceptions=ignore>(read<imaginary<f64>>(%26), read<imaginary<f64>>(%27));
// IR-NEXT:     }
// IR-NEXT:     fn %28 @real_over_imaginary(%29 x: i32, %30 y: imaginary<f64>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<imaginary<f64>, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%29)), read<imaginary<f64>>(%30));
// IR-NEXT:     }
// IR-NEXT:     fn %31 @imaginary_sum(%32 y: imaginary<f64>, %33 v: imaginary<f64>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<imaginary<f64>, rounding=nearest_even, exceptions=ignore>(read<imaginary<f64>>(%32), read<imaginary<f64>>(%33));
// IR-NEXT:     }
// IR-NEXT:     fn %34 @real_plus_imaginary(%35 x: f64, %36 y: imaginary<f64>) -> complex<f64> [linkage=external] [abi=sysv64(scalar, scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(read<f64>(%35), read<imaginary<f64>>(%36));
// IR-NEXT:     }
// IR-NEXT:     fn %37 @imaginary_minus_real(%38 y: imaginary<f64>, %39 x: f64) -> complex<f64> [linkage=external] [abi=sysv64(scalar, scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(read<imaginary<f64>>(%38), read<f64>(%39));
// IR-NEXT:     }
// IR-NEXT:     fn %40 @complex_times_imaginary(%41 z: complex<f64>, %42 y: imaginary<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>, scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(read<complex<f64>>(%41), read<imaginary<f64>>(%42));
// IR-NEXT:     }
// IR-NEXT:     fn %43 @complex_plus_imaginary(%44 z: complex<f32>, %45 y: imaginary<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<pair<f32>>, scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%44)), read<imaginary<f64>>(%45));
// IR-NEXT:     }
// IR-NEXT:     fn %46 @imaginary_equal(%47 y: imaginary<f64>, %48 v: imaginary<f64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<imaginary<f64>, exceptions=ignore>(read<imaginary<f64>>(%47), read<imaginary<f64>>(%48)));
// IR-NEXT:     }
// IR-NEXT:     fn %49 @real_equal_imaginary(%50 x: f64, %51 y: imaginary<f64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<f64, exceptions=ignore>(read<f64>(%50), read<imaginary<f64>>(%51)));
// IR-NEXT:     }
// IR-NEXT:     fn %52 @negate(%53 y: imaginary<f64>) -> imaginary<f64> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<imaginary<f64>>(read<imaginary<f64>>(%53));
// IR-NEXT:     }
// IR-NEXT:     fn %54 @truth(%55 y: imaginary<f64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<imaginary<f64>, exceptions=ignore>(read<imaginary<f64>>(%55), const<imaginary<f64>>(0.0))
// IR-NEXT:             return from_bool<i32, reason=return>(not<bool>(ne<imaginary<f64>, exceptions=ignore>(read<imaginary<f64>>(%55), const<imaginary<f64>>(0.0))));
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %56 @choose(%57 c: i32, %58 y: imaginary<f64>, %59 x: f64) -> complex<f64> [linkage=external] [abi=sysv64(scalar, scalar, scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<complex<f64>>(ne<i32>(read<i32>(%57), const<i32>(0)), imaginary_to_complex<complex<f64>, reason=usual_arith>(read<imaginary<f64>>(%58)), real_to_complex<complex<f64>, reason=usual_arith>(read<f64>(%59)));
// IR-NEXT:     }
// IR-NEXT:     fn %60 @assign(%61 out: ptr<imaginary<f64>>, %62 y: imaginary<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %63: ptr<imaginary<f64>> [synthetic] = read<ptr<imaginary<f64>>>(%61);
// IR-NEXT:         let %64: imaginary<f64> [synthetic] = read<imaginary<f64>>(deref(read<ptr<imaginary<f64>>>(%63)));
// IR-NEXT:         let %65: imaginary<f64> [synthetic] = mul<imaginary<f64>, rounding=nearest_even, exceptions=ignore>(read<imaginary<f64>>(%64), const<f64>(2.0));
// IR-NEXT:         write<imaginary<f64>>(deref(read<ptr<imaginary<f64>>>(%63)), read<imaginary<f64>>(%65));
// IR-NEXT:         write<imaginary<f64>>(deref(read<ptr<imaginary<f64>>>(%61)), read<imaginary<f64>>(%62));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
