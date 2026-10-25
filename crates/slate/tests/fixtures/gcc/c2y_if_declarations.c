/* { dg-options "-std=c2y" } */
#include <stdio.h>

static int pick(int n) {
  if (int doubled = n * 2; doubled > 10)
    return doubled;
  else
    return doubled + 1;
}

static int classify(int n) {
  switch (int bucket = n / 10; bucket) {
  case 0:
    return bucket;
  case 1:
    return bucket + 100;
  default:
    return -bucket;
  }
}

int main(void) {
  int i = 3;
  if (int arr[i] = {}; !arr[0])
    printf("zero %d\n", arr[2]);
  else
    printf("nonzero\n");
  printf("%d %d\n", pick(2), pick(8));
  printf("%d %d %d\n", classify(5), classify(15), classify(35));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=c2y" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn pick(n: i32) -> i32 {
// CHECK-NEXT:     let doubled: i32 = n * 2;
// CHECK-NEXT:     if doubled > 10 {
// CHECK-NEXT:         return doubled;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return doubled + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn classify(n: i32) -> i32 {
// CHECK-NEXT:     let bucket: i32 = n / 10;
// CHECK-NEXT:     match bucket {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return bucket;
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return bucket + 100;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return -bucket;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let i: i32 = 3;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = (i as i64) as u64;
// CHECK-NEXT:     let mut arr: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity([[V_0]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_0]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe { *(arr.as_mut_ptr() as *mut i32).add(0) }) == 0 {
// CHECK-NEXT:         let __slate_printf_arg0 = unsafe { *(arr.as_mut_ptr() as *mut i32).add(2) };
// CHECK-NEXT:         println!("zero {}", __slate_printf_arg0);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("nonzero");
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", pick(2i32), pick(8i32));
// CHECK-NEXT:     println!("{} {} {}", classify(5i32), classify(15i32), classify(35i32));
// CHECK-NEXT:     0
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
