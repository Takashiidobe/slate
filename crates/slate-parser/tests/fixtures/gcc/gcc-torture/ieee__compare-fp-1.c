/* { dg-do run } */
/* Copyright (C) 2004 Free Software Foundation.

   Test for correctness of composite floating-point comparisons.

   Written by Paolo Bonzini, 26th May 2004.  */

extern void abort(void);

#define TEST(c)                                                                \
  if ((c) != ok)                                                               \
    abort();
#define ORD(a, b)   (!__builtin_isunordered((a), (b)))
#define UNORD(a, b) (__builtin_isunordered((a), (b)))
#define UNEQ(a, b)  (__builtin_isunordered((a), (b)) || ((a) == (b)))
#define UNLT(a, b)  (__builtin_isunordered((a), (b)) || ((a) < (b)))
#define UNLE(a, b)  (__builtin_isunordered((a), (b)) || ((a) <= (b)))
#define UNGT(a, b)  (__builtin_isunordered((a), (b)) || ((a) > (b)))
#define UNGE(a, b)  (__builtin_isunordered((a), (b)) || ((a) >= (b)))
#define LTGT(a, b)  (__builtin_islessgreater((a), (b)))

float pinf;
float ninf;
float NaN;

int iuneq(float x, float y, int ok) {
  TEST(UNEQ(x, y));
  TEST(!LTGT(x, y));
  TEST(UNLE(x, y) && UNGE(x, y));
}

int ieq(float x, float y, int ok) { TEST(ORD(x, y) && UNEQ(x, y)); }

int iltgt(float x, float y, int ok) {
  TEST(!UNEQ(x, y)); /* Not optimizable. */
  TEST(LTGT(x, y));  /* Same, __builtin_islessgreater does not trap. */
  TEST(ORD(x, y) && (UNLT(x, y) || UNGT(x, y)));
}

int ine(float x, float y, int ok) { TEST(UNLT(x, y) || UNGT(x, y)); }

int iunlt(float x, float y, int ok) {
  TEST(UNLT(x, y));
  TEST(UNORD(x, y) || (x < y));
}

int ilt(float x, float y, int ok) {
  TEST(ORD(x, y) && UNLT(x, y)); /* Not optimized */
  TEST((x <= y) && (x != y));
  TEST((x <= y) && (y != x));
  TEST((x != y) && (x <= y)); /* Not optimized */
  TEST((y != x) && (x <= y)); /* Not optimized */
}

int iunle(float x, float y, int ok) {
  TEST(UNLE(x, y));
  TEST(UNORD(x, y) || (x <= y));
}

int ile(float x, float y, int ok) {
  TEST(ORD(x, y) && UNLE(x, y)); /* Not optimized */
  TEST((x < y) || (x == y));
  TEST((y > x) || (x == y));
  TEST((x == y) || (x < y)); /* Not optimized */
  TEST((y == x) || (x < y)); /* Not optimized */
}

int iungt(float x, float y, int ok) {
  TEST(UNGT(x, y));
  TEST(UNORD(x, y) || (x > y));
}

int igt(float x, float y, int ok) {
  TEST(ORD(x, y) && UNGT(x, y)); /* Not optimized */
  TEST((x >= y) && (x != y));
  TEST((x >= y) && (y != x));
  TEST((x != y) && (x >= y)); /* Not optimized */
  TEST((y != x) && (x >= y)); /* Not optimized */
}

int iunge(float x, float y, int ok) {
  TEST(UNGE(x, y));
  TEST(UNORD(x, y) || (x >= y));
}

int ige(float x, float y, int ok) {
  TEST(ORD(x, y) && UNGE(x, y)); /* Not optimized */
  TEST((x > y) || (x == y));
  TEST((y < x) || (x == y));
  TEST((x == y) || (x > y)); /* Not optimized */
  TEST((y == x) || (x > y)); /* Not optimized */
}

int main() {
  pinf = __builtin_inf();
  ninf = -__builtin_inf();
  NaN  = __builtin_nan("");

  iuneq(ninf, pinf, 0);
  iuneq(NaN, NaN, 1);
  iuneq(pinf, ninf, 0);
  iuneq(1, 4, 0);
  iuneq(3, 3, 1);
  iuneq(5, 2, 0);

  ieq(1, 4, 0);
  ieq(3, 3, 1);
  ieq(5, 2, 0);

  iltgt(ninf, pinf, 1);
  iltgt(NaN, NaN, 0);
  iltgt(pinf, ninf, 1);
  iltgt(1, 4, 1);
  iltgt(3, 3, 0);
  iltgt(5, 2, 1);

  ine(1, 4, 1);
  ine(3, 3, 0);
  ine(5, 2, 1);

  iunlt(NaN, ninf, 1);
  iunlt(pinf, NaN, 1);
  iunlt(pinf, ninf, 0);
  iunlt(pinf, pinf, 0);
  iunlt(ninf, ninf, 0);
  iunlt(1, 4, 1);
  iunlt(3, 3, 0);
  iunlt(5, 2, 0);

  ilt(1, 4, 1);
  ilt(3, 3, 0);
  ilt(5, 2, 0);

  iunle(NaN, ninf, 1);
  iunle(pinf, NaN, 1);
  iunle(pinf, ninf, 0);
  iunle(pinf, pinf, 1);
  iunle(ninf, ninf, 1);
  iunle(1, 4, 1);
  iunle(3, 3, 1);
  iunle(5, 2, 0);

  ile(1, 4, 1);
  ile(3, 3, 1);
  ile(5, 2, 0);

  iungt(NaN, ninf, 1);
  iungt(pinf, NaN, 1);
  iungt(pinf, ninf, 1);
  iungt(pinf, pinf, 0);
  iungt(ninf, ninf, 0);
  iungt(1, 4, 0);
  iungt(3, 3, 0);
  iungt(5, 2, 1);

  igt(1, 4, 0);
  igt(3, 3, 0);
  igt(5, 2, 1);

  iunge(NaN, ninf, 1);
  iunge(pinf, NaN, 1);
  iunge(ninf, pinf, 0);
  iunge(pinf, pinf, 1);
  iunge(ninf, ninf, 1);
  iunge(1, 4, 0);
  iunge(3, 3, 1);
  iunge(5, 2, 1);

  ige(1, 4, 0);
  ige(3, 3, 1);
  ige(5, 2, 1);

  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     global %1 pinf: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 ninf: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 NaN: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %56 .str56: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @iuneq(%5 x: f32, %6 y: f32, %7 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%5))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%6)))), const<i32>(0)), eq<f32, exceptions=ignore>(read<f32>(%5), read<f32>(%6)))), read<i32>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(lt<f32, exceptions=ignore>(read<f32>(%5), read<f32>(%6))), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(read<f32>(%5), read<f32>(%6)))), const<i32>(0)))), read<i32>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%5))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%6)))), const<i32>(0)), le<f32, exceptions=ignore>(read<f32>(%5), read<f32>(%6))), logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%5))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%6)))), const<i32>(0)), ge<f32, exceptions=ignore>(read<f32>(%5), read<f32>(%6))))), read<i32>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @ieq(%9 x: f32, %10 y: f32, %11 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%9))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%10)))), const<i32>(0))), logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%9))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%10)))), const<i32>(0)), eq<f32, exceptions=ignore>(read<f32>(%9), read<f32>(%10))))), read<i32>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @iltgt(%13 x: f32, %14 y: f32, %15 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%13))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%14)))), const<i32>(0)), eq<f32, exceptions=ignore>(read<f32>(%13), read<f32>(%14))))), read<i32>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(or<i32>(from_bool<i32, reason=promotion>(lt<f32, exceptions=ignore>(read<f32>(%13), read<f32>(%14))), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(read<f32>(%13), read<f32>(%14)))), read<i32>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%13))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%14)))), const<i32>(0))), logical_or<bool>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%13))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%14)))), const<i32>(0)), lt<f32, exceptions=ignore>(read<f32>(%13), read<f32>(%14))), logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%13))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%14)))), const<i32>(0)), gt<f32, exceptions=ignore>(read<f32>(%13), read<f32>(%14)))))), read<i32>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @ine(%17 x: f32, %18 y: f32, %19 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%17))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%18)))), const<i32>(0)), lt<f32, exceptions=ignore>(read<f32>(%17), read<f32>(%18))), logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%17))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%18)))), const<i32>(0)), gt<f32, exceptions=ignore>(read<f32>(%17), read<f32>(%18))))), read<i32>(%19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @iunlt(%21 x: f32, %22 y: f32, %23 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%21))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%22)))), const<i32>(0)), lt<f32, exceptions=ignore>(read<f32>(%21), read<f32>(%22)))), read<i32>(%23))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%21))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%22)))), const<i32>(0)), lt<f32, exceptions=ignore>(read<f32>(%21), read<f32>(%22)))), read<i32>(%23))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @ilt(%25 x: f32, %26 y: f32, %27 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%25))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%26)))), const<i32>(0))), logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%25))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%26)))), const<i32>(0)), lt<f32, exceptions=ignore>(read<f32>(%25), read<f32>(%26))))), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(le<f32, exceptions=ignore>(read<f32>(%25), read<f32>(%26)), ne<f32, exceptions=ignore>(read<f32>(%25), read<f32>(%26)))), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(le<f32, exceptions=ignore>(read<f32>(%25), read<f32>(%26)), ne<f32, exceptions=ignore>(read<f32>(%26), read<f32>(%25)))), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<f32, exceptions=ignore>(read<f32>(%25), read<f32>(%26)), le<f32, exceptions=ignore>(read<f32>(%25), read<f32>(%26)))), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<f32, exceptions=ignore>(read<f32>(%26), read<f32>(%25)), le<f32, exceptions=ignore>(read<f32>(%25), read<f32>(%26)))), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @iunle(%29 x: f32, %30 y: f32, %31 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%29))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%30)))), const<i32>(0)), le<f32, exceptions=ignore>(read<f32>(%29), read<f32>(%30)))), read<i32>(%31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%29))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%30)))), const<i32>(0)), le<f32, exceptions=ignore>(read<f32>(%29), read<f32>(%30)))), read<i32>(%31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @ile(%33 x: f32, %34 y: f32, %35 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%33))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%34)))), const<i32>(0))), logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%33))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%34)))), const<i32>(0)), le<f32, exceptions=ignore>(read<f32>(%33), read<f32>(%34))))), read<i32>(%35))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(lt<f32, exceptions=ignore>(read<f32>(%33), read<f32>(%34)), eq<f32, exceptions=ignore>(read<f32>(%33), read<f32>(%34)))), read<i32>(%35))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(gt<f32, exceptions=ignore>(read<f32>(%34), read<f32>(%33)), eq<f32, exceptions=ignore>(read<f32>(%33), read<f32>(%34)))), read<i32>(%35))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(eq<f32, exceptions=ignore>(read<f32>(%33), read<f32>(%34)), lt<f32, exceptions=ignore>(read<f32>(%33), read<f32>(%34)))), read<i32>(%35))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(eq<f32, exceptions=ignore>(read<f32>(%34), read<f32>(%33)), lt<f32, exceptions=ignore>(read<f32>(%33), read<f32>(%34)))), read<i32>(%35))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @iungt(%37 x: f32, %38 y: f32, %39 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%37))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%38)))), const<i32>(0)), gt<f32, exceptions=ignore>(read<f32>(%37), read<f32>(%38)))), read<i32>(%39))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%37))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%38)))), const<i32>(0)), gt<f32, exceptions=ignore>(read<f32>(%37), read<f32>(%38)))), read<i32>(%39))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @igt(%41 x: f32, %42 y: f32, %43 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%41))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%42)))), const<i32>(0))), logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%41))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%42)))), const<i32>(0)), gt<f32, exceptions=ignore>(read<f32>(%41), read<f32>(%42))))), read<i32>(%43))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ge<f32, exceptions=ignore>(read<f32>(%41), read<f32>(%42)), ne<f32, exceptions=ignore>(read<f32>(%41), read<f32>(%42)))), read<i32>(%43))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ge<f32, exceptions=ignore>(read<f32>(%41), read<f32>(%42)), ne<f32, exceptions=ignore>(read<f32>(%42), read<f32>(%41)))), read<i32>(%43))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<f32, exceptions=ignore>(read<f32>(%41), read<f32>(%42)), ge<f32, exceptions=ignore>(read<f32>(%41), read<f32>(%42)))), read<i32>(%43))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<f32, exceptions=ignore>(read<f32>(%42), read<f32>(%41)), ge<f32, exceptions=ignore>(read<f32>(%41), read<f32>(%42)))), read<i32>(%43))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @iunge(%45 x: f32, %46 y: f32, %47 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%45))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%46)))), const<i32>(0)), ge<f32, exceptions=ignore>(read<f32>(%45), read<f32>(%46)))), read<i32>(%47))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%45))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%46)))), const<i32>(0)), ge<f32, exceptions=ignore>(read<f32>(%45), read<f32>(%46)))), read<i32>(%47))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @ige(%49 x: f32, %50 y: f32, %51 ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%49))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%50)))), const<i32>(0))), logical_or<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%49))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%50)))), const<i32>(0)), ge<f32, exceptions=ignore>(read<f32>(%49), read<f32>(%50))))), read<i32>(%51))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(gt<f32, exceptions=ignore>(read<f32>(%49), read<f32>(%50)), eq<f32, exceptions=ignore>(read<f32>(%49), read<f32>(%50)))), read<i32>(%51))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(lt<f32, exceptions=ignore>(read<f32>(%50), read<f32>(%49)), eq<f32, exceptions=ignore>(read<f32>(%49), read<f32>(%50)))), read<i32>(%51))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(eq<f32, exceptions=ignore>(read<f32>(%49), read<f32>(%50)), gt<f32, exceptions=ignore>(read<f32>(%49), read<f32>(%50)))), read<i32>(%51))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(eq<f32, exceptions=ignore>(read<f32>(%50), read<f32>(%49)), gt<f32, exceptions=ignore>(read<f32>(%49), read<f32>(%50)))), read<i32>(%51))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @__builtin_inf() -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %55 @__builtin_nan(%54 <unnamed>: ptr<const i8>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %52 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f32>(%1, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn() -> f64>(%53)));
// DEFAULT-NEXT:         float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn() -> f64>(%53));
// DEFAULT-NEXT:         write<f32>(%2, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(call<f64, signature=fn() -> f64>(%53))));
// DEFAULT-NEXT:         float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(call<f64, signature=fn() -> f64>(%53)));
// DEFAULT-NEXT:         write<f32>(%3, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(ptr<const i8>) -> f64>(%55, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%56)))));
// DEFAULT-NEXT:         float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(ptr<const i8>) -> f64>(%55, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%56))));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%4, read<f32>(%2), read<f32>(%1), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%4, read<f32>(%3), read<f32>(%3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%4, read<f32>(%1), read<f32>(%2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%4, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%4, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%4, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%8, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%8, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%8, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%12, read<f32>(%2), read<f32>(%1), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%12, read<f32>(%3), read<f32>(%3), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%12, read<f32>(%1), read<f32>(%2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%12, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%12, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%12, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%16, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%16, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%16, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%20, read<f32>(%3), read<f32>(%2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%20, read<f32>(%1), read<f32>(%3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%20, read<f32>(%1), read<f32>(%2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%20, read<f32>(%1), read<f32>(%1), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%20, read<f32>(%2), read<f32>(%2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%20, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%20, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%20, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%24, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%24, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%24, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%28, read<f32>(%3), read<f32>(%2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%28, read<f32>(%1), read<f32>(%3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%28, read<f32>(%1), read<f32>(%2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%28, read<f32>(%1), read<f32>(%1), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%28, read<f32>(%2), read<f32>(%2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%28, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%28, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%28, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%32, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%32, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%32, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%36, read<f32>(%3), read<f32>(%2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%36, read<f32>(%1), read<f32>(%3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%36, read<f32>(%1), read<f32>(%2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%36, read<f32>(%1), read<f32>(%1), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%36, read<f32>(%2), read<f32>(%2), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%36, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%36, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%36, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%40, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%40, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%40, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%44, read<f32>(%3), read<f32>(%2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%44, read<f32>(%1), read<f32>(%3), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%44, read<f32>(%2), read<f32>(%1), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%44, read<f32>(%1), read<f32>(%1), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%44, read<f32>(%2), read<f32>(%2), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%44, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%44, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%44, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%48, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%48, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%48, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
