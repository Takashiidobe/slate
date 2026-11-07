#include <ctype.h>
#include <locale.h>
#include <stdio.h>

int main(void) {
  setlocale(LC_ALL, "");
  char c = 'A';
  if (isalpha(c)) {
    printf("yes\n");
  } else {
    printf("no\n");
  }
  if (isdigit(c)) {
    printf("digit\n");
  } else {
    printf("not-digit\n");
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: use slate_support::setlocale;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         setlocale(6i32 as i32, c"".as_ptr() as *const core::ffi::c_char) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     let c: i8 = 65;
// CHECK-NEXT:     if (unsafe { slate_support::isalpha(c as i32) }) != 0 {
// CHECK-NEXT:         println!("yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { slate_support::isdigit(c as i32) }) != 0 {
// CHECK-NEXT:         println!("digit");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("not-digit");
// CHECK-NEXT:     }
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
