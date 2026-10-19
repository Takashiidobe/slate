#include <stdbool.h>
#include <stdio.h>

static bool from_int(int x) {
  bool b = x;
  return b;
}

static _Bool from_compare(int x, int y) {
  _Bool b = x < y;
  return b;
}

static int use_bool(_Bool flag) { return flag; }

int main(void) {
  printf("%d\n", from_int(0));
  printf("%d\n", from_int(42));
  printf("%d\n", from_compare(2, 5));
  printf("%d\n", from_compare(9, 5));
  printf("%d\n", use_bool(2));
  printf("%d\n", use_bool(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn from_int(x: i32) -> bool {
// CHECK-NEXT:     let b: bool = x != 0;
// CHECK-NEXT:     b
// CHECK-NEXT: }
// CHECK-NEXT: fn from_compare(x: i32, y: i32) -> bool {
// CHECK-NEXT:     let b: bool = x < y;
// CHECK-NEXT:     b
// CHECK-NEXT: }
// CHECK-NEXT: fn use_bool(flag: bool) -> i32 {
// CHECK-NEXT:     flag as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", from_int(0i32) as i32);
// CHECK-NEXT:     println!("{}", from_int(42i32) as i32);
// CHECK-NEXT:     println!("{}", from_compare(2i32, 5i32) as i32);
// CHECK-NEXT:     println!("{}", from_compare(9i32, 5i32) as i32);
// CHECK-NEXT:     println!("{}", use_bool(2i32 != 0i32));
// CHECK-NEXT:     println!("{}", use_bool(0i32 != 0i32));
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
