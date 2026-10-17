#include <stdio.h>
#include <x86intrin.h>

static unsigned long long elapsed(void) {
  unsigned long long start = __rdtsc();
  volatile int       sink  = 0;
  for (int i = 0; i < 1000; i++) {
    sink += i;
  }
  return _rdtsc() - start;
}

int main(void) {
  unsigned long long ticks = elapsed();
  printf("%d\n", ticks > 0 && ticks < (1ull << 40));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.rdtsc"]
// CHECK-NEXT:     fn __slate_intrinsic_9e72945d5c6a2d09() -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: fn elapsed() -> u64 {
// CHECK-NEXT:     let start: u64 = unsafe { __slate_intrinsic_9e72945d5c6a2d09() };
// CHECK-NEXT:     let mut sink: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..1000 as i32 {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(sink)) };
// CHECK-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(sink), [[V_0]] + i) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __slate_intrinsic_9e72945d5c6a2d09() }.wrapping_sub(start)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let ticks: u64 = elapsed();
// CHECK-NEXT:     println!("{}", (ticks > 0 && ticks < (1 as u64) << (40 as i32)) as i32);
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
