/* { dg-additional-options "-fwrapv" } */

#include <limits.h>

extern void abort();

int test2(int x) { return x + INT_MIN; }

int test3(int x) { return x - INT_MIN; }

int test5(int x) {
  int y = INT_MIN;
  return x + y;
}

int test6(int x) {
  int y = INT_MIN;
  return x - y;
}

void test(int a, int b) {
  if (test2(a) != b)
    abort();
  if (test3(a) != b)
    abort();
  if (test5(a) != b)
    abort();
  if (test6(a) != b)
    abort();
}

int main() {
#if INT_MAX == 2147483647
  test(0x00000000, 0x80000000);
  test(0x80000000, 0x00000000);
  test(0x12345678, 0x92345678);
  test(0x92345678, 0x12345678);
  test(0x7fffffff, 0xffffffff);
  test(0xffffffff, 0x7fffffff);
#endif

#if INT_MAX == 32767
  test(0x0000, 0x8000);
  test(0x8000, 0x0000);
  test(0x1234, 0x9234);
  test(0x9234, 0x1234);
  test(0x7fff, 0xffff);
  test(0xffff, 0x7fff);
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
// CHECK-NEXT: fn test2(mut x: i32) -> i32 {
// CHECK-NEXT:     x.wrapping_add((2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut x: i32) -> i32 {
// CHECK-NEXT:     x.wrapping_sub((2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test5(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = (2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32);
// CHECK-NEXT:     x.wrapping_add(y)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test6(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = (2147483647 as i32).wrapping_neg().wrapping_sub(1 as i32);
// CHECK-NEXT:     x.wrapping_sub(y)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test(mut a: i32, mut b: i32) {
// CHECK-NEXT:     if test2(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test(0 as i32, (2147483648 as u32) as i32);
// CHECK-NEXT:     test((2147483648 as u32) as i32, 0 as i32);
// CHECK-NEXT:     test(305419896 as i32, (2452903544 as u32) as i32);
// CHECK-NEXT:     test((2452903544 as u32) as i32, 305419896 as i32);
// CHECK-NEXT:     test(2147483647 as i32, (4294967295 as u32) as i32);
// CHECK-NEXT:     test((4294967295 as u32) as i32, 2147483647 as i32);
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
