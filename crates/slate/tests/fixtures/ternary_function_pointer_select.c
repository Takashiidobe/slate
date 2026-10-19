#include <stdio.h>

typedef int (*BinaryOp)(int, int);

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

static int apply(int useAdd, int a, int b) {
  return (useAdd ? add : sub)(a, b);
}

int main(void) {
  int      useAdd = 1;
  BinaryOp op     = useAdd ? add : sub;
  printf("%d %d %d %d\n", op(10, 3), apply(0, 10, 3), apply(1, 4, 4),
         (useAdd ? sub : add)(9, 2));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.add")]
// CHECK-NEXT: extern "C-unwind" fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.sub")]
// CHECK-NEXT: extern "C-unwind" fn sub(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a - b
// CHECK-NEXT: }
// CHECK-NEXT: fn apply(useAdd: i32, a: i32, b: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = if useAdd
// CHECK-NEXT:                 != 0
// CHECK-NEXT:             {
// CHECK-NEXT:                 Some(add)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 Some(sub)
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         }
// CHECK-NEXT:             .unwrap()(a, b)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let useAdd: i32 = 1;
// CHECK-NEXT:     let op: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = {
// CHECK-NEXT:         let [[T_0]]: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = if useAdd != 0 {
// CHECK-NEXT:             Some(add)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             Some(sub)
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", unsafe { op.unwrap() (10i32, 3i32) }, apply(0i32, 10i32, 3i32),
// CHECK-NEXT:         apply(1i32, 4i32, 4i32), unsafe { { let [[T_1:__t[0-9]+]] : Option < unsafe extern "C-unwind"
// CHECK-NEXT:         fn (i32, i32) -> i32 > = if useAdd != 0 { Some(sub) } else { Some(add) }; [[T_1]] }
// CHECK-NEXT:         .unwrap() (9i32, 2i32) }
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
