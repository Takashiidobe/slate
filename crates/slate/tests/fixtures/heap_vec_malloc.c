#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int) * 3);
  p[0]   = 1;
  p[1]   = 2;
  p[2]   = 3;
  printf("%d\n", p[0] + p[1] + p[2]);
  free(p);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: *mut i32 = (unsafe {
// CHECK-NEXT:         malloc((std::mem::size_of::<i32>() as u64).wrapping_mul(3) as usize) as *mut ()
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p.add(0) = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p.add(1) = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p.add(2) = 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (unsafe { * p.add(0) }) + unsafe { * p.add(1) } + unsafe { * p.add(2) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { free((p as *mut ()) as *mut core::ffi::c_void) };
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
