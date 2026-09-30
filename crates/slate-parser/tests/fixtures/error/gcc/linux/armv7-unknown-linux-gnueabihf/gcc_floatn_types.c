// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES ARMV7 NO_FLOAT128
// SLATE-FILECHECK-ERROR ARMV7

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

// SLATE-FILECHECK-BEGIN ARMV7
// ARMV7: Error:   × semantic analysis failed
// ARMV7: Error:
// ARMV7: × floating type is not supported on this target
// ARMV7: ╭─[tests/fixtures/error/gcc/linux/armv7-unknown-linux-gnueabihf/gcc_floatn_types.c:5:1]
// ARMV7: 4 │ #elif defined(NO_FLOAT128)
// ARMV7: 5 │ _Float128 unsupported;
// ARMV7: · ──────────────────────
// ARMV7: 6 │ #else
// ARMV7: ╰────
// SLATE-FILECHECK-END ARMV7
