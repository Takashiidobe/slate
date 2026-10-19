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
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn realloc(__ptr: *mut (), __size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT:     fn free_sized(__ptr: *mut (), __size: u64);
// CHECK-NEXT:     fn free_aligned_sized(__ptr: *mut (), __alignment: u64, __size: u64);
// CHECK-NEXT:     fn aligned_alloc(__alignment: u64, __size: u64) -> *mut ();
// CHECK-NEXT:     fn memalignment(__p: *const ()) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: *mut () = unsafe { malloc(16u64) };
// CHECK-NEXT:     unsafe { free_sized(p, 16u64) };
// CHECK-NEXT:     let q: *mut () = unsafe { aligned_alloc(16u64, 32u64) };
// CHECK-NEXT:     unsafe { free_aligned_sized(q, 16u64, 32u64) };
// CHECK-NEXT:     let r: *mut () = unsafe { realloc(std::ptr::null_mut::<()>(), 0u64) };
// CHECK-NEXT:     unsafe { free(r) };
// CHECK-NEXT:     let a: *mut () = unsafe { aligned_alloc(64u64, 64u64) };
// CHECK-NEXT:     println!("{}", ((unsafe { memalignment(a as * const ()) }) % 64 == 0) as i32);
// CHECK-NEXT:     unsafe { free(a) };
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
