#include <ctype.h>
#include <locale.h>
#include <stdio.h>

int main(void) {
  setlocale(LC_ALL, "C");
  int lower = 'q';
  int upper = 'Q';
  printf("%d %d\n", toupper(lower), tolower(upper));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: use slate_support::setlocale;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         setlocale(6i32 as i32, c"C".as_ptr() as *const core::ffi::c_char) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     let lower: i32 = 113;
// CHECK-NEXT:     let upper: i32 = 81;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { slate_support::toupper(lower) }, unsafe {
// CHECK-NEXT:         slate_support::tolower(upper) }
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
