long long int_max = 2147483647;
long long decimal_above_int_max = 2147483648;
long long decimal_at_unsigned_long_max = 4294967295;
long long decimal_above_unsigned_long_max = 4294967296;
long long decimal_at_long_long_max = 9223372036854775807;
long long decimal_above_long_long_max = 9223372036854775808;
long long decimal_at_unsigned_long_long_max = 18446744073709551615;
long long octal_above_int_max = 020000000000;
long long octal_above_unsigned_long_max = 040000000000;
long long hex_above_int_max = 0x80000000;
long long hex_above_unsigned_long_max = 0x100000000;
long long long_suffix_above_long_max = 2147483648L;
long long unsigned_suffix_above_unsigned_long_max = 4294967296U;
long long long_long_suffix_above_long_max = 2147483648LL;

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "i686-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=4, align=4];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=4];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=4];
// C89-NEXT:         storage f80 [size=12, align=4];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     global %0 int_max: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(2147483647)) [linkage=external];
// C89-NEXT:     global %1 decimal_above_int_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2147483648))) [linkage=external];
// C89-NEXT:     global %2 decimal_at_unsigned_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(4294967295))) [linkage=external];
// C89-NEXT:     global %3 decimal_above_unsigned_long_max: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// C89-NEXT:     global %4 decimal_at_long_long_max: i64 [storage=static] = const<i64>(9223372036854775807) [linkage=external];
// C89-NEXT:     global %5 decimal_above_long_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(9223372036854775808)) [linkage=external];
// C89-NEXT:     global %6 decimal_at_unsigned_long_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551615)) [linkage=external];
// C89-NEXT:     global %7 octal_above_int_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2147483648))) [linkage=external];
// C89-NEXT:     global %8 octal_above_unsigned_long_max: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// C89-NEXT:     global %9 hex_above_int_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2147483648))) [linkage=external];
// C89-NEXT:     global %10 hex_above_unsigned_long_max: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// C89-NEXT:     global %11 long_suffix_above_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2147483648))) [linkage=external];
// C89-NEXT:     global %12 unsigned_suffix_above_unsigned_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=always>(const<u64>(4294967296)) [linkage=external];
// C89-NEXT:     global %13 long_long_suffix_above_long_max: i64 [storage=static] = const<i64>(2147483648) [linkage=external];
// C89-NEXT: }
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN C99
// C99: module {
// C99-NEXT:     target "i686-unknown-linux-gnu" {
// C99-NEXT:         endian = little;
// C99-NEXT:         pointer [size=4, align=4];
// C99-NEXT:         stack_alignment = 16;
// C99-NEXT:         long_double = f80;
// C99-NEXT:         storage bool [size=1, align=1];
// C99-NEXT:         storage i8, u8 [size=1, align=1];
// C99-NEXT:         storage i16, u16 [size=2, align=2];
// C99-NEXT:         storage i32, u32 [size=4, align=4];
// C99-NEXT:         storage i64, u64 [size=8, align=4];
// C99-NEXT:         storage i128, u128 [size=16, align=16];
// C99-NEXT:         storage bf16 [size=2, align=2];
// C99-NEXT:         storage f16 [size=2, align=2];
// C99-NEXT:         storage f32 [size=4, align=4];
// C99-NEXT:         storage f64 [size=8, align=4];
// C99-NEXT:         storage f80 [size=12, align=4];
// C99-NEXT:         storage f128 [size=16, align=16];
// C99-NEXT:         storage d32 [size=4, align=4];
// C99-NEXT:         storage d64 [size=8, align=8];
// C99-NEXT:         storage d128 [size=16, align=16];
// C99-NEXT:     }
// C99-NEXT:     global %0 int_max: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(2147483647)) [linkage=external];
// C99-NEXT:     global %1 decimal_above_int_max: i64 [storage=static] = const<i64>(2147483648) [linkage=external];
// C99-NEXT:     global %2 decimal_at_unsigned_long_max: i64 [storage=static] = const<i64>(4294967295) [linkage=external];
// C99-NEXT:     global %3 decimal_above_unsigned_long_max: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// C99-NEXT:     global %4 decimal_at_long_long_max: i64 [storage=static] = const<i64>(9223372036854775807) [linkage=external];
// C99-NEXT:     global %5 decimal_above_long_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(9223372036854775808)) [linkage=external];
// C99-NEXT:     global %6 decimal_at_unsigned_long_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551615)) [linkage=external];
// C99-NEXT:     global %7 octal_above_int_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2147483648))) [linkage=external];
// C99-NEXT:     global %8 octal_above_unsigned_long_max: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// C99-NEXT:     global %9 hex_above_int_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2147483648))) [linkage=external];
// C99-NEXT:     global %10 hex_above_unsigned_long_max: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// C99-NEXT:     global %11 long_suffix_above_long_max: i64 [storage=static] = const<i64>(2147483648) [linkage=external];
// C99-NEXT:     global %12 unsigned_suffix_above_unsigned_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=always>(const<u64>(4294967296)) [linkage=external];
// C99-NEXT:     global %13 long_long_suffix_above_long_max: i64 [storage=static] = const<i64>(2147483648) [linkage=external];
// C99-NEXT: }
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "i686-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=4, align=4];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=4];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=4];
// C23-NEXT:         storage f80 [size=12, align=4];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %0 int_max: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(2147483647)) [linkage=external];
// C23-NEXT:     global %1 decimal_above_int_max: i64 [storage=static] = const<i64>(2147483648) [linkage=external];
// C23-NEXT:     global %2 decimal_at_unsigned_long_max: i64 [storage=static] = const<i64>(4294967295) [linkage=external];
// C23-NEXT:     global %3 decimal_above_unsigned_long_max: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// C23-NEXT:     global %4 decimal_at_long_long_max: i64 [storage=static] = const<i64>(9223372036854775807) [linkage=external];
// C23-NEXT:     global %5 decimal_above_long_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(9223372036854775808)) [linkage=external];
// C23-NEXT:     global %6 decimal_at_unsigned_long_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(const<u64>(18446744073709551615)) [linkage=external];
// C23-NEXT:     global %7 octal_above_int_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2147483648))) [linkage=external];
// C23-NEXT:     global %8 octal_above_unsigned_long_max: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// C23-NEXT:     global %9 hex_above_int_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=unknown>(widen<u64, reason=assign>(const<u32>(2147483648))) [linkage=external];
// C23-NEXT:     global %10 hex_above_unsigned_long_max: i64 [storage=static] = const<i64>(4294967296) [linkage=external];
// C23-NEXT:     global %11 long_suffix_above_long_max: i64 [storage=static] = const<i64>(2147483648) [linkage=external];
// C23-NEXT:     global %12 unsigned_suffix_above_unsigned_long_max: i64 [storage=static] = reinterpret<i64, reason=assign, fits=always>(const<u64>(4294967296)) [linkage=external];
// C23-NEXT:     global %13 long_long_suffix_above_long_max: i64 [storage=static] = const<i64>(2147483648) [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
