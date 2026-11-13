#include <stdio.h>
#include <stdlib.h>

int main(void) {
  void *p = malloc(16);
  free_sized(p, 16);

  void *q = aligned_alloc(16, 32);
  free_aligned_sized(q, 16, 32);

  void *r = realloc(NULL, 0);
  free(r);

  void *a = aligned_alloc(64, 64);
  printf("%d\n", memalignment(a) % 64 == 0);
  free(a);

  printf("ok\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn free_sized([[ARG_0:arg[0-9]+]]: *mut (), [[ARG_1:arg[0-9]+]]: u64);
// CHECK-NEXT:     fn free_aligned_sized([[ARG_0]]: *mut (), [[ARG_1]]: u64, [[ARG_2:arg[0-9]+]]: u64);
// CHECK-NEXT:     fn memalignment([[ARG_0]]: *const ()) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::realloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: use slate_support::aligned_alloc;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: *mut () = unsafe { malloc(16u64 as usize) as *mut () };
// CHECK-NEXT:     unsafe { free_sized(p, 16u64) };
// CHECK-NEXT:     let q: *mut () = unsafe { aligned_alloc(16u64 as usize, 32u64 as usize) as *mut () };
// CHECK-NEXT:     unsafe { free_aligned_sized(q, 16u64, 32u64) };
// CHECK-NEXT:     let r: *mut () = unsafe {
// CHECK-NEXT:         realloc(std::ptr::null_mut::<()>() as *mut core::ffi::c_void, 0u64 as usize)
// CHECK-NEXT:             as *mut ()
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { free(r as *mut core::ffi::c_void) };
// CHECK-NEXT:     let a: *mut () = unsafe { aligned_alloc(64u64 as usize, 64u64 as usize) as *mut () };
// CHECK-NEXT:     println!("{}", ((unsafe { memalignment(a as * const ()) }) % 64 == 0) as i32);
// CHECK-NEXT:     unsafe { free(a as *mut core::ffi::c_void) };
// CHECK-NEXT:     println!("ok");
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
