// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -std=c17

_Static_assert(_Generic(1i8, char: 1, default: 0), "i8 is char");
_Static_assert(_Generic(1ui8, unsigned char: 1, default: 0), "ui8 is unsigned char");
_Static_assert(_Generic(1i64, long long: 1, default: 0), "i64 is long long");
_Static_assert(_Generic(1i128, __int128: 1, default: 0), "i128 is __int128");
_Static_assert(_Generic(1ui128, unsigned __int128: 1, default: 0), "ui128");
_Static_assert(0xFFi8 == -1 && 200i8 == -56, "in-range bits reinterpret");
_Static_assert(18446744073709551615i64 == -1, "i64 reinterprets");

#if !(2147483648i32 > 0) || !(-1ui8 > 0) || 0xFFi8 < 0
#error preprocessor ignores the suffix width
#endif

long long max = 9223372036854775807i64;
__int128 wide = 1i128 << 100;
unsigned __int128 all = -1ui128;

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_max:[0-9]+]] max: i64 [storage=static] = const<i64>(9223372036854775807) [linkage=external];
// IR-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: i128 [storage=static] = shl<i128>(const<i128>(1), const<i32>(100)) [linkage=external];
// IR-NEXT:     global %[[VALUE_all:[0-9]+]] all: u128 [storage=static] = neg<u128>(const<u128>(1)) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
