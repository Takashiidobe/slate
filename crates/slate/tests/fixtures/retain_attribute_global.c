#include <stdio.h>

static int kept __attribute__((used, retain, section("slate_gnu_retain"))) = 41;
static int counter __attribute__((retain)) = 3;

extern int __start_slate_gnu_retain[];
extern int __stop_slate_gnu_retain[];

int main(void) {
  counter += 4;
  int sum = 0;
  for (int *it = __start_slate_gnu_retain; it < __stop_slate_gnu_retain; it++)
    sum += *it;
  printf("%d %d\n", counter, sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(used_with_arg)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut __start_slate_gnu_retain: [i32; 0];
// CHECK-NEXT:     static mut __stop_slate_gnu_retain: [i32; 0];
// CHECK-NEXT: }
// CHECK-NEXT: #[used(linker)]
// CHECK-NEXT: #[unsafe(link_section = "slate_gnu_retain")]
// CHECK-NEXT: static mut kept: i32 = 41;
// CHECK-NEXT: #[used(linker)]
// CHECK-NEXT: static mut counter: i32 = 3;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counter = counter + 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let mut it: *mut i32 = unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(__start_slate_gnu_retain) as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     while it < unsafe { std::ptr::addr_of_mut!(__stop_slate_gnu_retain) as *mut i32 } {
// CHECK-NEXT:         sum += unsafe { *it };
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut i32 = it;
// CHECK-NEXT:         it = unsafe { [[V_0]].add(1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", unsafe { counter }, sum);
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
