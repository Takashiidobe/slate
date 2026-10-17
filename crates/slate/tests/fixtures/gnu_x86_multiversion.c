#include <stdio.h>

__attribute__((target("default"))) static int gnu_target_default(int value) {
  return value + 1;
}

__attribute__((target("sse2"))) static int gnu_target_sse2(int value) {
  return value + 2;
}

__attribute__((target_clones("default", "arch=x86-64-v2"))) static int
gnu_target_clones(int value) {
  return value + 3;
}

int gnu_cpu_dispatch(int value) __attribute__((cpu_dispatch(generic, haswell)));

int __attribute__((cpu_specific(generic))) gnu_cpu_dispatch(int value) {
  return value + 4;
}

int __attribute__((cpu_specific(haswell))) gnu_cpu_dispatch(int value) {
  return value + 4;
}

int main(void) {
  printf("%d %d %d %d\n", gnu_target_default(10), gnu_target_sse2(10),
         gnu_target_clones(10), gnu_cpu_dispatch(10));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_target_default(value: i32) -> i32 {
// CHECK-NEXT:     value + 1
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn gnu_target_sse2(value: i32) -> i32 {
// CHECK-NEXT:     value + 2
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_target_clones(value: i32) -> i32 {
// CHECK-NEXT:     value + 3
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_cpu_dispatch(value: i32) -> i32 {
// CHECK-NEXT:     value + 4
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", unsafe { gnu_target_default(10 as i32) }, unsafe {
// CHECK-NEXT:         gnu_target_sse2(10 as i32) }, gnu_target_clones(10 as i32), gnu_cpu_dispatch(10
// CHECK-NEXT:         as i32)
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
