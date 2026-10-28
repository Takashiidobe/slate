#include <stdio.h>

__attribute__((destructor)) static void farewell(void) {
  printf("destructor ran\n");
}

int main(void) {
  printf("main ran\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn farewell() {
// CHECK-NEXT:     println!("destructor ran");
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn __slate_fini_farewell() {
// CHECK-NEXT:     farewell();
// CHECK-NEXT: }
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used]
// CHECK-NEXT: #[unsafe(link_section = ".fini_array")]
// CHECK-NEXT: static __SLATE_FINI_farewell: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_fini_farewell,
// CHECK-NEXT: );
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("main ran");
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
