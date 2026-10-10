// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT

#line 18446744073709551616
long long wraps_to_zero = __LINE__;
#line 12312312312435
long long wraps = __LINE__;
#line 4294967295
long long max_unsigned = __LINE__;
#line 2147483648
long long past_int = __LINE__;
#line 2147483647
long long max_int = __LINE__;
#line 0
long long zero = __LINE__;

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
// DEFAULT-NEXT:     global %[[VALUE_wraps_to_zero:[0-9]+]] wraps_to_zero: i64 [storage=static] = widen<i64>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wraps:[0-9]+]] wraps: i64 [storage=static] = const<i64>(2936042099) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_max_unsigned:[0-9]+]] max_unsigned: i64 [storage=static] = const<i64>(4294967295) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_past_int:[0-9]+]] past_int: i64 [storage=static] = const<i64>(2147483648) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_max_int:[0-9]+]] max_int: i64 [storage=static] = widen<i64>(const<i32>(2147483647)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_zero:[0-9]+]] zero: i64 [storage=static] = widen<i64>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
