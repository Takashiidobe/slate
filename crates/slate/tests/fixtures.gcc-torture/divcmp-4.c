/* PR middle-end/17894 */

extern void abort(void);

int test1(int x) { return x / -10 == 2; }

int test2(int x) { return x / -10 == 0; }

int test3(int x) { return x / -10 != 2; }

int test4(int x) { return x / -10 != 0; }

int test5(int x) { return x / -10 < 2; }

int test6(int x) { return x / -10 < 0; }

int test7(int x) { return x / -10 <= 2; }

int test8(int x) { return x / -10 <= 0; }

int test9(int x) { return x / -10 > 2; }

int test10(int x) { return x / -10 > 0; }

int test11(int x) { return x / -10 >= 2; }

int test12(int x) { return x / -10 >= 0; }

int main() {
  if (test1(-30) != 0)
    abort();
  if (test1(-29) != 1)
    abort();
  if (test1(-20) != 1)
    abort();
  if (test1(-19) != 0)
    abort();

  if (test2(0) != 1)
    abort();
  if (test2(9) != 1)
    abort();
  if (test2(10) != 0)
    abort();
  if (test2(-1) != 1)
    abort();
  if (test2(-9) != 1)
    abort();
  if (test2(-10) != 0)
    abort();

  if (test3(-30) != 1)
    abort();
  if (test3(-29) != 0)
    abort();
  if (test3(-20) != 0)
    abort();
  if (test3(-19) != 1)
    abort();

  if (test4(0) != 0)
    abort();
  if (test4(9) != 0)
    abort();
  if (test4(10) != 1)
    abort();
  if (test4(-1) != 0)
    abort();
  if (test4(-9) != 0)
    abort();
  if (test4(-10) != 1)
    abort();

  if (test5(-30) != 0)
    abort();
  if (test5(-29) != 0)
    abort();
  if (test5(-20) != 0)
    abort();
  if (test5(-19) != 1)
    abort();

  if (test6(0) != 0)
    abort();
  if (test6(9) != 0)
    abort();
  if (test6(10) != 1)
    abort();
  if (test6(-1) != 0)
    abort();
  if (test6(-9) != 0)
    abort();
  if (test6(-10) != 0)
    abort();

  if (test7(-30) != 0)
    abort();
  if (test7(-29) != 1)
    abort();
  if (test7(-20) != 1)
    abort();
  if (test7(-19) != 1)
    abort();

  if (test8(0) != 1)
    abort();
  if (test8(9) != 1)
    abort();
  if (test8(10) != 1)
    abort();
  if (test8(-1) != 1)
    abort();
  if (test8(-9) != 1)
    abort();
  if (test8(-10) != 0)
    abort();

  if (test9(-30) != 1)
    abort();
  if (test9(-29) != 0)
    abort();
  if (test9(-20) != 0)
    abort();
  if (test9(-19) != 0)
    abort();

  if (test10(0) != 0)
    abort();
  if (test10(9) != 0)
    abort();
  if (test10(10) != 0)
    abort();
  if (test10(-1) != 0)
    abort();
  if (test10(-9) != 0)
    abort();
  if (test10(-10) != 1)
    abort();

  if (test11(-30) != 1)
    abort();
  if (test11(-29) != 1)
    abort();
  if (test11(-20) != 1)
    abort();
  if (test11(-19) != 0)
    abort();

  if (test12(0) != 1)
    abort();
  if (test12(9) != 1)
    abort();
  if (test12(10) != 0)
    abort();
  if (test12(-1) != 1)
    abort();
  if (test12(-9) != 1)
    abort();
  if (test12(-10) != 1)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/17894
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) == (2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) == (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) != (2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) != (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test5(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) < (2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test6(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) < (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test7(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) <= (2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test8(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) <= (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test9(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) > (2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test10(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) > (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test11(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) >= (2 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test12(mut x: i32) -> i32 {
// CHECK-NEXT:     (x / -(10 as i32) >= (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if test1(-(30 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1(-(29 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1(-(20 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1(-(19 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(0 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(9 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(10 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(-(1 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(-(9 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(-(10 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(-(30 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(-(29 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(-(20 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(-(19 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(9 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(10 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(-(1 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(-(9 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(-(10 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(-(30 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(-(29 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(-(20 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(-(19 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(9 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(10 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(-(1 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(-(9 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(-(10 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test7(-(30 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test7(-(29 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test7(-(20 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test7(-(19 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test8(0 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test8(9 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test8(10 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test8(-(1 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test8(-(9 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test8(-(10 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test9(-(30 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test9(-(29 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test9(-(20 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test9(-(19 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test10(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test10(9 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test10(10 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test10(-(1 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test10(-(9 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test10(-(10 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test11(-(30 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test11(-(29 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test11(-(20 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test11(-(19 as i32)) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test12(0 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test12(9 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test12(10 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test12(-(1 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test12(-(9 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test12(-(10 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
