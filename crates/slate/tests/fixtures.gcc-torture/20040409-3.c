#include <limits.h>

extern void abort();

int test1(int x) { return ~(x ^ INT_MIN); }

unsigned int test1u(unsigned int x) { return ~(x ^ (unsigned int)INT_MIN); }

unsigned int test2u(unsigned int x) { return ~(x + (unsigned int)INT_MIN); }

unsigned int test3u(unsigned int x) { return ~(x - (unsigned int)INT_MIN); }

int test4(int x) {
  int y = INT_MIN;
  return ~(x ^ y);
}

unsigned int test4u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  return ~(x ^ y);
}

unsigned int test5u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  return ~(x + y);
}

unsigned int test6u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  return ~(x - y);
}

void test(int a, int b) {
  if (test1(a) != b)
    abort();
  if (test4(a) != b)
    abort();
}

void testu(unsigned int a, unsigned int b) {
  if (test1u(a) != b)
    abort();
  if (test2u(a) != b)
    abort();
  if (test3u(a) != b)
    abort();
  if (test4u(a) != b)
    abort();
  if (test5u(a) != b)
    abort();
  if (test6u(a) != b)
    abort();
}

int main() {
#if INT_MAX == 2147483647
  test(0x00000000, 0x7fffffff);
  test(0x80000000, 0xffffffff);
  test(0x12345678, 0x6dcba987);
  test(0x92345678, 0xedcba987);
  test(0x7fffffff, 0x00000000);
  test(0xffffffff, 0x80000000);

  testu(0x00000000, 0x7fffffff);
  testu(0x80000000, 0xffffffff);
  testu(0x12345678, 0x6dcba987);
  testu(0x92345678, 0xedcba987);
  testu(0x7fffffff, 0x00000000);
  testu(0xffffffff, 0x80000000);
#endif

#if INT_MAX == 32767
  test(0x0000, 0x7fff);
  test(0x8000, 0xffff);
  test(0x1234, 0x6dcb);
  test(0x9234, 0xedcb);
  test(0x7fff, 0x0000);
  test(0xffff, 0x8000);

  testu(0x0000, 0x7fff);
  testu(0x8000, 0xffff);
  testu(0x1234, 0x6dcb);
  testu(0x9234, 0xedcb);
  testu(0x7fff, 0x0000);
  testu(0xffff, 0x8000);
#endif

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn test1(mut x: i32) -> i32 {
// CHECK-NEXT:     !(x ^ -(2147483647 as i32) - (1 as i32))
// CHECK-NEXT: }
// CHECK-NEXT: fn test1u(mut x: u32) -> u32 {
// CHECK-NEXT:     !(x ^ ((-(2147483647 as i32) - (1 as i32)) as u32))
// CHECK-NEXT: }
// CHECK-NEXT: fn test2u(mut x: u32) -> u32 {
// CHECK-NEXT:     !x.wrapping_add((-(2147483647 as i32) - (1 as i32)) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test3u(mut x: u32) -> u32 {
// CHECK-NEXT:     !x.wrapping_sub((-(2147483647 as i32) - (1 as i32)) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test4(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = -(2147483647 as i32) - (1 as i32);
// CHECK-NEXT:     !(x ^ y)
// CHECK-NEXT: }
// CHECK-NEXT: fn test4u(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: u32 = (-(2147483647 as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     !(x ^ y)
// CHECK-NEXT: }
// CHECK-NEXT: fn test5u(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: u32 = (-(2147483647 as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     !x.wrapping_add(y)
// CHECK-NEXT: }
// CHECK-NEXT: fn test6u(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: u32 = (-(2147483647 as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     !x.wrapping_sub(y)
// CHECK-NEXT: }
// CHECK-NEXT: fn test(mut a: i32, mut b: i32) {
// CHECK-NEXT:     if test1(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn testu(mut a: u32, mut b: u32) {
// CHECK-NEXT:     if test1u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test5u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test6u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test(0 as i32, 2147483647 as i32);
// CHECK-NEXT:     test((2147483648 as u32) as i32, (4294967295 as u32) as i32);
// CHECK-NEXT:     test(305419896 as i32, 1842063751 as i32);
// CHECK-NEXT:     test((2452903544 as u32) as i32, (3989547399 as u32) as i32);
// CHECK-NEXT:     test(2147483647 as i32, 0 as i32);
// CHECK-NEXT:     test((4294967295 as u32) as i32, (2147483648 as u32) as i32);
// CHECK-NEXT:     testu((0 as i32) as u32, (2147483647 as i32) as u32);
// CHECK-NEXT:     testu(2147483648 as u32, 4294967295 as u32);
// CHECK-NEXT:     testu((305419896 as i32) as u32, (1842063751 as i32) as u32);
// CHECK-NEXT:     testu(2452903544 as u32, 3989547399 as u32);
// CHECK-NEXT:     testu((2147483647 as i32) as u32, (0 as i32) as u32);
// CHECK-NEXT:     testu(4294967295 as u32, 2147483648 as u32);
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
