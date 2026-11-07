#include <stdio.h>
#include <stdlib.h>

void bail(int code) __attribute__((noreturn));

void bail(int code) {
  printf("bailing with %d\n", code);
  exit(code);
}

int main(void) {
  printf("main\n");
  bail(7);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: use slate_support::exit;
// CHECK-NEXT: fn bail(code: i32) {
// CHECK-NEXT:     println!("bailing with {}", code);
// CHECK-NEXT:     unsafe { std::process::exit(code as i32) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("main");
// CHECK-NEXT:     bail(7i32);
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
