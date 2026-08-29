#include <stdio.h>

struct Triple {
  int x;
  int y;
  int z;
};

int main(void) {
  struct Triple full       = {1, 2, 3};
  struct Triple partial    = {4, 5};
  struct Triple designated = {.z = 9, .x = 7};
  printf("%d %d %d\n", full.x, full.y, full.z);
  printf("%d %d %d\n", partial.x, partial.y, partial.z);
  printf("%d %d %d\n", designated.x, designated.y, designated.z);

  struct Triple copy = full;
  copy.x             = 42;
  printf("%d %d\n", full.x, copy.x);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Triple {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT:     z: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut full: Triple = Triple { x: 0, y: 0, z: 0 };
// LOWERING-NEXT:     let mut partial: Triple = Triple { x: 0, y: 0, z: 0 };
// LOWERING-NEXT:     let mut designated: Triple = Triple { x: 0, y: 0, z: 0 };
// LOWERING-NEXT:     let mut copy: Triple = Triple { x: 0, y: 0, z: 0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     full = Triple { x: 1, y: 2, z: 3 };
// LOWERING-NEXT:     partial = Triple { x: 4, y: 5, z: 0 };
// LOWERING-NEXT:     designated = Triple { x: 7, y: 0, z: 9 };
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = full.x;
// LOWERING-NEXT:     let _v3: i32 = full.y;
// LOWERING-NEXT:     let _v4: i32 = full.z;
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = partial.x;
// LOWERING-NEXT:     let _v8: i32 = partial.y;
// LOWERING-NEXT:     let _v9: i32 = partial.z;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v6 as *const i8, _v7, _v8, _v9) };
// LOWERING-NEXT:     let _v11: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: i32 = designated.x;
// LOWERING-NEXT:     let _v13: i32 = designated.y;
// LOWERING-NEXT:     let _v14: i32 = designated.z;
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v11 as *const i8, _v12, _v13, _v14) };
// LOWERING-NEXT:     copy = full;
// LOWERING-NEXT:     let _v16: i32 = 42;
// LOWERING-NEXT:     copy.x = _v16;
// LOWERING-NEXT:     let _v17: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v18: i32 = full.x;
// LOWERING-NEXT:     let _v19: i32 = copy.x;
// LOWERING-NEXT:     let _v20: i32 = unsafe { printf(_v17 as *const i8, _v18, _v19) };
// LOWERING-NEXT:     let _v21: i32 = 0;
// LOWERING-NEXT:     __retval = _v21;
// LOWERING-NEXT:     let _v22: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v22 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Triple {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT:     z: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut full: Triple = Triple { x: 0, y: 0, z: 0 };
// REWRITES-NEXT: let mut partial: Triple = Triple { x: 0, y: 0, z: 0 };
// REWRITES-NEXT: let mut designated: Triple = Triple { x: 0, y: 0, z: 0 };
// REWRITES-NEXT: let mut copy: Triple = Triple { x: 0, y: 0, z: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: full = Triple { x: 1, y: 2, z: 3 };
// REWRITES-NEXT: partial = Triple { x: 4, y: 5, z: 0 };
// REWRITES-NEXT: designated = Triple { x: 7, y: 0, z: 9 };
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = full.x;
// REWRITES-NEXT: let _v3: i32 = full.y;
// REWRITES-NEXT: let _v4: i32 = full.z;
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = partial.x;
// REWRITES-NEXT: let _v8: i32 = partial.y;
// REWRITES-NEXT: let _v9: i32 = partial.z;
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v6 as *const i8, _v7, _v8, _v9) };
// REWRITES-NEXT: let _v11: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = designated.x;
// REWRITES-NEXT: let _v13: i32 = designated.y;
// REWRITES-NEXT: let _v14: i32 = designated.z;
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v11 as *const i8, _v12, _v13, _v14) };
// REWRITES-NEXT: copy = full;
// REWRITES-NEXT: copy.x = 42;
// REWRITES-NEXT: let _v17: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: i32 = full.x;
// REWRITES-NEXT: let _v19: i32 = copy.x;
// REWRITES-NEXT: let _v20: i32 = unsafe { printf(_v17 as *const i8, _v18, _v19) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
