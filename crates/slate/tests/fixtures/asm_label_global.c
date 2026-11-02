#include <stdio.h>

extern char **environ_alias __asm__("environ");
int counter __asm__("slate_counter_sym") = 5;
static int hidden __asm__("slate_hidden_sym") = 2;

int bump(void) { return ++counter + hidden; }

int main(void) {
  printf("%d %d\n", bump(), environ_alias != 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "environ"]
// CHECK-NEXT:     static mut environ_alias: *mut *mut i8;
// CHECK-NEXT: }
// CHECK-NEXT: static mut counter: i32 = 5;
// CHECK-NEXT: static mut hidden: i32 = 2;
// CHECK-NEXT: fn bump() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = (unsafe { counter }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counter = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]] + unsafe { hidden }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", bump(), ! unsafe { environ_alias } .is_null() as i32);
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
