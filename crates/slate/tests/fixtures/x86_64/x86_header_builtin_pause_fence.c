#include <emmintrin.h>
#include <stdio.h>

static int spin_until(volatile int *flag, int target) {
  int spins = 0;
  while (*flag < target) {
    _mm_pause();
    *flag += 1;
    spins++;
  }
  return spins;
}

int main(void) {
  volatile int flag = 0;
  int data[4] = {0};
  for (int i = 0; i < 4; i++) {
    data[i] = i * 3;
    _mm_sfence();
  }
  _mm_mfence();
  int sum = 0;
  for (int i = 0; i < 4; i++) {
    _mm_lfence();
    sum += data[i];
  }
  printf("spins=%d sum=%d\n", spin_until(&flag, 5), sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.sse2.lfence"]
// CHECK-NEXT:     fn __slate_intrinsic_6f7c5327cf9e8272();
// CHECK-NEXT:     #[link_name = "llvm.x86.sse2.mfence"]
// CHECK-NEXT:     fn __slate_intrinsic_9ae5c42efd200fe6();
// CHECK-NEXT:     #[link_name = "llvm.x86.sse.sfence"]
// CHECK-NEXT:     fn __slate_intrinsic_b2ae1adafd1df521();
// CHECK-NEXT:     #[link_name = "llvm.x86.sse2.pause"]
// CHECK-NEXT:     fn __slate_intrinsic_cf7232a07549d448();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn spin_until(flag: *mut i32, target: i32) -> i32 {
// CHECK-NEXT:     let mut spins: i32 = 0;
// CHECK-NEXT:     while (unsafe { std::ptr::read_volatile(flag) }) < target {
// CHECK-NEXT:         unsafe { __slate_intrinsic_cf7232a07549d448() };
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut i32 = flag;
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile([[V_0]]) };
// CHECK-NEXT:         unsafe { std::ptr::write_volatile([[V_0]], [[V_1]] + 1) };
// CHECK-NEXT:         spins += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     spins
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     let mut data: __SlateAlign16<[i32; 4]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         data.0[i as usize] = i * 3;
// CHECK-NEXT:         unsafe { __slate_intrinsic_b2ae1adafd1df521() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { __slate_intrinsic_9ae5c42efd200fe6() };
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in data.0.iter().copied() {
// CHECK-NEXT:         unsafe { __slate_intrinsic_6f7c5327cf9e8272() };
// CHECK-NEXT:         sum += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("spins={} sum={}", spin_until(std::ptr::addr_of_mut!(flag), 5i32), sum);
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
