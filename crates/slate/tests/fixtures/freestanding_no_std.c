/* { dg-options "-ffreestanding" } */
#include <stdio.h>
#include <stdlib.h>

static int sum(int *values, int count) {
  int total = 0;
  for (int i = 0; i < count; i++)
    total += values[i];
  return total;
}

static void release(int *cell) { free(cell); }

int main(void) {
  int *cell = malloc(sizeof *cell);
  *cell = 5;
  int values[3] = {1, 2, *cell};
  release(cell);
  printf("sum %d\n", sum(values, 3));
  putchar('\n');
  if (getenv("SLATE_FREESTANDING_UNSET"))
    abort();
  exit(sum(values, 3) == 8 ? 0 : 1);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-ffreestanding" }
// CHECK-NEXT: use slate_support::abort;
// CHECK-NEXT: use slate_support::exit;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: use slate_support::getenv;
// CHECK-NEXT: use slate_support::malloc;
// CHECK-EMPTY:
// CHECK-NEXT: fn sum(values: &[i32]) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..values.len() as i32 {
// CHECK-NEXT:         total += unsafe { values[i as usize] };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn release(cell: *mut i32) {
// CHECK-NEXT:     unsafe { free((cell as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let cell: *mut i32 =
// CHECK-NEXT:         (unsafe { malloc((core::mem::size_of::<i32>() as u64) as usize) as *mut () }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *cell = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     let values: [i32; 3] = [1, 2, unsafe { *cell }];
// CHECK-NEXT:     release(cell);
// CHECK-NEXT:     unsafe { slate_support::printf(c"sum %d\n".as_ptr(), sum(&values)) };
// CHECK-NEXT:     unsafe { slate_support::putchar(10i32) };
// CHECK-NEXT:     if !unsafe {
// CHECK-NEXT:         getenv(c"SLATE_FREESTANDING_UNSET".as_ptr() as *const core::ffi::c_char) as *mut i8
// CHECK-NEXT:     }
// CHECK-NEXT:     .is_null()
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit((if sum(&values) == 8 { 0i32 } else { 1i32 }) as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
