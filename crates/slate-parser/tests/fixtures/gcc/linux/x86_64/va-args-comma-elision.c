// the comma before ## __VA_ARGS__ goes when the variadic argument is omitted
// (and for H() only in gnu modes); msvc drops it before any empty __VA_ARGS__
#define STR(...) #__VA_ARGS__
#define XSTR(...) STR(__VA_ARGS__)
#define E
#define F(msg, ...) f(#msg, ##__VA_ARGS__)
#define H(...) h(0, ##__VA_ARGS__)
#define M(fmt, ...) m(fmt, __VA_ARGS__)
#define P(fmt, ...) m(fmt, ##__VA_ARGS__, 9)

const char omitted[] = XSTR(F(a));
const char empty[] = XSTR(F(b,));
const char empty_after_expansion[] = XSTR(F(d, E));
const char present[] = XSTR(F(c, 1));
const char only_variadic_omitted[] = XSTR(H());
const char only_variadic_present[] = XSTR(H(1));
const char middle[] = XSTR(P(2));
const char plain_omitted[] = XSTR(M(3));
const char plain_empty[] = XSTR(M(4,));

// SLATE-FILECHECK-DEFINES GNU
// SLATE-FILECHECK-STD GNU gnu17
// SLATE-FILECHECK-DEFINES ISO
// SLATE-FILECHECK-STD ISO c17

// SLATE-FILECHECK-BEGIN GNU
// GNU: module {
// GNU-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU-NEXT:         endian = little;
// GNU-NEXT:         pointer [size=8, align=8];
// GNU-NEXT:         stack_alignment = 16;
// GNU-NEXT:         long_double = f80;
// GNU-NEXT:         storage bool [size=1, align=1];
// GNU-NEXT:         storage i8, u8 [size=1, align=1];
// GNU-NEXT:         storage i16, u16 [size=2, align=2];
// GNU-NEXT:         storage i32, u32 [size=4, align=4];
// GNU-NEXT:         storage i64, u64 [size=8, align=8];
// GNU-NEXT:         storage i128, u128 [size=16, align=16];
// GNU-NEXT:         storage bf16 [size=2, align=2];
// GNU-NEXT:         storage f16 [size=2, align=2];
// GNU-NEXT:         storage f32 [size=4, align=4];
// GNU-NEXT:         storage f64 [size=8, align=8];
// GNU-NEXT:         storage f80 [size=16, align=16];
// GNU-NEXT:         storage f128 [size=16, align=16];
// GNU-NEXT:         storage d32 [size=4, align=4];
// GNU-NEXT:         storage d64 [size=8, align=8];
// GNU-NEXT:         storage d128 [size=16, align=16];
// GNU-NEXT:     }
// GNU-NEXT:     global %[[VALUE_omitted:[0-9]+]] omitted: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([102, 40, 34, 97, 34, 41, 0]) [linkage=external];
// GNU-NEXT:     global %[[VALUE_empty:[0-9]+]] empty: array<i8, 8> [storage=static] [const] = code_units<array<i8, 8>>([102, 40, 34, 98, 34, 44, 41, 0]) [linkage=external];
// GNU-NEXT:     global %[[VALUE_empty_after_expansion:[0-9]+]] empty_after_expansion: array<i8, 8> [storage=static] [const] = code_units<array<i8, 8>>([102, 40, 34, 100, 34, 44, 41, 0]) [linkage=external];
// GNU-NEXT:     global %[[VALUE_present:[0-9]+]] present: array<i8, 9> [storage=static] [const] = code_units<array<i8, 9>>([102, 40, 34, 99, 34, 44, 49, 41, 0]) [linkage=external];
// GNU-NEXT:     global %[[VALUE_only_variadic_omitted:[0-9]+]] only_variadic_omitted: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([104, 40, 48, 41, 0]) [linkage=external];
// GNU-NEXT:     global %[[VALUE_only_variadic_present:[0-9]+]] only_variadic_present: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([104, 40, 48, 44, 49, 41, 0]) [linkage=external];
// GNU-NEXT:     global %[[VALUE_middle:[0-9]+]] middle: array<i8, 8> [storage=static] [const] = code_units<array<i8, 8>>([109, 40, 50, 44, 32, 57, 41, 0]) [linkage=external];
// GNU-NEXT:     global %[[VALUE_plain_omitted:[0-9]+]] plain_omitted: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([109, 40, 51, 44, 41, 0]) [linkage=external];
// GNU-NEXT:     global %[[VALUE_plain_empty:[0-9]+]] plain_empty: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([109, 40, 52, 44, 41, 0]) [linkage=external];
// GNU-NEXT: }
// SLATE-FILECHECK-END GNU
// SLATE-FILECHECK-BEGIN ISO
// ISO: module {
// ISO-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO-NEXT:         endian = little;
// ISO-NEXT:         pointer [size=8, align=8];
// ISO-NEXT:         stack_alignment = 16;
// ISO-NEXT:         long_double = f80;
// ISO-NEXT:         storage bool [size=1, align=1];
// ISO-NEXT:         storage i8, u8 [size=1, align=1];
// ISO-NEXT:         storage i16, u16 [size=2, align=2];
// ISO-NEXT:         storage i32, u32 [size=4, align=4];
// ISO-NEXT:         storage i64, u64 [size=8, align=8];
// ISO-NEXT:         storage i128, u128 [size=16, align=16];
// ISO-NEXT:         storage bf16 [size=2, align=2];
// ISO-NEXT:         storage f16 [size=2, align=2];
// ISO-NEXT:         storage f32 [size=4, align=4];
// ISO-NEXT:         storage f64 [size=8, align=8];
// ISO-NEXT:         storage f80 [size=16, align=16];
// ISO-NEXT:         storage f128 [size=16, align=16];
// ISO-NEXT:         storage d32 [size=4, align=4];
// ISO-NEXT:         storage d64 [size=8, align=8];
// ISO-NEXT:         storage d128 [size=16, align=16];
// ISO-NEXT:     }
// ISO-NEXT:     global %[[VALUE_omitted:[0-9]+]] omitted: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([102, 40, 34, 97, 34, 41, 0]) [linkage=external];
// ISO-NEXT:     global %[[VALUE_empty:[0-9]+]] empty: array<i8, 8> [storage=static] [const] = code_units<array<i8, 8>>([102, 40, 34, 98, 34, 44, 41, 0]) [linkage=external];
// ISO-NEXT:     global %[[VALUE_empty_after_expansion:[0-9]+]] empty_after_expansion: array<i8, 8> [storage=static] [const] = code_units<array<i8, 8>>([102, 40, 34, 100, 34, 44, 41, 0]) [linkage=external];
// ISO-NEXT:     global %[[VALUE_present:[0-9]+]] present: array<i8, 9> [storage=static] [const] = code_units<array<i8, 9>>([102, 40, 34, 99, 34, 44, 49, 41, 0]) [linkage=external];
// ISO-NEXT:     global %[[VALUE_only_variadic_omitted:[0-9]+]] only_variadic_omitted: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([104, 40, 48, 44, 41, 0]) [linkage=external];
// ISO-NEXT:     global %[[VALUE_only_variadic_present:[0-9]+]] only_variadic_present: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([104, 40, 48, 44, 49, 41, 0]) [linkage=external];
// ISO-NEXT:     global %[[VALUE_middle:[0-9]+]] middle: array<i8, 8> [storage=static] [const] = code_units<array<i8, 8>>([109, 40, 50, 44, 32, 57, 41, 0]) [linkage=external];
// ISO-NEXT:     global %[[VALUE_plain_omitted:[0-9]+]] plain_omitted: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([109, 40, 51, 44, 41, 0]) [linkage=external];
// ISO-NEXT:     global %[[VALUE_plain_empty:[0-9]+]] plain_empty: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([109, 40, 52, 44, 41, 0]) [linkage=external];
// ISO-NEXT: }
// SLATE-FILECHECK-END ISO
