// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES AARCH64 QUAD_LONG_DOUBLE

#if defined(CLANG)
int _Float32, _Float64, _Float32x, _Float64x, _Float128, __float80;
#elif defined(NO_FLOAT128)
_Float128 unsupported;
#else
#define SAME(E, T) _Static_assert(_Generic((E), T: 1, default: 0), "")
#define DISTINCT(T, U) _Static_assert(!_Generic((T){0}, U: 1, default: 0), "")

DISTINCT(_Float32, float);
DISTINCT(_Float64, double);
DISTINCT(_Float32x, double);
DISTINCT(_Float32x, _Float64);
DISTINCT(_Float64x, long double);
SAME(1.0f32 + 1.0f, _Float32);
SAME(1.0f32 + 1.0, double);
SAME(1.0f32x + 1.0, double);
SAME(1.0f64 + 1.0f32x, _Float64);
SAME(1.0f64x + 1.0L, long double);
SAME(1.0f128 + 1.0L, _Float128);
SAME(1.0f128 + 1.0f64x, _Float128);

_Float32 f32 = 1.5f32;
_Float64 f64 = 2.5f64;
_Float32x f32x = 3.5f32x;
_Float64x f64x = 4.5f64x;
_Float128 f128 = 5.5f128;
_Complex _Float32 c32;
_Float64 _Complex c64;

#ifdef X87
SAME((__float80)0, long double);
SAME((__float128)0, _Float128);
__float80 f80 = 6.5w;
_Static_assert(sizeof(_Float64x) == sizeof(long double), "");
#endif

#ifdef QUAD_LONG_DOUBLE
DISTINCT(_Float128, long double);
_Static_assert(sizeof(_Float64x) == 16, "");
#endif
#endif

// SLATE-FILECHECK-BEGIN AARCH64
// AARCH64: module {
// AARCH64-NEXT:     target "aarch64-unknown-linux-gnu" {
// AARCH64-NEXT:         endian = little;
// AARCH64-NEXT:         pointer [size=8, align=8];
// AARCH64-NEXT:         stack_alignment = 16;
// AARCH64-NEXT:         long_double = f128;
// AARCH64-NEXT:         storage bool [size=1, align=1];
// AARCH64-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-NEXT:         storage f16 [size=2, align=2];
// AARCH64-NEXT:         storage f32 [size=4, align=4];
// AARCH64-NEXT:         storage f64 [size=8, align=8];
// AARCH64-NEXT:         storage f128 [size=16, align=16];
// AARCH64-NEXT:         storage d32 [size=4, align=4];
// AARCH64-NEXT:         storage d64 [size=8, align=8];
// AARCH64-NEXT:         storage d128 [size=16, align=16];
// AARCH64-NEXT:     }
// AARCH64-NEXT:     global %[[VALUE_f32:[0-9]+]] f32: f32 [storage=static] = const<f32>(1.5) [linkage=external];
// AARCH64-NEXT:     global %[[VALUE_f64:[0-9]+]] f64: f64 [storage=static] = const<f64>(2.5) [linkage=external];
// AARCH64-NEXT:     global %[[VALUE_f32x:[0-9]+]] f32x: f64 [storage=static] = const<f64>(3.5) [linkage=external];
// AARCH64-NEXT:     global %[[VALUE_f64x:[0-9]+]] f64x: f128 [storage=static] = const<f128>(4.5) [linkage=external];
// AARCH64-NEXT:     global %[[VALUE_f128:[0-9]+]] f128: f128 [storage=static] = const<f128>(5.5) [linkage=external];
// AARCH64-NEXT:     global %[[VALUE_c32:[0-9]+]] c32: complex<f32> [storage=static] [linkage=external];
// AARCH64-NEXT:     global %[[VALUE_c64:[0-9]+]] c64: complex<f64> [storage=static] [linkage=external];
// AARCH64-NEXT: }
// SLATE-FILECHECK-END AARCH64
