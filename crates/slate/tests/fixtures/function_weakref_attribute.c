#include <stdio.h>

int        target(int value) { return value + 7; }
static int helper(int value) { return value * 3; }

static int ref(int) __attribute__((weakref("target")));
static int helper_ref(int) __attribute__((weakref("helper")));

int main(void) {
  int (*through)(int) = ref;
  printf("%d %d %d %d\n", ref(89), helper_ref(5), through(1),
         through == target);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.target")]
// CHECK-NEXT: extern "C-unwind" fn target(value: i32) -> i32 {
// CHECK-NEXT:     value + 7
// CHECK-NEXT: }
// CHECK-NEXT: fn helper(value: i32) -> i32 {
// CHECK-NEXT:     value * 3
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let through: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(target);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", target(89i32), helper(5i32), unsafe { through.unwrap() (1i32) },
// CHECK-NEXT:         (through == Some(target)) as i32
// CHECK-NEXT:     );
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
