#include <stdio.h>
#include <stdlib.h>

void touch(int *y) { *y += 1; }

static int use_and_free(int *y) {
  touch(y);
  int v = *y;
  free(y);
  return v;
}

int main(void) {
  int *y = malloc(sizeof(int));
  *y     = 41;
  printf("%d\n", use_and_free(y));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: fn touch(y: &mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = *y + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn use_and_free(mut y: Box<i32>) -> i32 {
// CHECK-NEXT:     touch(unsafe { &mut (*(&mut (*y) as *mut i32)) });
// CHECK-NEXT:     let v: i32 = unsafe { *(&mut (*y) as *mut i32) };
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let y: *mut i32 = (unsafe {
// CHECK-NEXT:         malloc((std::mem::size_of::<i32>() as u64) as usize) as *mut ()
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = 41;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", use_and_free(unsafe { Box::from_raw(y) }));
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
