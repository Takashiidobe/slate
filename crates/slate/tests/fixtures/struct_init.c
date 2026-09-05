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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Triple {
// COMMON-LOWERING-NEXT:     x: i32,
// COMMON-LOWERING-NEXT:     y: i32,
// COMMON-LOWERING-NEXT:     z: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut full: Triple = Triple { x: 0, y: 0, z: 0 };
// COMMON-LOWERING-NEXT:     let mut partial: Triple = Triple { x: 0, y: 0, z: 0 };
// COMMON-LOWERING-NEXT:     let mut designated: Triple = Triple { x: 0, y: 0, z: 0 };
// COMMON-LOWERING-NEXT:     let mut copy: Triple = Triple { x: 0, y: 0, z: 0 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Triple = Triple { x: 1, y: 2, z: 3 };
// COMMON-LOWERING-NEXT:     full = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Triple = Triple { x: 4, y: 5, z: 0 };
// COMMON-LOWERING-NEXT:     partial = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Triple = Triple { x: 7, y: 0, z: 9 };
// COMMON-LOWERING-NEXT:     designated = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = full.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = full.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = full.z;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = partial.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = partial.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = partial.z;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = designated.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = designated.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = designated.z;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     copy = full;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:     copy.x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = full.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = copy.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Triple {
// COMMON-REWRITES-NEXT:     x: i32,
// COMMON-REWRITES-NEXT:     y: i32,
// COMMON-REWRITES-NEXT:     z: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut full: Triple = Triple { x: 0, y: 0, z: 0 };
// COMMON-REWRITES-NEXT:     let mut partial: Triple = Triple { x: 0, y: 0, z: 0 };
// COMMON-REWRITES-NEXT:     let mut designated: Triple = Triple { x: 0, y: 0, z: 0 };
// COMMON-REWRITES-NEXT:     let mut copy: Triple = Triple { x: 0, y: 0, z: 0 };
// COMMON-REWRITES-NEXT:     full = Triple { x: 1, y: 2, z: 3 };
// COMMON-REWRITES-NEXT:     partial = Triple { x: 4, y: 5, z: 0 };
// COMMON-REWRITES-NEXT:     designated = Triple { x: 7, y: 0, z: 9 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = full.x;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = full.y;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = full.z;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = partial.x;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = partial.y;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = partial.z;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = designated.x;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = designated.y;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = designated.z;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     copy = full;
// COMMON-REWRITES-NEXT:     copy.x = 42;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = full.x;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = copy.x;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
