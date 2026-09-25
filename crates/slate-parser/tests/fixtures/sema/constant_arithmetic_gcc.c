// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --flavor=gcc

enum GccConstants {
    SHIFT_HIGH = 1 << 32,
    ZERO_SHIFT = 0 << -1,
    NEGATIVE_RIGHT = -1 >> -1,
    OVERFLOW = 2147483647 + 1,
};

_Static_assert(SHIFT_HIGH == 0, "");
_Static_assert(ZERO_SHIFT == 0, "");
_Static_assert(NEGATIVE_RIGHT == -1, "");
_Static_assert(OVERFLOW == (-2147483647 - 1), "");

#if 1 << -1
#error invalid preprocessor shift must be zero
#endif

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
// IR-NEXT:     type @type0 GccConstants = enum : i32 {
// IR-NEXT:         %0 SHIFT_HIGH = const<i32>(0);
// IR-NEXT:         %1 ZERO_SHIFT = const<i32>(0);
// IR-NEXT:         %2 NEGATIVE_RIGHT = const<i32>(-1);
// IR-NEXT:         %3 OVERFLOW = const<i32>(-2147483648);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
