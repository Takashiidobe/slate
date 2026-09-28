// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT

#define T(x) _Generic((x), long: 1, unsigned long: 2, long long: 3, unsigned long long: 4, default: 0)

int u64_max = T(18446744073709551615);
int ll_suffix = T(9223372036854775808LL);
int truncated_decimal = T(99999999999999999999);
int truncated_hex = T(0x1ffffffffffffffff);
int truncated_unsigned = T(99999999999999999999u);
unsigned long long truncated_value = 99999999999999999999;
long long negated = -18446744073709551615;

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
// DEFAULT-NEXT:     global %0 u64_max: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %1 ll_suffix: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %2 truncated_decimal: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 truncated_hex: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %4 truncated_unsigned: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %5 truncated_value: u64 [storage=static] = reinterpret<u64>(const<i64>(7766279631452241919)) [linkage=external];
// DEFAULT-NEXT:     global %6 negated: i64 [storage=static] = truncate<i64>(neg<i128>(const<i128>(18446744073709551615))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
