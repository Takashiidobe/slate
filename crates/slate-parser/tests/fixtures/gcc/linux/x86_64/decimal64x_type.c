// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-STD DEFAULT c23
// SLATE-FILECHECK-DEFINES DEFAULT

#define expr_has_type(e, t) _Generic((e), default : 0, t : 1)

_Decimal64x plain = 1.5d64x;
_Decimal64x upper = 2.5D64x;
_Decimal64x inf = __builtin_infd64x();
_Decimal64x magnitude = __builtin_fabsd64x(-1.5D64x);
_Decimal64x sum(_Decimal64x a, _Decimal64x b) { return a + b; }

static_assert(expr_has_type(1.1d64x, _Decimal64x));
static_assert(!expr_has_type(1.1d64x, _Decimal128));
static_assert(expr_has_type(1.1D64 + 1.1d64x, _Decimal64x));
static_assert(expr_has_type(1.1D128 + 1.1d64x, _Decimal128));
static_assert(expr_has_type(1 + 1.1d64x, _Decimal64x));

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
// DEFAULT-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: d128 [storage=static] = const<d128>(1.5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_upper:[0-9]+]] upper: d128 [storage=static] = const<d128>(2.5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_inf:[0-9]+]] inf: d128 [storage=static] = call<d128>(%[[VALUE___builtin_infd64x:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_magnitude:[0-9]+]] magnitude: d128 [storage=static] = call<d128>(%[[VALUE___builtin_fabsd64x:[0-9]+]], neg<d128>(const<d128>(1.5))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infd64x]] @__builtin_infd64x() -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabsd64x]] @__builtin_fabsd64x(%[[VALUE0:[0-9]+]] <unnamed>: d128) -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_a:[0-9]+]] a: d128, %[[VALUE_b:[0-9]+]] b: d128) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<d128>(read<d128>(%[[VALUE_a]]), read<d128>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
