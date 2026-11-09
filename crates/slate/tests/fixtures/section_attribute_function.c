#include <stdio.h>

__attribute__((noinline, section("slate_gnu_text"))) int placed(int value) {
  return value + 13;
}

__attribute__((noinline, section("slate_gnu_text"))) static int
placed_static(int value) {
  return value * 2;
}

__attribute__((noinline)) int elsewhere(int value) { return value - 1; }

extern char __start_slate_gnu_text[];
extern char __stop_slate_gnu_text[];

static int inside(int (*function)(int)) {
  char *address = (char *)function;
  return address >= __start_slate_gnu_text && address < __stop_slate_gnu_text;
}

int main(void) {
  printf("%d %d %d\n", placed(1), placed_static(2), elsewhere(3));
  printf("%d %d %d\n", inside(placed), inside(placed_static),
         inside(elsewhere));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut __start_slate_gnu_text: [i8; 0];
// CHECK-NEXT:     static mut __stop_slate_gnu_text: [i8; 0];
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = "slate_gnu_text")]
// CHECK-NEXT: extern "C-unwind" fn placed(value: i32) -> i32 {
// CHECK-NEXT:     value + 13
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = "slate_gnu_text")]
// CHECK-NEXT: extern "C-unwind" fn placed_static(value: i32) -> i32 {
// CHECK-NEXT:     value * 2
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.elsewhere")]
// CHECK-NEXT: extern "C-unwind" fn elsewhere(value: i32) -> i32 {
// CHECK-NEXT:     value - 1
// CHECK-NEXT: }
// CHECK-NEXT: fn inside(function: Option<unsafe extern "C-unwind" fn(i32) -> i32>) -> i32 {
// CHECK-NEXT:     let address: *mut i8 = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             *mut i8,
// CHECK-NEXT:         >(function)
// CHECK-NEXT:     };
// CHECK-NEXT:     (address >= unsafe { std::ptr::addr_of_mut!(__start_slate_gnu_text) as *mut i8 }
// CHECK-NEXT:         && address < unsafe { std::ptr::addr_of_mut!(__stop_slate_gnu_text) as *mut i8 })
// CHECK-NEXT:         as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {} {}", placed(1i32), placed_static(2i32), elsewhere(3i32));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", inside(Some(placed)), inside(Some(placed_static)),
// CHECK-NEXT:         inside(Some(elsewhere))
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
