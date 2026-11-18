/* { dg-options "-fms-extensions" } */
#include <stdio.h>

struct point {
  int x;
  int y;
};

typedef struct point point_t;

struct labeled {
  struct point;
  int label;
};

struct typedef_member {
  point_t;
  int z;
};

int main(void) {
  struct labeled l = {{1, 2}, 3};
  struct typedef_member t = {{4, 5}, 6};
  printf("%d %d %d %d %d %d %zu\n", l.x, l.y, l.label, t.x, t.y, t.z, sizeof(struct labeled));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fms-extensions" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct labeled {
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: point,
// CHECK-NEXT:     label: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct typedef_member {
// CHECK-NEXT:     [[SLATE_ANON_0]]: point,
// CHECK-NEXT:     z: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let l: labeled = labeled {
// CHECK-NEXT:         [[SLATE_ANON_0]]: point { x: 1, y: 2 },
// CHECK-NEXT:         label: 3,
// CHECK-NEXT:     };
// CHECK-NEXT:     let t: typedef_member = typedef_member {
// CHECK-NEXT:         [[SLATE_ANON_0]]: point { x: 4, y: 5 },
// CHECK-NEXT:         z: 6,
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %d %d %d %d %zu\n".as_ptr(),
// CHECK-NEXT:             l.[[SLATE_ANON_0]].x,
// CHECK-NEXT:             l.[[SLATE_ANON_0]].y,
// CHECK-NEXT:             l.label,
// CHECK-NEXT:             t.[[SLATE_ANON_0]].x,
// CHECK-NEXT:             t.[[SLATE_ANON_0]].y,
// CHECK-NEXT:             t.z,
// CHECK-NEXT:             core::mem::size_of::<labeled>() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
