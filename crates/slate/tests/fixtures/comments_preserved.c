/* preserved-file-comment */
#include <stdio.h>
#include "comments_preserved.h"
#include "comments_preserved.h"
struct Value {
  /* preserved-field-comment */
  int n;
  /* preserved-field-trailing-comment */
};
enum Kind {
  /* preserved-enumerator-comment */
  KIND = 3
};
/* preserved-switch-function-comment */
static int selected(int n) {
  switch (n) {
    /* preserved-before-case-comment */
    case 0:
      /* preserved-between-cases-comment */
    case 1:
      return 3;
      /* preserved-after-return-comment */
    default:
      return KIND;
  }
}
/* preserved-goto-function-comment */
static int jumped(int n) {
  /* preserved-before-goto-comment */
  goto done;
  /* preserved-unreachable-comment */
  n++;
done:
  /* preserved-label-comment */
  return n;
}
/* preserved-function-comment */
static int value(int n) {
  /* preserved-local-comment */
  int result = n;
  while (result < 3) {
    // preserved-loop-comment
    result++;
  }
  /* preserved-return-comment */
  return result;
}
int main(void) {
  struct HeaderValue h = { jumped(selected(1)) };
  struct Value v = { value(h.n) };
  // preserved-call-comment
  printf("%d\n", v.n);
  return 0;
}
/* preserved-trailing-comment */

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! preserved-file-comment
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct HeaderValue {
// CHECK-NEXT:     n: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Value {
// CHECK-NEXT:     /// preserved-field-comment
// CHECK-NEXT:     n: i32,
// CHECK-NEXT:     // preserved-field-trailing-comment
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// preserved-switch-function-comment
// CHECK-NEXT: fn selected(n: i32) -> i32 {
// CHECK-NEXT:     match n {
// CHECK-NEXT:         0 | 1 => {
// CHECK-NEXT:             // preserved-before-case-comment
// CHECK-NEXT:             return 3; // preserved-between-cases-comment
// CHECK-NEXT:             // preserved-after-return-comment
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 3;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// preserved-goto-function-comment
// CHECK-NEXT: fn jumped(n: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         // preserved-before-goto-comment
// CHECK-NEXT:         return n;
// CHECK-NEXT:     }
// CHECK-NEXT:     // preserved-unreachable-comment
// CHECK-NEXT:     // preserved-label-comment
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// preserved-function-comment
// CHECK-NEXT: fn value(mut result: i32) -> i32 {
// CHECK-NEXT:     // preserved-local-comment
// CHECK-NEXT:     while result < 3 {
// CHECK-NEXT:         // preserved-loop-comment
// CHECK-NEXT:         result += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     // preserved-return-comment
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let h: HeaderValue = HeaderValue {
// CHECK-NEXT:         n: jumped(selected(1 as i32)),
// CHECK-NEXT:     };
// CHECK-NEXT:     let v: Value = Value { n: value(h.n) };
// CHECK-NEXT:     // preserved-call-comment
// CHECK-NEXT:     println!("{}", v.n);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // preserved-trailing-comment
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
