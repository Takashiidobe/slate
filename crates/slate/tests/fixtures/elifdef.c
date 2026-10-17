#include <stdio.h>

int main(void) {
  int elifdef_value  = 0;
  int elifndef_value = 0;
  int inactive_value = 0;

#if 0
  elifdef_value = 1;
#elifdef __STDC__
  elifdef_value = 23;
#elifndef __STDC__
  elifdef_value = 2;
#else
  elifdef_value = 3;
#endif

#if 0
  elifndef_value = 1;
#elifdef C23_MISSING
  elifndef_value = 2;
#elifndef C23_MISSING
  elifndef_value = 29;
#else
  elifndef_value = 3;
#endif

#ifdef __STDC__
  inactive_value = 31;
#elifdef __STDC__
  inactive_value = 2;
#elifndef C23_MISSING
  inactive_value = 3;
#else
  inactive_value = 4;
#endif

  printf("%d %d %d\n", elifdef_value, elifndef_value, inactive_value);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let elifdef_value: i32 = 23;
// CHECK-NEXT:     let elifndef_value: i32 = 29;
// CHECK-NEXT:     let inactive_value: i32 = 31;
// CHECK-NEXT:     println!("{} {} {}", elifdef_value, elifndef_value, inactive_value);
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
