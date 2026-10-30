#include <stdio.h>

static int add_pair(int lhs, int rhs) { return lhs + rhs; }

static int mul_pair(int lhs, int rhs) { return lhs * rhs; }

static int apply_binary(int (*op)(int, int), int lhs, int rhs) {
  return op(lhs, rhs);
}

int main(void) {
  int (*chosen)(int, int) = add_pair;
  printf("%d\n", apply_binary(chosen, 4, 5));
  chosen = mul_pair;
  printf("%d\n", chosen(3, 6));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.add_pair")]
// CHECK-NEXT: extern "C-unwind" fn add_pair(lhs: i32, rhs: i32) -> i32 {
// CHECK-NEXT:     lhs + rhs
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.mul_pair")]
// CHECK-NEXT: extern "C-unwind" fn mul_pair(lhs: i32, rhs: i32) -> i32 {
// CHECK-NEXT:     lhs * rhs
// CHECK-NEXT: }
// CHECK-NEXT: fn apply_binary(
// CHECK-NEXT:     op: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:     lhs: i32,
// CHECK-NEXT:     rhs: i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { op.unwrap()(lhs, rhs) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut chosen: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = Some(
// CHECK-NEXT:         add_pair,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", apply_binary(chosen, 4i32, 5i32));
// CHECK-NEXT:     chosen = Some(mul_pair);
// CHECK-NEXT:     println!("{}", unsafe { chosen.unwrap() (3i32, 6i32) });
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
