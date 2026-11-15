/* { dg-options "-ffreestanding" } */
#include <stdio.h>
#include <stdlib.h>

static int counter;

static void step(int amount) { counter += amount; }

void main(void) {
  step(3);
  step(4);
  printf("%d\n", counter);
  fflush(stdout);
  exit(counter == 7 ? 0 : 1);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-ffreestanding" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush([[ARG_0:arg[0-9]+]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::exit;
// CHECK-EMPTY:
// CHECK-NEXT: fn step(amount: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counter = counter + amount;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() {
// CHECK-NEXT:     step(3i32);
// CHECK-NEXT:     step(4i32);
// CHECK-NEXT:     unsafe { slate_support::printf(c"%d\n".as_ptr(), counter) };
// CHECK-NEXT:     unsafe { fflush(slate_support::stdout()) };
// CHECK-NEXT:     unsafe { exit((if counter == 7 { 0i32 } else { 1i32 }) as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() };
// CHECK-NEXT:     0i32
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
