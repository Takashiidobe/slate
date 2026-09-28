typedef _Decimal64 money;

_Decimal32 small = 1.5DF;
_Decimal64 medium = 2.5dd;
_Decimal128 large = 3.5DL;
money total;

struct ledger {
  _Decimal32 fee;
  _Decimal128 balance;
};

_Decimal64 scale(_Decimal64 value, _Decimal32 factor);

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=8, align=8];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=8];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     type @type0 money = d64;
// C89-NEXT:     type @type1 ledger = struct {
// C89-NEXT:         field0 fee: d32;
// C89-NEXT:         field1 balance: d128;
// C89-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// C89-NEXT:     global %1 small: d32 [storage=static] = const<d32>(1.5) [linkage=external];
// C89-NEXT:     global %2 medium: d64 [storage=static] = const<d64>(2.5) [linkage=external];
// C89-NEXT:     global %3 large: d128 [storage=static] = const<d128>(3.5) [linkage=external];
// C89-NEXT:     global %4 total: d64 [storage=static] [linkage=external];
// C89-NEXT:     fn %8 @scale(%9 value: d64, %10 factor: d32) -> d64 [linkage=external];
// C89-NEXT: }
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     type @type0 money = d64;
// C23-NEXT:     type @type1 ledger = struct {
// C23-NEXT:         field0 fee: d32;
// C23-NEXT:         field1 balance: d128;
// C23-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// C23-NEXT:     global %1 small: d32 [storage=static] = const<d32>(1.5) [linkage=external];
// C23-NEXT:     global %2 medium: d64 [storage=static] = const<d64>(2.5) [linkage=external];
// C23-NEXT:     global %3 large: d128 [storage=static] = const<d128>(3.5) [linkage=external];
// C23-NEXT:     global %4 total: d64 [storage=static] [linkage=external];
// C23-NEXT:     fn %8 @scale(%9 value: d64, %10 factor: d32) -> d64 [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
