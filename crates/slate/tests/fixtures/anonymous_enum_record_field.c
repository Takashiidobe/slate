#include <stdio.h>

struct Conditional {
  enum { IN_THEN, IN_ELIF = 4, IN_ELSE } ctx;
};

int main(void) {
  struct Conditional conditional = {IN_THEN};
  conditional.ctx                = IN_ELSE;
  printf("%d %d\n", (int)conditional.ctx, IN_ELIF);
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Conditional {
// CHECK-NEXT:     ctx: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut conditional: Conditional = Conditional { ctx: 0 };
// CHECK-NEXT:     conditional.ctx = 5;
// CHECK-NEXT:     println!("{} {}", (conditional.ctx as u32) as i32, 4 as i32);
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
