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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! Copyright (C) 2004 Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test for correctness of composite floating-point comparisons.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Paolo Bonzini, 26th May 2004.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn __builtin_inf() -> f64;
// CHECK-NEXT:     #[link_name = "nan"]
// CHECK-NEXT:     fn __builtin_nan([[V_0:__v[0-9]+]]: *const i8) -> f64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut pinf: f32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut ninf: f32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut NaN: f32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn iuneq(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x == y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if (!(((x < y) as i32) | ((x > y) as i32) != (0 as i32)) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x <= y)
// CHECK-NEXT:         && ((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x >= y)) as i32)
// CHECK-NEXT:         != ok
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ieq(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if ((!((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32))
// CHECK-NEXT:         && ((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x == y)) as i32)
// CHECK-NEXT:         != ok
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn iltgt(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if (!((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x == y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimizable.
// CHECK-NEXT:     if ((x < y) as i32) | ((x > y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Same, __builtin_islessgreater does not trap.
// CHECK-NEXT:     if ((!((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32))
// CHECK-NEXT:         && ((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32)
// CHECK-NEXT:             || x < y
// CHECK-NEXT:             || ((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x > y))) as i32)
// CHECK-NEXT:         != ok
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ine(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32)
// CHECK-NEXT:         || x < y
// CHECK-NEXT:         || ((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x > y)) as i32)
// CHECK-NEXT:         != ok
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn iunlt(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x < y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x < y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ilt(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if ((!((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32))
// CHECK-NEXT:         && ((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x < y)) as i32)
// CHECK-NEXT:         != ok
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     if ((x <= y && x != y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((x <= y && y != x) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((x != y && x <= y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     if ((y != x && x <= y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn iunle(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x <= y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x <= y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ile(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if ((!((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32))
// CHECK-NEXT:         && ((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x <= y)) as i32)
// CHECK-NEXT:         != ok
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     if ((x < y || x == y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((y > x || x == y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((x == y || x < y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     if ((y == x || x < y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn iungt(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x > y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x > y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn igt(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if ((!((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32))
// CHECK-NEXT:         && ((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x > y)) as i32)
// CHECK-NEXT:         != ok
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     if ((x >= y && x != y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((x >= y && y != x) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((x != y && x >= y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     if ((y != x && x >= y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn iunge(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x >= y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if (((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x >= y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ige(mut x: f32, mut y: f32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if ((!((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32))
// CHECK-NEXT:         && ((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x >= y)) as i32)
// CHECK-NEXT:         != ok
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     if ((x > y || x == y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((y < x || x == y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     if ((x == y || x > y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     if ((y == x || x > y) as i32) != ok {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     // Not optimized
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         pinf = f64::INFINITY as f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ninf = -f64::INFINITY as f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         NaN = (unsafe { __builtin_nan(c"".as_ptr() as *const i8) }) as f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     iuneq(unsafe { ninf }, unsafe { pinf }, 0 as i32);
// CHECK-NEXT:     iuneq(unsafe { NaN }, unsafe { NaN }, 1 as i32);
// CHECK-NEXT:     iuneq(unsafe { pinf }, unsafe { ninf }, 0 as i32);
// CHECK-NEXT:     iuneq((1 as i32) as f32, (4 as i32) as f32, 0 as i32);
// CHECK-NEXT:     iuneq((3 as i32) as f32, (3 as i32) as f32, 1 as i32);
// CHECK-NEXT:     iuneq((5 as i32) as f32, (2 as i32) as f32, 0 as i32);
// CHECK-NEXT:     ieq((1 as i32) as f32, (4 as i32) as f32, 0 as i32);
// CHECK-NEXT:     ieq((3 as i32) as f32, (3 as i32) as f32, 1 as i32);
// CHECK-NEXT:     ieq((5 as i32) as f32, (2 as i32) as f32, 0 as i32);
// CHECK-NEXT:     iltgt(unsafe { ninf }, unsafe { pinf }, 1 as i32);
// CHECK-NEXT:     iltgt(unsafe { NaN }, unsafe { NaN }, 0 as i32);
// CHECK-NEXT:     iltgt(unsafe { pinf }, unsafe { ninf }, 1 as i32);
// CHECK-NEXT:     iltgt((1 as i32) as f32, (4 as i32) as f32, 1 as i32);
// CHECK-NEXT:     iltgt((3 as i32) as f32, (3 as i32) as f32, 0 as i32);
// CHECK-NEXT:     iltgt((5 as i32) as f32, (2 as i32) as f32, 1 as i32);
// CHECK-NEXT:     ine((1 as i32) as f32, (4 as i32) as f32, 1 as i32);
// CHECK-NEXT:     ine((3 as i32) as f32, (3 as i32) as f32, 0 as i32);
// CHECK-NEXT:     ine((5 as i32) as f32, (2 as i32) as f32, 1 as i32);
// CHECK-NEXT:     iunlt(unsafe { NaN }, unsafe { ninf }, 1 as i32);
// CHECK-NEXT:     iunlt(unsafe { pinf }, unsafe { NaN }, 1 as i32);
// CHECK-NEXT:     iunlt(unsafe { pinf }, unsafe { ninf }, 0 as i32);
// CHECK-NEXT:     iunlt(unsafe { pinf }, unsafe { pinf }, 0 as i32);
// CHECK-NEXT:     iunlt(unsafe { ninf }, unsafe { ninf }, 0 as i32);
// CHECK-NEXT:     iunlt((1 as i32) as f32, (4 as i32) as f32, 1 as i32);
// CHECK-NEXT:     iunlt((3 as i32) as f32, (3 as i32) as f32, 0 as i32);
// CHECK-NEXT:     iunlt((5 as i32) as f32, (2 as i32) as f32, 0 as i32);
// CHECK-NEXT:     ilt((1 as i32) as f32, (4 as i32) as f32, 1 as i32);
// CHECK-NEXT:     ilt((3 as i32) as f32, (3 as i32) as f32, 0 as i32);
// CHECK-NEXT:     ilt((5 as i32) as f32, (2 as i32) as f32, 0 as i32);
// CHECK-NEXT:     iunle(unsafe { NaN }, unsafe { ninf }, 1 as i32);
// CHECK-NEXT:     iunle(unsafe { pinf }, unsafe { NaN }, 1 as i32);
// CHECK-NEXT:     iunle(unsafe { pinf }, unsafe { ninf }, 0 as i32);
// CHECK-NEXT:     iunle(unsafe { pinf }, unsafe { pinf }, 1 as i32);
// CHECK-NEXT:     iunle(unsafe { ninf }, unsafe { ninf }, 1 as i32);
// CHECK-NEXT:     iunle((1 as i32) as f32, (4 as i32) as f32, 1 as i32);
// CHECK-NEXT:     iunle((3 as i32) as f32, (3 as i32) as f32, 1 as i32);
// CHECK-NEXT:     iunle((5 as i32) as f32, (2 as i32) as f32, 0 as i32);
// CHECK-NEXT:     ile((1 as i32) as f32, (4 as i32) as f32, 1 as i32);
// CHECK-NEXT:     ile((3 as i32) as f32, (3 as i32) as f32, 1 as i32);
// CHECK-NEXT:     ile((5 as i32) as f32, (2 as i32) as f32, 0 as i32);
// CHECK-NEXT:     iungt(unsafe { NaN }, unsafe { ninf }, 1 as i32);
// CHECK-NEXT:     iungt(unsafe { pinf }, unsafe { NaN }, 1 as i32);
// CHECK-NEXT:     iungt(unsafe { pinf }, unsafe { ninf }, 1 as i32);
// CHECK-NEXT:     iungt(unsafe { pinf }, unsafe { pinf }, 0 as i32);
// CHECK-NEXT:     iungt(unsafe { ninf }, unsafe { ninf }, 0 as i32);
// CHECK-NEXT:     iungt((1 as i32) as f32, (4 as i32) as f32, 0 as i32);
// CHECK-NEXT:     iungt((3 as i32) as f32, (3 as i32) as f32, 0 as i32);
// CHECK-NEXT:     iungt((5 as i32) as f32, (2 as i32) as f32, 1 as i32);
// CHECK-NEXT:     igt((1 as i32) as f32, (4 as i32) as f32, 0 as i32);
// CHECK-NEXT:     igt((3 as i32) as f32, (3 as i32) as f32, 0 as i32);
// CHECK-NEXT:     igt((5 as i32) as f32, (2 as i32) as f32, 1 as i32);
// CHECK-NEXT:     iunge(unsafe { NaN }, unsafe { ninf }, 1 as i32);
// CHECK-NEXT:     iunge(unsafe { pinf }, unsafe { NaN }, 1 as i32);
// CHECK-NEXT:     iunge(unsafe { ninf }, unsafe { pinf }, 0 as i32);
// CHECK-NEXT:     iunge(unsafe { pinf }, unsafe { pinf }, 1 as i32);
// CHECK-NEXT:     iunge(unsafe { ninf }, unsafe { ninf }, 1 as i32);
// CHECK-NEXT:     iunge((1 as i32) as f32, (4 as i32) as f32, 0 as i32);
// CHECK-NEXT:     iunge((3 as i32) as f32, (3 as i32) as f32, 1 as i32);
// CHECK-NEXT:     iunge((5 as i32) as f32, (2 as i32) as f32, 1 as i32);
// CHECK-NEXT:     ige((1 as i32) as f32, (4 as i32) as f32, 0 as i32);
// CHECK-NEXT:     ige((3 as i32) as f32, (3 as i32) as f32, 1 as i32);
// CHECK-NEXT:     ige((5 as i32) as f32, (2 as i32) as f32, 1 as i32);
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
