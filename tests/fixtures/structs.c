#include <stdio.h>

struct Pair {
  int left;
  int right;
};

static int sum_pair(int a, int b) {
  struct Pair p;
  p.left  = a;
  p.right = b;
  return p.left + p.right;
}

static int overwrite_left(int a, int b) {
  struct Pair p;
  p.left  = a;
  p.right = b;
  p.left  = p.right + 2;
  return p.left;
}

int main(void) {
  printf("%d\n", sum_pair(4, 5));
  printf("%d\n", overwrite_left(3, 8));
  return 0;
}

// REWRITES-DAG: p.left + p.right
// REWRITES-NOT: unsafe { p.left }
// REWRITES-NOT: unsafe { p.right }

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
// LOWERING-NEXT: struct Pair {
// LOWERING-NEXT:     left: i32,
// LOWERING-NEXT:     right: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum_pair({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = overwrite_left({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum_pair({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// LOWERING-NEXT:     p.left = {{arg[0-9]+}};
// LOWERING-NEXT:     p.right = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.left;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.right;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn overwrite_left({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// LOWERING-NEXT:     p.left = {{arg[0-9]+}};
// LOWERING-NEXT:     p.right = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.right;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     p.left = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.left;
// LOWERING-NEXT:     return {{__v[0-9]+}};
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
// REWRITES-NEXT: struct Pair {
// REWRITES-NEXT:     left: i32,
// REWRITES-NEXT:     right: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), sum_pair(4, 5)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), overwrite_left(3, 8)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum_pair({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// REWRITES-NEXT:     p.left = {{arg[0-9]+}};
// REWRITES-NEXT:     p.right = {{arg[0-9]+}};
// REWRITES-NEXT:     p.left + p.right
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn overwrite_left({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// REWRITES-NEXT:     p.left = {{arg[0-9]+}};
// REWRITES-NEXT:     p.right = {{arg[0-9]+}};
// REWRITES-NEXT:     p.left = p.right + 2;
// REWRITES-NEXT:     p.left
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
