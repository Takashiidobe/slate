#include <stdio.h>

static int check_target(const char *ptr, const char *end, int *tokPtr) {
  int upper = 0;
  *tokPtr   = 7;
  if (end - ptr != 3)
    return 1;
  switch (ptr[0]) {
  case 'x':
    break;
  case 'X':
    upper = 1;
    break;
  default:
    return 1;
  }
  if (upper)
    *tokPtr = 9;
  return 0;
}

int main(void) {
  int         tok = 0;
  const char *s   = "xyz";
  int         r   = check_target(s, s + 3, &tok);
  printf("%d %d\n", r, tok);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn check_target(ptr: *const i8, end: *const i8, tokPtr: &mut i32) -> i32 {
// CHECK-NEXT:     let mut upper: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(tokPtr as *mut i32) = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { end.offset_from(ptr as *const i8) }) as i64) != 3 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     match (unsafe { *ptr.add(0) }) as i32 {
// CHECK-NEXT:         120 => {}
// CHECK-NEXT:         88 => {
// CHECK-NEXT:             upper = 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if upper != 0 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(tokPtr as *mut i32) = 9;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut tok: i32 = 0;
// CHECK-NEXT:     let s: *const i8 = c"xyz".as_ptr();
// CHECK-NEXT:     let r: i32 = check_target(
// CHECK-NEXT:         s,
// CHECK-NEXT:         unsafe { s.add(3) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(tok)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", r, tok);
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
