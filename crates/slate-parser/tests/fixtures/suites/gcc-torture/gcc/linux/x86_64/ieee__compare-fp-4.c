/* { dg-do run }
   { dg-additional-options "-fno-trapping-math" }

   # The ARM VxWorks kernel uses an external floating-point library in
   # which routines like __ledf2 are just aliases for __cmpdf2.  These
   # routines therefore don't handle NaNs correctly.
   { dg-xfail-if "" { arm*-*-vxworks* } } */

/* Copyright (C) 2004 Free Software Foundation.

   Test for correctness of composite floating-point comparisons.

   Written by Paolo Bonzini, 26th May 2004.  */

extern void abort(void);

#define TEST(c)                                                                \
  if ((c) != ok)                                                               \
    abort();
#define ORD(a, b)   (((a) < (b)) || (a) >= (b))
#define UNORD(a, b) (!ORD((a), (b)))
#define UNEQ(a, b)  (!LTGT((a), (b)))
#define UNLT(a, b)  (((a) < (b)) || __builtin_isunordered((a), (b)))
#define UNLE(a, b)  (((a) <= (b)) || __builtin_isunordered((a), (b)))
#define UNGT(a, b)  (((a) > (b)) || __builtin_isunordered((a), (b)))
#define UNGE(a, b)  (((a) >= (b)) || __builtin_isunordered((a), (b)))
#define LTGT(a, b)  (((a) < (b)) || (a) > (b))

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
  TEST(!UNEQ(x, y));
  TEST(LTGT(x, y));
  TEST(ORD(x, y) && (UNLT(x, y) || UNGT(x, y)));
}

int ine(float x, float y, int ok) {
  TEST(UNLT(x, y) || UNGT(x, y));
  TEST((x < y) || (x > y) || UNORD(x, y));
}

int iunlt(float x, float y, int ok) {
  TEST(UNLT(x, y));
  TEST(UNORD(x, y) || (x < y));
}

int ilt(float x, float y, int ok) {
  TEST(ORD(x, y) && UNLT(x, y));
  TEST((x <= y) && (x != y));
  TEST((x <= y) && (y != x));
  TEST((x != y) && (x <= y));
  TEST((y != x) && (x <= y));
}

int iunle(float x, float y, int ok) {
  TEST(UNLE(x, y));
  TEST(UNORD(x, y) || (x <= y));
}

int ile(float x, float y, int ok) {
  TEST(ORD(x, y) && UNLE(x, y));
  TEST((x < y) || (x == y));
  TEST((y > x) || (x == y));
  TEST((x == y) || (x < y));
  TEST((y == x) || (x < y));
}

int iungt(float x, float y, int ok) {
  TEST(UNGT(x, y));
  TEST(UNORD(x, y) || (x > y));
}

int igt(float x, float y, int ok) {
  TEST(ORD(x, y) && UNGT(x, y));
  TEST((x >= y) && (x != y));
  TEST((x >= y) && (y != x));
  TEST((x != y) && (x >= y));
  TEST((y != x) && (x >= y));
}

int iunge(float x, float y, int ok) {
  TEST(UNGE(x, y));
  TEST(UNORD(x, y) || (x >= y));
}

int ige(float x, float y, int ok) {
  TEST(ORD(x, y) && UNGE(x, y));
  TEST((x > y) || (x == y));
  TEST((y < x) || (x == y));
  TEST((x == y) || (x > y));
  TEST((y == x) || (x > y));
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
// DEFAULT-NEXT:     global %[[VALUE_pinf:[0-9]+]] pinf: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ninf:[0-9]+]] ninf: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_NaN:[0-9]+]] NaN: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_iuneq:[0-9]+]] @iuneq(%[[VALUE_x:[0-9]+]] x: f32, %[[VALUE_y:[0-9]+]] y: f32, %[[VALUE_ok:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]])), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]]))))), read<i32>(%[[VALUE_ok]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]])), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]]))))), read<i32>(%[[VALUE_ok]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(logical_or<bool>(le<f32, exceptions=observable>(read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y]])))), const<i32>(0))), logical_or<bool>(ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y]])))), const<i32>(0))))), read<i32>(%[[VALUE_ok]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ieq:[0-9]+]] @ieq(%[[VALUE_x_2:[0-9]+]] x: f32, %[[VALUE_y_2:[0-9]+]] y: f32, %[[VALUE_ok_2:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_2]]), read<f32>(%[[VALUE_y_2]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_2]]), read<f32>(%[[VALUE_y_2]]))), not<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_2]]), read<f32>(%[[VALUE_y_2]])), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_2]]), read<f32>(%[[VALUE_y_2]])))))), read<i32>(%[[VALUE_ok_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_iltgt:[0-9]+]] @iltgt(%[[VALUE_x_3:[0-9]+]] x: f32, %[[VALUE_y_3:[0-9]+]] y: f32, %[[VALUE_ok_3:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(not<bool>(not<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), read<f32>(%[[VALUE_y_3]])), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), read<f32>(%[[VALUE_y_3]])))))), read<i32>(%[[VALUE_ok_3]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), read<f32>(%[[VALUE_y_3]])), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), read<f32>(%[[VALUE_y_3]])))), read<i32>(%[[VALUE_ok_3]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), read<f32>(%[[VALUE_y_3]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), read<f32>(%[[VALUE_y_3]]))), logical_or<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), read<f32>(%[[VALUE_y_3]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_3]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_3]])))), const<i32>(0))), logical_or<bool>(gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), read<f32>(%[[VALUE_y_3]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_3]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_3]])))), const<i32>(0)))))), read<i32>(%[[VALUE_ok_3]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ine:[0-9]+]] @ine(%[[VALUE_x_4:[0-9]+]] x: f32, %[[VALUE_y_4:[0-9]+]] y: f32, %[[VALUE_ok_4:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_4]]), read<f32>(%[[VALUE_y_4]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_4]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_4]])))), const<i32>(0))), logical_or<bool>(gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_4]]), read<f32>(%[[VALUE_y_4]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_4]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_4]])))), const<i32>(0))))), read<i32>(%[[VALUE_ok_4]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_4]]), read<f32>(%[[VALUE_y_4]])), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_4]]), read<f32>(%[[VALUE_y_4]]))), not<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_4]]), read<f32>(%[[VALUE_y_4]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_4]]), read<f32>(%[[VALUE_y_4]])))))), read<i32>(%[[VALUE_ok_4]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_iunlt:[0-9]+]] @iunlt(%[[VALUE_x_5:[0-9]+]] x: f32, %[[VALUE_y_5:[0-9]+]] y: f32, %[[VALUE_ok_5:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_5]]), read<f32>(%[[VALUE_y_5]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_5]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_5]])))), const<i32>(0)))), read<i32>(%[[VALUE_ok_5]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(not<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_5]]), read<f32>(%[[VALUE_y_5]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_5]]), read<f32>(%[[VALUE_y_5]])))), lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_5]]), read<f32>(%[[VALUE_y_5]])))), read<i32>(%[[VALUE_ok_5]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ilt:[0-9]+]] @ilt(%[[VALUE_x_6:[0-9]+]] x: f32, %[[VALUE_y_6:[0-9]+]] y: f32, %[[VALUE_ok_6:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]]))), logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_6]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_6]])))), const<i32>(0))))), read<i32>(%[[VALUE_ok_6]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(le<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]])), ne<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]])))), read<i32>(%[[VALUE_ok_6]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(le<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]])), ne<f32, exceptions=observable>(read<f32>(%[[VALUE_y_6]]), read<f32>(%[[VALUE_x_6]])))), read<i32>(%[[VALUE_ok_6]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]])), le<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]])))), read<i32>(%[[VALUE_ok_6]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<f32, exceptions=observable>(read<f32>(%[[VALUE_y_6]]), read<f32>(%[[VALUE_x_6]])), le<f32, exceptions=observable>(read<f32>(%[[VALUE_x_6]]), read<f32>(%[[VALUE_y_6]])))), read<i32>(%[[VALUE_ok_6]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_iunle:[0-9]+]] @iunle(%[[VALUE_x_7:[0-9]+]] x: f32, %[[VALUE_y_7:[0-9]+]] y: f32, %[[VALUE_ok_7:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(le<f32, exceptions=observable>(read<f32>(%[[VALUE_x_7]]), read<f32>(%[[VALUE_y_7]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_7]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_7]])))), const<i32>(0)))), read<i32>(%[[VALUE_ok_7]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(not<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_7]]), read<f32>(%[[VALUE_y_7]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_7]]), read<f32>(%[[VALUE_y_7]])))), le<f32, exceptions=observable>(read<f32>(%[[VALUE_x_7]]), read<f32>(%[[VALUE_y_7]])))), read<i32>(%[[VALUE_ok_7]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ile:[0-9]+]] @ile(%[[VALUE_x_8:[0-9]+]] x: f32, %[[VALUE_y_8:[0-9]+]] y: f32, %[[VALUE_ok_8:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]]))), logical_or<bool>(le<f32, exceptions=observable>(read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_8]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_8]])))), const<i32>(0))))), read<i32>(%[[VALUE_ok_8]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]])), eq<f32, exceptions=observable>(read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]])))), read<i32>(%[[VALUE_ok_8]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(gt<f32, exceptions=observable>(read<f32>(%[[VALUE_y_8]]), read<f32>(%[[VALUE_x_8]])), eq<f32, exceptions=observable>(read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]])))), read<i32>(%[[VALUE_ok_8]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(eq<f32, exceptions=observable>(read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]])), lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]])))), read<i32>(%[[VALUE_ok_8]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(eq<f32, exceptions=observable>(read<f32>(%[[VALUE_y_8]]), read<f32>(%[[VALUE_x_8]])), lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_8]]), read<f32>(%[[VALUE_y_8]])))), read<i32>(%[[VALUE_ok_8]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_iungt:[0-9]+]] @iungt(%[[VALUE_x_9:[0-9]+]] x: f32, %[[VALUE_y_9:[0-9]+]] y: f32, %[[VALUE_ok_9:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_9]]), read<f32>(%[[VALUE_y_9]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_9]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_9]])))), const<i32>(0)))), read<i32>(%[[VALUE_ok_9]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(not<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_9]]), read<f32>(%[[VALUE_y_9]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_9]]), read<f32>(%[[VALUE_y_9]])))), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_9]]), read<f32>(%[[VALUE_y_9]])))), read<i32>(%[[VALUE_ok_9]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_igt:[0-9]+]] @igt(%[[VALUE_x_10:[0-9]+]] x: f32, %[[VALUE_y_10:[0-9]+]] y: f32, %[[VALUE_ok_10:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]]))), logical_or<bool>(gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_10]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_10]])))), const<i32>(0))))), read<i32>(%[[VALUE_ok_10]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]])), ne<f32, exceptions=observable>(read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]])))), read<i32>(%[[VALUE_ok_10]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]])), ne<f32, exceptions=observable>(read<f32>(%[[VALUE_y_10]]), read<f32>(%[[VALUE_x_10]])))), read<i32>(%[[VALUE_ok_10]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<f32, exceptions=observable>(read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]])))), read<i32>(%[[VALUE_ok_10]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<f32, exceptions=observable>(read<f32>(%[[VALUE_y_10]]), read<f32>(%[[VALUE_x_10]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_10]]), read<f32>(%[[VALUE_y_10]])))), read<i32>(%[[VALUE_ok_10]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_iunge:[0-9]+]] @iunge(%[[VALUE_x_11:[0-9]+]] x: f32, %[[VALUE_y_11:[0-9]+]] y: f32, %[[VALUE_ok_11:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_11]]), read<f32>(%[[VALUE_y_11]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_11]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_11]])))), const<i32>(0)))), read<i32>(%[[VALUE_ok_11]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(not<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_11]]), read<f32>(%[[VALUE_y_11]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_11]]), read<f32>(%[[VALUE_y_11]])))), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_11]]), read<f32>(%[[VALUE_y_11]])))), read<i32>(%[[VALUE_ok_11]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ige:[0-9]+]] @ige(%[[VALUE_x_12:[0-9]+]] x: f32, %[[VALUE_y_12:[0-9]+]] y: f32, %[[VALUE_ok_12:[0-9]+]] ok: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_12]]), read<f32>(%[[VALUE_y_12]])), ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_12]]), read<f32>(%[[VALUE_y_12]]))), logical_or<bool>(ge<f32, exceptions=observable>(read<f32>(%[[VALUE_x_12]]), read<f32>(%[[VALUE_y_12]])), ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_x_12]]))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(read<f32>(%[[VALUE_y_12]])))), const<i32>(0))))), read<i32>(%[[VALUE_ok_12]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_12]]), read<f32>(%[[VALUE_y_12]])), eq<f32, exceptions=observable>(read<f32>(%[[VALUE_x_12]]), read<f32>(%[[VALUE_y_12]])))), read<i32>(%[[VALUE_ok_12]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_y_12]]), read<f32>(%[[VALUE_x_12]])), eq<f32, exceptions=observable>(read<f32>(%[[VALUE_x_12]]), read<f32>(%[[VALUE_y_12]])))), read<i32>(%[[VALUE_ok_12]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(eq<f32, exceptions=observable>(read<f32>(%[[VALUE_x_12]]), read<f32>(%[[VALUE_y_12]])), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_12]]), read<f32>(%[[VALUE_y_12]])))), read<i32>(%[[VALUE_ok_12]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         if ne<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(eq<f32, exceptions=observable>(read<f32>(%[[VALUE_y_12]]), read<f32>(%[[VALUE_x_12]])), gt<f32, exceptions=observable>(read<f32>(%[[VALUE_x_12]]), read<f32>(%[[VALUE_y_12]])))), read<i32>(%[[VALUE_ok_12]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf:[0-9]+]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan:[0-9]+]] @__builtin_nan(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f32>(%[[VALUE_pinf]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_ninf]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_NaN]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])))));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iuneq]], read<f32>(%[[VALUE_ninf]]), read<f32>(%[[VALUE_pinf]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iuneq]], read<f32>(%[[VALUE_NaN]]), read<f32>(%[[VALUE_NaN]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iuneq]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_ninf]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iuneq]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iuneq]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iuneq]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ieq]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ieq]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ieq]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iltgt]], read<f32>(%[[VALUE_ninf]]), read<f32>(%[[VALUE_pinf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iltgt]], read<f32>(%[[VALUE_NaN]]), read<f32>(%[[VALUE_NaN]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iltgt]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_ninf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iltgt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iltgt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iltgt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ine]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ine]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ine]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunlt]], read<f32>(%[[VALUE_NaN]]), read<f32>(%[[VALUE_ninf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunlt]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_NaN]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunlt]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_ninf]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunlt]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_pinf]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunlt]], read<f32>(%[[VALUE_ninf]]), read<f32>(%[[VALUE_ninf]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunlt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunlt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunlt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ilt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ilt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ilt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunle]], read<f32>(%[[VALUE_NaN]]), read<f32>(%[[VALUE_ninf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunle]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_NaN]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunle]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_ninf]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunle]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_pinf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunle]], read<f32>(%[[VALUE_ninf]]), read<f32>(%[[VALUE_ninf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunle]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunle]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunle]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ile]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ile]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ile]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iungt]], read<f32>(%[[VALUE_NaN]]), read<f32>(%[[VALUE_ninf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iungt]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_NaN]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iungt]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_ninf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iungt]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_pinf]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iungt]], read<f32>(%[[VALUE_ninf]]), read<f32>(%[[VALUE_ninf]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iungt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iungt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iungt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_igt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_igt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_igt]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunge]], read<f32>(%[[VALUE_NaN]]), read<f32>(%[[VALUE_ninf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunge]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_NaN]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunge]], read<f32>(%[[VALUE_ninf]]), read<f32>(%[[VALUE_pinf]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunge]], read<f32>(%[[VALUE_pinf]]), read<f32>(%[[VALUE_pinf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunge]], read<f32>(%[[VALUE_ninf]]), read<f32>(%[[VALUE_ninf]]), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunge]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunge]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_iunge]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ige]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ige]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(f32, f32, i32) -> i32>(%[[VALUE_ige]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
