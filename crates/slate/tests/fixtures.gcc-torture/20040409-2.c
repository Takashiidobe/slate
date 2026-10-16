#include <limits.h>

extern void abort();

int test1(int x) { return (x ^ INT_MIN) ^ 0x1234; }

unsigned int test1u(unsigned int x) {
  return (x ^ (unsigned int)INT_MIN) ^ 0x1234;
}

int test2(int x) { return (x ^ 0x1234) ^ INT_MIN; }

unsigned int test2u(unsigned int x) {
  return (x ^ 0x1234) ^ (unsigned int)INT_MIN;
}

unsigned int test3u(unsigned int x) {
  return (x + (unsigned int)INT_MIN) ^ 0x1234;
}

unsigned int test4u(unsigned int x) {
  return (x ^ 0x1234) + (unsigned int)INT_MIN;
}

unsigned int test5u(unsigned int x) {
  return (x - (unsigned int)INT_MIN) ^ 0x1234;
}

unsigned int test6u(unsigned int x) {
  return (x ^ 0x1234) - (unsigned int)INT_MIN;
}

int test7(int x) {
  int y = INT_MIN;
  int z = 0x1234;
  return (x ^ y) ^ z;
}

unsigned int test7u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  unsigned int z = 0x1234;
  return (x ^ y) ^ z;
}

int test8(int x) {
  int y = 0x1234;
  int z = INT_MIN;
  return (x ^ y) ^ z;
}

unsigned int test8u(unsigned int x) {
  unsigned int y = 0x1234;
  unsigned int z = (unsigned int)INT_MIN;
  return (x ^ y) ^ z;
}

unsigned int test9u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  unsigned int z = 0x1234;
  return (x + y) ^ z;
}

unsigned int test10u(unsigned int x) {
  unsigned int y = 0x1234;
  unsigned int z = (unsigned int)INT_MIN;
  return (x ^ y) + z;
}

unsigned int test11u(unsigned int x) {
  unsigned int y = (unsigned int)INT_MIN;
  unsigned int z = 0x1234;
  return (x - y) ^ z;
}

unsigned int test12u(unsigned int x) {
  unsigned int y = 0x1234;
  unsigned int z = (unsigned int)INT_MIN;
  return (x ^ y) - z;
}

void test(int a, int b) {
  if (test1(a) != b)
    abort();
  if (test2(a) != b)
    abort();
  if (test7(a) != b)
    abort();
  if (test8(a) != b)
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
  if (test7u(a) != b)
    abort();
  if (test8u(a) != b)
    abort();
  if (test9u(a) != b)
    abort();
  if (test10u(a) != b)
    abort();
  if (test11u(a) != b)
    abort();
  if (test12u(a) != b)
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

  testu(0x00000000, 0x80001234);
  testu(0x00001234, 0x80000000);
  testu(0x80000000, 0x00001234);
  testu(0x80001234, 0x00000000);
  testu(0x7fffffff, 0xffffedcb);
  testu(0xffffffff, 0x7fffedcb);
#endif

#if INT_MAX == 32767
  test(0x0000, 0x9234);
  test(0x1234, 0x8000);
  test(0x8000, 0x1234);
  test(0x9234, 0x0000);
  test(0x7fff, 0xedcb);
  test(0xffff, 0x6dcb);

  testu(0x0000, 0x9234);
  testu(0x8000, 0x1234);
  testu(0x1234, 0x8000);
  testu(0x9234, 0x0000);
  testu(0x7fff, 0xedcb);
  testu(0xffff, 0x6dcb);
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
// CHECK-NEXT:     x ^ -(2147483647 as i32) - (1 as i32) ^ (4660 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test1u(mut x: u32) -> u32 {
// CHECK-NEXT:     x ^ ((-(2147483647 as i32) - (1 as i32)) as u32) ^ ((4660 as i32) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test2(mut x: i32) -> i32 {
// CHECK-NEXT:     x ^ (4660 as i32) ^ -(2147483647 as i32) - (1 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test2u(mut x: u32) -> u32 {
// CHECK-NEXT:     x ^ ((4660 as i32) as u32) ^ ((-(2147483647 as i32) - (1 as i32)) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test3u(mut x: u32) -> u32 {
// CHECK-NEXT:     x.wrapping_add((-(2147483647 as i32) - (1 as i32)) as u32) ^ ((4660 as i32) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test4u(mut x: u32) -> u32 {
// CHECK-NEXT:     (x ^ ((4660 as i32) as u32)).wrapping_add((-(2147483647 as i32) - (1 as i32)) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test5u(mut x: u32) -> u32 {
// CHECK-NEXT:     x.wrapping_sub((-(2147483647 as i32) - (1 as i32)) as u32) ^ ((4660 as i32) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test6u(mut x: u32) -> u32 {
// CHECK-NEXT:     (x ^ ((4660 as i32) as u32)).wrapping_sub((-(2147483647 as i32) - (1 as i32)) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn test7(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = -(2147483647 as i32) - (1 as i32);
// CHECK-NEXT:     let mut z: i32 = 4660 as i32;
// CHECK-NEXT:     x ^ y ^ z
// CHECK-NEXT: }
// CHECK-NEXT: fn test7u(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: u32 = (-(2147483647 as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     let mut z: u32 = (4660 as i32) as u32;
// CHECK-NEXT:     x ^ y ^ z
// CHECK-NEXT: }
// CHECK-NEXT: fn test8(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = 4660 as i32;
// CHECK-NEXT:     let mut z: i32 = -(2147483647 as i32) - (1 as i32);
// CHECK-NEXT:     x ^ y ^ z
// CHECK-NEXT: }
// CHECK-NEXT: fn test8u(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: u32 = (4660 as i32) as u32;
// CHECK-NEXT:     let mut z: u32 = (-(2147483647 as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     x ^ y ^ z
// CHECK-NEXT: }
// CHECK-NEXT: fn test9u(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: u32 = (-(2147483647 as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     let mut z: u32 = (4660 as i32) as u32;
// CHECK-NEXT:     x.wrapping_add(y) ^ z
// CHECK-NEXT: }
// CHECK-NEXT: fn test10u(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: u32 = (4660 as i32) as u32;
// CHECK-NEXT:     let mut z: u32 = (-(2147483647 as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     (x ^ y).wrapping_add(z)
// CHECK-NEXT: }
// CHECK-NEXT: fn test11u(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: u32 = (-(2147483647 as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     let mut z: u32 = (4660 as i32) as u32;
// CHECK-NEXT:     x.wrapping_sub(y) ^ z
// CHECK-NEXT: }
// CHECK-NEXT: fn test12u(mut x: u32) -> u32 {
// CHECK-NEXT:     let mut y: u32 = (4660 as i32) as u32;
// CHECK-NEXT:     let mut z: u32 = (-(2147483647 as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     (x ^ y).wrapping_sub(z)
// CHECK-NEXT: }
// CHECK-NEXT: fn test(mut a: i32, mut b: i32) {
// CHECK-NEXT:     if test1(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test7(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test8(a) != b {
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
// CHECK-NEXT:     if test7u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test8u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test9u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test10u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test11u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test12u(a) != b {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test(0 as i32, (2147488308 as u32) as i32);
// CHECK-NEXT:     test(4660 as i32, (2147483648 as u32) as i32);
// CHECK-NEXT:     test((2147483648 as u32) as i32, 4660 as i32);
// CHECK-NEXT:     test((2147488308 as u32) as i32, 0 as i32);
// CHECK-NEXT:     test(2147483647 as i32, (4294962635 as u32) as i32);
// CHECK-NEXT:     test((4294967295 as u32) as i32, 2147478987 as i32);
// CHECK-NEXT:     testu((0 as i32) as u32, 2147488308 as u32);
// CHECK-NEXT:     testu((4660 as i32) as u32, 2147483648 as u32);
// CHECK-NEXT:     testu(2147483648 as u32, (4660 as i32) as u32);
// CHECK-NEXT:     testu(2147488308 as u32, (0 as i32) as u32);
// CHECK-NEXT:     testu((2147483647 as i32) as u32, 4294962635 as u32);
// CHECK-NEXT:     testu(4294967295 as u32, (2147478987 as i32) as u32);
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
