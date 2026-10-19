#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((malloc, alloc_size(1), alloc_align(2))) static void *
allocate(size_t size, size_t alignment) {
  (void)alignment;
  return malloc(size);
}

__attribute__((malloc, alloc_size(1, 2))) static void *
allocate_array(size_t count, size_t size) {
  return calloc(count, size);
}

__attribute__((assume_aligned(16, 4))) static void *
offset_aligned(void *pointer) {
  return (char *)pointer + 4;
}

int main(void) {
  int *values = allocate(3 * sizeof(int), _Alignof(int));
  int *zeroed = allocate_array(2, sizeof(int));
  values[0]   = 7;
  values[1]   = 9;
  int *second = offset_aligned(values);
  printf("%d %d %d\n", values[0], *second, zeroed[1]);
  free(zeroed);
  free(values);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn calloc(__nmemb: u64, __size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: fn allocate(size: u64, alignment: u64) -> *mut () {
// CHECK-NEXT:     unsafe { malloc(size) }
// CHECK-NEXT: }
// CHECK-NEXT: fn allocate_array(count: u64, size: u64) -> *mut () {
// CHECK-NEXT:     unsafe { calloc(count, size) }
// CHECK-NEXT: }
// CHECK-NEXT: fn offset_aligned(pointer: *mut ()) -> *mut () {
// CHECK-NEXT:     (unsafe { (pointer as *mut i8).offset(4 as isize) }) as *mut ()
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let values: *mut i32 = allocate(12u64, 4u64) as *mut i32;
// CHECK-NEXT:     let zeroed: *mut i32 = allocate_array(2u64, 4u64) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *values.offset(0 as isize) = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *values.offset(1 as isize) = 9;
// CHECK-NEXT:     }
// CHECK-NEXT:     let second: *mut i32 = offset_aligned(values as *mut ()) as *mut i32;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", unsafe { * values.offset(0 as isize) }, unsafe { * second }, unsafe {
// CHECK-NEXT:         * zeroed.offset(1 as isize) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { free(zeroed as *mut ()) };
// CHECK-NEXT:     unsafe { free(values as *mut ()) };
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
