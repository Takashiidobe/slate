
#include <stdio.h>

int real_impl(int x) { return x * 3 + 1; }

extern int alias_impl(int x) __attribute__((alias("real_impl")));

int call_alias(int x) { return alias_impl(x) + real_impl(x); }

int main(void) {
  printf("%d %d\n", alias_impl(5), call_alias(2));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn real_impl(x: i32) -> i32 {
// CHECK-NEXT:     x * 3 + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn call_alias(x: i32) -> i32 {
// CHECK-NEXT:     real_impl(x) + real_impl(x)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", real_impl(5i32), call_alias(2i32));
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
