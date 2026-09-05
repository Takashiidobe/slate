#include <stdio.h>

struct Point {
  int x;
  int y;
};

int main(void) {
  struct Point ps[2];
  ps[0].x = 1;
  ps[0].y = 2;
  ps[1].x = 3;
  ps[1].y = 4;
  printf("%d\n", ps[0].x + ps[1].y);

  struct Point init[2] = {{10, 20}, {30, 40}};
  printf("%d\n", init[0].y + init[1].x);
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
// LOWERING-NEXT: struct Point {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut ps: aligned::Aligned<aligned::A16, [Point; 2]> =
// LOWERING-NEXT:         aligned::Aligned([Point { x: 0, y: 0 }; 2]);
// LOWERING-NEXT:     let mut init: aligned::Aligned<aligned::A16, [Point; 2]> =
// LOWERING-NEXT:         aligned::Aligned([Point { x: 0, y: 0 }; 2]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     ps[({{_v[0-9]+}} as usize)].x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     ps[({{_v[0-9]+}} as usize)].y = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     ps[({{_v[0-9]+}} as usize)].x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     ps[({{_v[0-9]+}} as usize)].y = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = ps[({{_v[0-9]+}} as usize)].x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = ps[({{_v[0-9]+}} as usize)].y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: [Point; 2] = [Point { x: 10, y: 20 }, Point { x: 30, y: 40 }];
// LOWERING-NEXT:     *init = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = init[({{_v[0-9]+}} as usize)].y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = init[({{_v[0-9]+}} as usize)].x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: struct Point {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut ps: aligned::Aligned<aligned::A16, [Point; 2]> =
// REWRITES-NEXT:         aligned::Aligned([Point { x: 0, y: 0 }; 2]);
// REWRITES-NEXT:     let mut init: aligned::Aligned<aligned::A16, [Point; 2]> =
// REWRITES-NEXT:         aligned::Aligned([Point { x: 0, y: 0 }; 2]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:     ps[({{_v[0-9]+}} as usize)].x = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:     ps[({{_v[0-9]+}} as usize)].y = 2;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:     ps[({{_v[0-9]+}} as usize)].x = 3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:     ps[({{_v[0-9]+}} as usize)].y = 4;
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), ps[0].x + ps[1].y) };
// REWRITES-NEXT:     *init = [Point { x: 10, y: 20 }, Point { x: 30, y: 40 }];
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), init[0].y + init[1].x) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
