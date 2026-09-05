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
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Triple {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT:     z: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut full: Triple = Triple { x: 0, y: 0, z: 0 };
// LOWERING-NEXT:     let mut partial: Triple = Triple { x: 0, y: 0, z: 0 };
// LOWERING-NEXT:     let mut designated: Triple = Triple { x: 0, y: 0, z: 0 };
// LOWERING-NEXT:     let mut copy: Triple = Triple { x: 0, y: 0, z: 0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Triple = Triple { x: 1, y: 2, z: 3 };
// LOWERING-NEXT:     full = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Triple = Triple { x: 4, y: 5, z: 0 };
// LOWERING-NEXT:     partial = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Triple = Triple { x: 7, y: 0, z: 9 };
// LOWERING-NEXT:     designated = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = full.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = full.y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = full.z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = partial.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = partial.y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = partial.z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = designated.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = designated.y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = designated.z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     copy = full;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:     copy.x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = full.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = copy.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Triple {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT:     z: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut full: Triple = Triple { x: 0, y: 0, z: 0 };
// REWRITES-NEXT:     let mut partial: Triple = Triple { x: 0, y: 0, z: 0 };
// REWRITES-NEXT:     let mut designated: Triple = Triple { x: 0, y: 0, z: 0 };
// REWRITES-NEXT:     let mut copy: Triple = Triple { x: 0, y: 0, z: 0 };
// REWRITES-NEXT:     full = Triple { x: 1, y: 2, z: 3 };
// REWRITES-NEXT:     partial = Triple { x: 4, y: 5, z: 0 };
// REWRITES-NEXT:     designated = Triple { x: 7, y: 0, z: 9 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = full.x;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = full.y;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = full.z;
// REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = partial.x;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = partial.y;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = partial.z;
// REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = designated.x;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = designated.y;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = designated.z;
// REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:     copy = full;
// REWRITES-NEXT:     copy.x = 42;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = full.x;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = copy.x;
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
