#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct FixedPair {
  int16_t  left;
  uint32_t right;
  size_t   count;
};

static uint32_t global_u32  = 4000000000u;
static size_t   global_size = 7;

static int32_t add_i32(int32_t a, int16_t b) {
  int32_t sum = a + b;
  return sum;
}

static uint64_t widen_u32(uint32_t value) {
  uint64_t wide = value + global_size;
  return wide;
}

static int use_fixed_types(void) {
  int8_t           small        = -5;
  uint8_t          byte         = 250;
  int16_t          short_value  = 1200;
  uint16_t         ushort_value = 65000;
  int32_t          signed_value = add_i32(100000, short_value);
  uint64_t         wide         = widen_u32(global_u32);
  struct FixedPair pair;
  pair.left  = short_value;
  pair.right = global_u32;
  pair.count = global_size + 3;
  return small + byte + pair.left + signed_value + ushort_value + pair.count +
         wide;
}

int main(void) {
  printf("%d\n", add_i32(10, 20));
  printf("%lu\n", widen_u32(5));
  printf("%d\n", use_fixed_types());
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct FixedPair {
// CHECK-NEXT:     left: i16,
// CHECK-NEXT:     right: u32,
// CHECK-NEXT:     count: u64,
// CHECK-NEXT: }
// CHECK-NEXT: static mut global_u32: u32 = 4000000000 as u32;
// CHECK-NEXT: static mut global_size: u64 = ((7 as i32) as i64) as u64;
// CHECK-NEXT: fn add_i32(a: i32, b: i16) -> i32 {
// CHECK-NEXT:     let sum: i32 = a + (b as i32);
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn widen_u32(value: u32) -> u64 {
// CHECK-NEXT:     let wide: u64 = (value as u64).wrapping_add(unsafe { global_size });
// CHECK-NEXT:     wide
// CHECK-NEXT: }
// CHECK-NEXT: fn use_fixed_types() -> i32 {
// CHECK-NEXT:     let small: i8 = -5;
// CHECK-NEXT:     let byte: u8 = ((250 as i32) as i8) as u8;
// CHECK-NEXT:     let short_value: i16 = 1200;
// CHECK-NEXT:     let ushort_value: u16 = ((65000 as i32) as i16) as u16;
// CHECK-NEXT:     let signed_value: i32 = add_i32(100000 as i32, short_value);
// CHECK-NEXT:     let wide: u64 = widen_u32(unsafe { global_u32 });
// CHECK-NEXT:     let mut pair: FixedPair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     pair.left = short_value;
// CHECK-NEXT:     pair.right = unsafe { global_u32 };
// CHECK-NEXT:     pair.count = unsafe { global_size }.wrapping_add(3);
// CHECK-NEXT:     (((((small as i32) + ((byte as u32) as i32) + (pair.left as i32) + signed_value
// CHECK-NEXT:         + ((ushort_value as u32) as i32)) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(pair.count)
// CHECK-NEXT:         .wrapping_add(wide) as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", add_i32(10 as i32, 20 as i16));
// CHECK-NEXT:     unsafe { printf(c"%lu\n".as_ptr(), widen_u32(5 as u32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", use_fixed_types());
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
