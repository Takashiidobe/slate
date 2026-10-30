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
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __ctype_b_loc() -> *mut *const u16;
// CHECK-NEXT:     fn setlocale([[ARG_0:arg[0-9]+]]: i32, [[ARG_1:arg[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { setlocale(6i32, c"".as_ptr()) };
// CHECK-NEXT:     let c: i8 = 65;
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((c as i32) as isize) }) as u32)
// CHECK-NEXT:         as i32) & 1024 != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("no");
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { *unsafe { *__ctype_b_loc() }.offset((c as i32) as isize) }) as u32)
// CHECK-NEXT:         as i32) & 2048 != 0
// CHECK-NEXT:     {
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
