// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

_Static_assert(sizeof(__int128) == 16, "");
_Static_assert(_Alignof(__int128) == 16, "");
_Static_assert(sizeof(_BitInt(128)) == 16, "");
_Static_assert(_Alignof(_BitInt(128)) == 8, "");
_Static_assert(sizeof(_BitInt(32)) == 4, "");
_Static_assert(_Alignof(_BitInt(32)) == 4, "");

__int128 wide;
_BitInt(128) precise;
unsigned _BitInt(32) narrow;

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
// IR-NEXT:     global %0 wide: i128 [storage=static] [linkage=external];
// IR-NEXT:     global %1 precise: i128b [storage=static] [linkage=external];
// IR-NEXT:     global %2 narrow: u32b [storage=static] [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
