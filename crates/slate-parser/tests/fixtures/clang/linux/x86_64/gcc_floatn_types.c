// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES CLANG CLANG

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

// SLATE-FILECHECK-BEGIN CLANG
// CLANG: module {
// CLANG-NEXT:     target "x86_64-unknown-linux-gnu" {
// CLANG-NEXT:         endian = little;
// CLANG-NEXT:         pointer [size=8, align=8];
// CLANG-NEXT:         stack_alignment = 16;
// CLANG-NEXT:         long_double = f80;
// CLANG-NEXT:         storage bool [size=1, align=1];
// CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// CLANG-NEXT:         storage i64, u64 [size=8, align=8];
// CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// CLANG-NEXT:         storage bf16 [size=2, align=2];
// CLANG-NEXT:         storage f16 [size=2, align=2];
// CLANG-NEXT:         storage f32 [size=4, align=4];
// CLANG-NEXT:         storage f64 [size=8, align=8];
// CLANG-NEXT:         storage f80 [size=16, align=16];
// CLANG-NEXT:         storage f128 [size=16, align=16];
// CLANG-NEXT:         storage d32 [size=4, align=4];
// CLANG-NEXT:         storage d64 [size=8, align=8];
// CLANG-NEXT:         storage d128 [size=16, align=16];
// CLANG-NEXT:     }
// CLANG-NEXT:     global %0 _Float32: i32 [storage=static] [linkage=external];
// CLANG-NEXT:     global %1 _Float64: i32 [storage=static] [linkage=external];
// CLANG-NEXT:     global %2 _Float32x: i32 [storage=static] [linkage=external];
// CLANG-NEXT:     global %3 _Float64x: i32 [storage=static] [linkage=external];
// CLANG-NEXT:     global %4 _Float128: i32 [storage=static] [linkage=external];
// CLANG-NEXT:     global %5 __float80: i32 [storage=static] [linkage=external];
// CLANG-NEXT: }
// SLATE-FILECHECK-END CLANG
