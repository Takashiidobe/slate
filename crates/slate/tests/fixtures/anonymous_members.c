#include <stdio.h>

struct container {
  int prefix;
  union {
    int   integer;
    float real;
  };
  struct {
    int x;
    int y;
  };
};

int main(void) {
  struct container value = {0};
  value.prefix           = 3;
  value.integer          = 31;
  value.x                = 37;
  value.y                = 41;
  printf("%d %d %d %d %zu\n", value.prefix, value.integer, value.x, value.y,
         sizeof(value));
  value.real = 2.5f;
  printf("%d\n", (int)value.real);
  struct container positional = {1, {2}, {3, 4}};
  struct container designated = {.y = 7, .integer = 6, .prefix = 5};
  printf("%d %d %d %d\n", positional.prefix, positional.integer, positional.x,
         positional.y);
  printf("%d %d %d %d\n", designated.prefix, designated.integer, designated.x,
         designated.y);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct container {
// CHECK-NEXT:     prefix: i32,
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     [[SLATE_ANON_1:__slate_anon_[0-9]+]]: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0]] {
// CHECK-NEXT:     integer: i32,
// CHECK-NEXT:     real: f32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1]] {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: container = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: container = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].prefix = 0 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     value.prefix = 3;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         value.[[SLATE_ANON_0]].integer = 31;
// CHECK-NEXT:     }
// CHECK-NEXT:     value.[[SLATE_ANON_1]].x = 37;
// CHECK-NEXT:     value.[[SLATE_ANON_1]].y = 41;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d %zu\n".as_ptr(),
// CHECK-NEXT:             value.prefix,
// CHECK-NEXT:             unsafe { value.[[SLATE_ANON_0]].integer },
// CHECK-NEXT:             value.[[SLATE_ANON_1]].x,
// CHECK-NEXT:             value.[[SLATE_ANON_1]].y,
// CHECK-NEXT:             16 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         value.[[SLATE_ANON_0]].real = 2.5f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", (unsafe { value.[[SLATE_ANON_0]].real }) as i32);
// CHECK-NEXT:     let positional: container = container {
// CHECK-NEXT:         prefix: 1,
// CHECK-NEXT:         [[SLATE_ANON_0]]: {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].integer = 2 as i32;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:         [[SLATE_ANON_1]]: [[SLATERECORD_1]] { x: 3, y: 4 },
// CHECK-NEXT:     };
// CHECK-NEXT:     let designated: container = container {
// CHECK-NEXT:         prefix: 5,
// CHECK-NEXT:         [[SLATE_ANON_0]]: {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].integer = 6 as i32;
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:         [[SLATE_ANON_1]]: {
// CHECK-NEXT:             let mut [[T_3:__t[0-9]+]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_3]].y = 7 as i32;
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", positional.prefix, unsafe { positional.[[SLATE_ANON_0]].integer },
// CHECK-NEXT:         positional.[[SLATE_ANON_1]].x, positional.[[SLATE_ANON_1]].y
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", designated.prefix, unsafe { designated.[[SLATE_ANON_0]].integer },
// CHECK-NEXT:         designated.[[SLATE_ANON_1]].x, designated.[[SLATE_ANON_1]].y
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
