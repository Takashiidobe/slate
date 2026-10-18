#include <stdio.h>

typedef struct {
  int  kind;
  int  size;
  _Bool is_unsigned;
} Type;

static _Bool from_int    = 5;
static _Bool from_zero   = 0;
static int   less        = 1 < 2;
static int   greater     = -1 > 2u;
static int   equal       = 3 == 3;
static int   both        = 1 && 0;
static int   either      = 0 || 7;
static int   float_order = 1.5 < 2.5;
static Type *uchar       = &(Type){1, 1, 1};
static Type  plain       = {2, 4, 0 != 0};

int main(void) {
  printf("%d %d %d %d %d %d %d %d\n", from_int, from_zero, less, greater, equal,
         both, either, float_order);
  printf("%d %d %d %d\n", uchar->kind, uchar->is_unsigned, plain.size,
         plain.is_unsigned);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     kind: i32,
// CHECK-NEXT:     size: i32,
// CHECK-NEXT:     is_unsigned: bool,
// CHECK-NEXT: }
// CHECK-NEXT: static mut from_int: bool = (5 as i32) != (0 as i32);
// CHECK-NEXT: static mut from_zero: bool = (0 as i32) != (0 as i32);
// CHECK-NEXT: static mut less: i32 = ((1 as i32) < (2 as i32)) as i32;
// CHECK-NEXT: static mut greater: i32 = ((-(1 as i32) as u32) > (2 as u32)) as i32;
// CHECK-NEXT: static mut equal: i32 = ((3 as i32) == (3 as i32)) as i32;
// CHECK-NEXT: static mut both: i32 = ((1 as i32) != (0 as i32) && (0 as i32) != (0 as i32)) as i32;
// CHECK-NEXT: static mut either: i32 = ((0 as i32) != (0 as i32) || (7 as i32) != (0 as i32)) as i32;
// CHECK-NEXT: static mut float_order: i32 = (1.5f64 < 2.5f64) as i32;
// CHECK-NEXT: static mut uchar: *mut [[SLATERECORD_0]] = unsafe {
// CHECK-NEXT:     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0:__slate_compound_[0-9]+]])
// CHECK-NEXT: };
// CHECK-NEXT: static mut plain: [[SLATERECORD_0]] = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].kind = 2 as i32;
// CHECK-NEXT:     [[T_0]].size = 4 as i32;
// CHECK-NEXT:     [[T_0]].is_unsigned = (0 as i32) != (0 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {}", (unsafe { from_int }) as i32, (unsafe { from_zero })
// CHECK-NEXT:         as i32, unsafe { less }, unsafe { greater }, unsafe { equal }, unsafe { both },
// CHECK-NEXT:         unsafe { either }, unsafe { float_order }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", unsafe { (* uchar).kind }, (unsafe { (* uchar).is_unsigned }) as
// CHECK-NEXT:         i32, unsafe { plain.size }, (unsafe { plain.is_unsigned }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut [[SLATE_COMPOUND_0]]: [[SLATERECORD_0]] = {
// CHECK-NEXT:     let mut [[T_0]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].kind = 1 as i32;
// CHECK-NEXT:     [[T_0]].size = 1 as i32;
// CHECK-NEXT:     [[T_0]].is_unsigned = (1 as i32) != (0 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
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
