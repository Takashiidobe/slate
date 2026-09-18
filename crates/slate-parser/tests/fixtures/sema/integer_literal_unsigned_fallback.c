unsigned long at_long_max = 9223372036854775807;
unsigned long above_long_max = 9223372036854775808;
unsigned long at_unsigned_long_max = 18446744073709551615;
unsigned long hex_above_long_max = 0x8000000000000000;

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99
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
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     global %0 at_long_max: u64 [storage=static] = reinterpret<u64, reason=assign, fits=always>(const<i64>(9223372036854775807)) [linkage=external];
// C89-NEXT:     global %1 above_long_max: u64 [storage=static] = const<u64>(9223372036854775808) [linkage=external];
// C89-NEXT:     global %2 at_unsigned_long_max: u64 [storage=static] = const<u64>(18446744073709551615) [linkage=external];
// C89-NEXT:     global %3 hex_above_long_max: u64 [storage=static] = const<u64>(9223372036854775808) [linkage=external];
// C89-NEXT: }
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN C99
// C99: module {
// C99-NEXT:     target "x86_64-unknown-linux-gnu" {
// C99-NEXT:         endian = little;
// C99-NEXT:         pointer [size=8, align=8];
// C99-NEXT:         stack_alignment = 16;
// C99-NEXT:         long_double = f80;
// C99-NEXT:         storage bool [size=1, align=1];
// C99-NEXT:         storage i8, u8 [size=1, align=1];
// C99-NEXT:         storage i16, u16 [size=2, align=2];
// C99-NEXT:         storage i32, u32 [size=4, align=4];
// C99-NEXT:         storage i64, u64 [size=8, align=8];
// C99-NEXT:         storage i128, u128 [size=16, align=16];
// C99-NEXT:         storage f16 [size=2, align=2];
// C99-NEXT:         storage f32 [size=4, align=4];
// C99-NEXT:         storage f64 [size=8, align=8];
// C99-NEXT:         storage f80 [size=16, align=16];
// C99-NEXT:         storage f128 [size=16, align=16];
// C99-NEXT:     }
// C99-NEXT:     global %0 at_long_max: u64 [storage=static] = reinterpret<u64, reason=assign, fits=always>(const<i64>(9223372036854775807)) [linkage=external];
// C99-NEXT:     global %1 above_long_max: u64 [storage=static] = const<u64>(9223372036854775808) [linkage=external];
// C99-NEXT:     global %2 at_unsigned_long_max: u64 [storage=static] = const<u64>(18446744073709551615) [linkage=external];
// C99-NEXT:     global %3 hex_above_long_max: u64 [storage=static] = const<u64>(9223372036854775808) [linkage=external];
// C99-NEXT: }
// SLATE-FILECHECK-END C99
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
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %0 at_long_max: u64 [storage=static] = reinterpret<u64, reason=assign, fits=always>(const<i64>(9223372036854775807)) [linkage=external];
// C23-NEXT:     global %1 above_long_max: u64 [storage=static] = const<u64>(9223372036854775808) [linkage=external];
// C23-NEXT:     global %2 at_unsigned_long_max: u64 [storage=static] = const<u64>(18446744073709551615) [linkage=external];
// C23-NEXT:     global %3 hex_above_long_max: u64 [storage=static] = const<u64>(9223372036854775808) [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
