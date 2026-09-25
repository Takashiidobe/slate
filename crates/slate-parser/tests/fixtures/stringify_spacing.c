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
// DEFAULT-NEXT:     global %14 .str14: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([40, 45, 51, 48, 55, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %0 adjacent: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%14)) [linkage=external];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 43, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %1 operator_: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%15)) [linkage=external];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 32, 43, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %2 spaced: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%16)) [linkage=external];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %3 comment: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%17)) [linkage=external];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([34, 113, 92, 110, 34, 32, 39, 120, 39, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %4 literals: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%18)) [linkage=external];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([40, 45, 51, 48, 55, 41, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %5 expanded: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%19)) [linkage=external];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 44, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %6 variadic_tight: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%20)) [linkage=external];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 44, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 variadic_space: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%21)) [linkage=external];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 32, 44, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 variadic_wide: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%22)) [linkage=external];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 32, 44, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 variadic_before: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%23)) [linkage=external];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 44, 32, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 variadic_after: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%24)) [linkage=external];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 43, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 expansion_edge: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%25)) [linkage=external];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 32, 98, 43, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 expansion_inner: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%26)) [linkage=external];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([45, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 expansion_prefix: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%27)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
