/* { dg-additional-options "-fwrapv" } */

#include <limits.h>

extern void abort();

int test3(int x) { return (x + INT_MIN) ^ 0x1234; }

int test4(int x) { return (x ^ 0x1234) + INT_MIN; }

int test5(int x) { return (x - INT_MIN) ^ 0x1234; }

int test6(int x) { return (x ^ 0x1234) - INT_MIN; }

int test9(int x) {
  int y = INT_MIN;
  int z = 0x1234;
  return (x + y) ^ z;
}

int test10(int x) {
  int y = 0x1234;
  int z = INT_MIN;
  return (x ^ y) + z;
}

int test11(int x) {
  int y = INT_MIN;
  int z = 0x1234;
  return (x - y) ^ z;
}

int test12(int x) {
  int y = 0x1234;
  int z = INT_MIN;
  return (x ^ y) - z;
}

void test(int a, int b) {
  if (test3(a) != b)
    abort();
  if (test4(a) != b)
    abort();
  if (test5(a) != b)
    abort();
  if (test6(a) != b)
    abort();
  if (test9(a) != b)
    abort();
  if (test10(a) != b)
    abort();
  if (test11(a) != b)
    abort();
  if (test12(a) != b)
    abort();
}

int main() {
#if INT_MAX == 2147483647
  test(0x00000000, 0x80001234);
  test(0x00001234, 0x80000000);
  test(0x80000000, 0x00001234);
  test(0x80001234, 0x00000000);
  test(0x7fffffff, 0xffffedcb);
  test(0xffffffff, 0x7fffedcb);
#endif

#if INT_MAX == 32767
  test(0x0000, 0x9234);
  test(0x1234, 0x8000);
  test(0x8000, 0x1234);
  test(0x9234, 0x0000);
  test(0x7fff, 0xedcb);
  test(0xffff, 0x6dcb);
#endif

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-additional-options "-fwrapv" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut x: i32) -> i32 {
// CHECK-NEXT:     x.wrapping_add((2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32)) ^ (4660 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4(mut x: i32) -> i32 {
// CHECK-NEXT:     (x ^ (4660 as i32)).wrapping_add((2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test5(mut x: i32) -> i32 {
// CHECK-NEXT:     x.wrapping_sub((2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32)) ^ (4660 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test6(mut x: i32) -> i32 {
// CHECK-NEXT:     (x ^ (4660 as i32)).wrapping_sub((2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test9(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = (2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32);
// CHECK-NEXT:     let mut z: i32 = 4660 as i32;
// CHECK-NEXT:     x.wrapping_add(y) ^ z
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test10(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = 4660 as i32;
// CHECK-NEXT:     let mut z: i32 = (2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32);
// CHECK-NEXT:     (x ^ y).wrapping_add(z)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test11(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = (2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32);
// CHECK-NEXT:     let mut z: i32 = 4660 as i32;
// CHECK-NEXT:     x.wrapping_sub(y) ^ z
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test12(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = 4660 as i32;
// CHECK-NEXT:     let mut z: i32 = (2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32);
// CHECK-NEXT:     (x ^ y).wrapping_sub(z)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test(mut a: i32, mut b: i32) {
// CHECK-NEXT:     if test3(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test9(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test10(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test11(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test12(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test(0 as i32, (2147488308 as u32) as i32);
// CHECK-NEXT:     test(4660 as i32, (2147483648 as u32) as i32);
// CHECK-NEXT:     test((2147483648 as u32) as i32, 4660 as i32);
// CHECK-NEXT:     test((2147488308 as u32) as i32, 0 as i32);
// CHECK-NEXT:     test(2147483647 as i32, (4294962635 as u32) as i32);
// CHECK-NEXT:     test((4294967295 as u32) as i32, 2147478987 as i32);
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
