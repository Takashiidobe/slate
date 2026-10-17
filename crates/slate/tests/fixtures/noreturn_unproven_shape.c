#include <stdio.h>

/* noreturn via an infinite loop: correct C, but Slate's lowering does not yet
 * prove this diverges (the generated Rust loop keeps a conditional break
 * guard), so it must fall back to a conservative, non-`!` return type
 * instead of emitting Rust that rustc would reject. */
__attribute__((noreturn)) static void spin(void) {
  for (;;) {
  }
}

void (*unused_spin)(void) = spin;

int main(void) {
  printf("main\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// noreturn via an infinite loop: correct C, but Slate's lowering does not yet
// CHECK-NEXT: /// prove this diverges (the generated Rust loop keeps a conditional break
// CHECK-NEXT: /// guard), so it must fall back to a conservative, non-`!` return type
// CHECK-NEXT: /// instead of emitting Rust that rustc would reject.
// CHECK-NEXT: extern "C-unwind" fn spin() {
// CHECK-NEXT:     loop {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut unused_spin: Option<unsafe extern "C-unwind" fn()> = Some(spin);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("main");
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
