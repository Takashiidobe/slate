// a parameter with leading space whose argument is empty passes the space to
// the next token; an argument that only expands empty does not, as in cl
#define STR(...) #__VA_ARGS__
#define XSTR(...) STR(__VA_ARGS__)
#define E
#define M(fmt, ...) m(fmt, __VA_ARGS__)
#define N(a, b) n(a, b)
#define J(a) j(x a)
#define L(a, b) l(a b)
#define K(a, b) k(a b c)
#define P(a, b) p(a ## b x)

const char variadic_omitted[] = XSTR(M(3));
const char variadic_empty[] = XSTR(M(4,));
const char variadic_expands_empty[] = XSTR(M(5, E));
const char second_empty[] = XSTR(N(1,));
const char second_expands_empty[] = XSTR(N(1, E));
const char first_empty[] = XSTR(N(,2));
const char only_empty[] = XSTR(J());
const char only_expands_empty[] = XSTR(J(E));
const char both_empty[] = XSTR(L(,));
const char before_other_token[] = XSTR(K(1,));
const char paste_operands_empty[] = XSTR(P(,));

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_variadic_omitted:[0-9]+]] variadic_omitted: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([109, 40, 51, 44, 32, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_variadic_empty:[0-9]+]] variadic_empty: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([109, 40, 52, 44, 32, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_variadic_expands_empty:[0-9]+]] variadic_expands_empty: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([109, 40, 53, 44, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_second_empty:[0-9]+]] second_empty: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([110, 40, 49, 44, 32, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_second_expands_empty:[0-9]+]] second_expands_empty: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([110, 40, 49, 44, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_first_empty:[0-9]+]] first_empty: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([110, 40, 44, 32, 50, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_only_empty:[0-9]+]] only_empty: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([106, 40, 120, 32, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_only_expands_empty:[0-9]+]] only_expands_empty: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([106, 40, 120, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_both_empty:[0-9]+]] both_empty: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([108, 40, 32, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_before_other_token:[0-9]+]] before_other_token: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([107, 40, 49, 32, 99, 41, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_paste_operands_empty:[0-9]+]] paste_operands_empty: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([112, 40, 32, 120, 41, 0]) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
