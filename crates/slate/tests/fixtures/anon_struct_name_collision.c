#include <stdio.h>

struct __slate_anonymous_struct_0 {
  int value;
};

int main(void) {
  struct __slate_anonymous_struct_0 named = {7};
  struct {
    int x;
    int y;
  } point = {3, 4};
  printf("%d\n", named.value + point.x + point.y);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __slate_anonymous_struct_0 {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_anon_struct_name_collision_c_12 {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let named: __slate_anonymous_struct_0 = __slate_anonymous_struct_0 {
// CHECK-NEXT:         value: 7,
// CHECK-NEXT:     };
// CHECK-NEXT:     let point: __SlateRecord_anon_struct_name_collision_c_12 = __SlateRecord_anon_struct_name_collision_c_12 {
// CHECK-NEXT:         x: 3,
// CHECK-NEXT:         y: 4,
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", named.value + point.x + point.y);
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
