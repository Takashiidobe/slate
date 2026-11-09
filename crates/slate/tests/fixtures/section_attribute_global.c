#include <stdio.h>

static int first __attribute__((section("slate_gnu_section"))) = 3;
int        second __attribute__((section("slate_gnu_section"))) = 5;
static const int sealed __attribute__((section("slate_gnu_ro"))) = 7;

extern int __start_slate_gnu_section[];
extern int __stop_slate_gnu_section[];

int main(void) {
  first += 10;
  second *= 2;
  int sum   = 0;
  int count = 0;
  for (int *it = __start_slate_gnu_section; it < __stop_slate_gnu_section;
       it++) {
    sum += *it;
    count++;
  }
  printf("%d %d %d %d %d\n", first, second, sealed, sum, count);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut __start_slate_gnu_section: [i32; 0];
// CHECK-NEXT:     static mut __stop_slate_gnu_section: [i32; 0];
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = "slate_gnu_section")]
// CHECK-NEXT: static mut first: i32 = 3;
// CHECK-NEXT: #[unsafe(link_section = "slate_gnu_section")]
// CHECK-NEXT: static mut second: i32 = 5;
// CHECK-NEXT: #[unsafe(link_section = "slate_gnu_ro")]
// CHECK-NEXT: static mut sealed: i32 = 7;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         first = first + 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         second = second * 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let mut count: i32 = 0;
// CHECK-NEXT:     let mut it: *mut i32 = unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(__start_slate_gnu_section) as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     while it < unsafe { std::ptr::addr_of_mut!(__stop_slate_gnu_section) as *mut i32 } {
// CHECK-NEXT:         sum += unsafe { *it };
// CHECK-NEXT:         count += 1;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut i32 = it;
// CHECK-NEXT:         it = unsafe { [[V_0]].add(1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}", unsafe { first }, unsafe { second }, unsafe { sealed }, sum,
// CHECK-NEXT:         count
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
