// SLATE-FILECHECK-DEFINES DEFAULT

#define S(x) #x
#define T(x) S(x)
#define V(...) #__VA_ARGS__
#define N (-307)
#define P(x) x
#define Q a b

const char *adjacent = S((-307));
const char *operator_ = S(a+b);
const char *spaced = S(  a   +   b  );
const char *comment = S(a/**/b);
const char *literals = S("q\n" 'x');
const char *expanded = T(N);
const char *variadic_tight = V(a,b);
const char *variadic_space = V(a, b);
const char *variadic_wide = V(a , b);
const char *variadic_before = V(a ,b);
const char *variadic_after = V(a,  b);
const char *expansion_edge = T(P(1)+2);
const char *expansion_inner = T(Q+1);
const char *expansion_prefix = T(-P(1));

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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([40, 45, 51, 48, 55, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_adjacent:[0-9]+]] adjacent: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 43, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_operator_:[0-9]+]] operator_: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 32, 43, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_spaced:[0-9]+]] spaced: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_3]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_comment:[0-9]+]] comment: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([34, 113, 92, 110, 34, 32, 39, 120, 39, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_literals:[0-9]+]] literals: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_5]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([40, 45, 51, 48, 55, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_expanded:[0-9]+]] expanded: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_6]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 44, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_variadic_tight:[0-9]+]] variadic_tight: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_7]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 44, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_variadic_space:[0-9]+]] variadic_space: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_8]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 32, 44, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_variadic_wide:[0-9]+]] variadic_wide: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_9]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 32, 44, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_variadic_before:[0-9]+]] variadic_before: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_10]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 44, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_variadic_after:[0-9]+]] variadic_after: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_11]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 43, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_expansion_edge:[0-9]+]] expansion_edge: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_12]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 32, 98, 43, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_expansion_inner:[0-9]+]] expansion_inner: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_13]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([45, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_expansion_prefix:[0-9]+]] expansion_prefix: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_14]])) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
