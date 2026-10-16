#include <stdint.h>
#include <stdlib.h>

uint32_t f0a(uint64_t arg2) __attribute__((noinline));

uint32_t f0a(uint64_t arg) { return ~((unsigned)(arg > -3)); }

int main() {
  uint32_t r1;
  r1 = f0a(12094370573988097329ULL);
  if (r1 != ~0U)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f0a(mut arg: u64) -> u32 {
// CHECK-NEXT:     !((arg > ((-(3 as i32) as i64) as u64)) as u32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r1: u32 = f0a(12094370573988097329 as u64);
// CHECK-NEXT:     if r1 != !(0 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
