// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

#if __has_builtin(__builtin_infd32) && __has_builtin(__builtin_nansd128)
int has_decimal = 1;
#endif

_Decimal32 inf32 = __builtin_infd32();
_Decimal64 inf64 = __builtin_infd64();
_Decimal128 inf128 = __builtin_infd128();
_Decimal32 nan32 = __builtin_nand32("");
_Decimal64 nan64 = __builtin_nand64("1");
_Decimal128 nan128 = __builtin_nand128("");
_Decimal32 nans32 = __builtin_nansd32("");
_Decimal64 nans64 = __builtin_nansd64("");
_Decimal128 nans128 = __builtin_nansd128("");

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
// DEFAULT-NEXT:     global %[[VALUE_has_decimal:[0-9]+]] has_decimal: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_inf32:[0-9]+]] inf32: d32 [storage=static] = call<d32>(%[[VALUE___builtin_infd32:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_inf64:[0-9]+]] inf64: d64 [storage=static] = call<d64>(%[[VALUE___builtin_infd64:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_inf128:[0-9]+]] inf128: d128 [storage=static] = call<d128>(%[[VALUE___builtin_infd128:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_nan32:[0-9]+]] nan32: d32 [storage=static] = call<d32>(%[[VALUE___builtin_nand32:[0-9]+]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_nan64:[0-9]+]] nan64: d64 [storage=static] = call<d64>(%[[VALUE___builtin_nand64:[0-9]+]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_nan128:[0-9]+]] nan128: d128 [storage=static] = call<d128>(%[[VALUE___builtin_nand128:[0-9]+]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_nans32:[0-9]+]] nans32: d32 [storage=static] = call<d32>(%[[VALUE___builtin_nansd32:[0-9]+]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_4]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_nans64:[0-9]+]] nans64: d64 [storage=static] = call<d64>(%[[VALUE___builtin_nansd64:[0-9]+]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_5]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_nans128:[0-9]+]] nans128: d128 [storage=static] = call<d128>(%[[VALUE___builtin_nansd128:[0-9]+]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_6]]))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infd32]] @__builtin_infd32() -> d32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infd64]] @__builtin_infd64() -> d64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infd128]] @__builtin_infd128() -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nand32]] @__builtin_nand32(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> d32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nand64]] @__builtin_nand64(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> d64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nand128]] @__builtin_nand128(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> d128 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd32]] @__builtin_nansd32(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>) -> d32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd64]] @__builtin_nansd64(%[[VALUE4:[0-9]+]] <unnamed>: ptr<const i8>) -> d64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd128]] @__builtin_nansd128(%[[VALUE5:[0-9]+]] <unnamed>: ptr<const i8>) -> d128 [linkage=external] [memory=read];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
