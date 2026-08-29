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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Point {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut ps: aligned::Aligned<aligned::A16, [Point; 2]> = aligned::Aligned([Point { x: 0, y: 0 }; 2]);
// LOWERING-NEXT:     let mut init: aligned::Aligned<aligned::A16, [Point; 2]> = aligned::Aligned([Point { x: 0, y: 0 }; 2]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     let _v2: i64 = 0;
// LOWERING-NEXT:     ps[(_v2 as usize)].x = _v1;
// LOWERING-NEXT:     let _v3: i32 = 2;
// LOWERING-NEXT:     let _v4: i64 = 0;
// LOWERING-NEXT:     ps[(_v4 as usize)].y = _v3;
// LOWERING-NEXT:     let _v5: i32 = 3;
// LOWERING-NEXT:     let _v6: i64 = 1;
// LOWERING-NEXT:     ps[(_v6 as usize)].x = _v5;
// LOWERING-NEXT:     let _v7: i32 = 4;
// LOWERING-NEXT:     let _v8: i64 = 1;
// LOWERING-NEXT:     ps[(_v8 as usize)].y = _v7;
// LOWERING-NEXT:     let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: i64 = 0;
// LOWERING-NEXT:     let _v11: i32 = ps[(_v10 as usize)].x;
// LOWERING-NEXT:     let _v12: i64 = 1;
// LOWERING-NEXT:     let _v13: i32 = ps[(_v12 as usize)].y;
// LOWERING-NEXT:     let _v14: i32 = _v11 + _v13;
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v9 as *const i8, _v14) };
// LOWERING-NEXT:     *init = [Point { x: 10, y: 20 }, Point { x: 30, y: 40 }];
// LOWERING-NEXT:     let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: i64 = 0;
// LOWERING-NEXT:     let _v18: i32 = init[(_v17 as usize)].y;
// LOWERING-NEXT:     let _v19: i64 = 1;
// LOWERING-NEXT:     let _v20: i32 = init[(_v19 as usize)].x;
// LOWERING-NEXT:     let _v21: i32 = _v18 + _v20;
// LOWERING-NEXT:     let _v22: i32 = unsafe { printf(_v16 as *const i8, _v21) };
// LOWERING-NEXT:     let _v23: i32 = 0;
// LOWERING-NEXT:     __retval = _v23;
// LOWERING-NEXT:     let _v24: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v24 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Point {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut ps: aligned::Aligned<aligned::A16, [Point; 2]> = aligned::Aligned([Point { x: 0, y: 0 }; 2]);
// REWRITES-NEXT: let mut init: aligned::Aligned<aligned::A16, [Point; 2]> = aligned::Aligned([Point { x: 0, y: 0 }; 2]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v2: i64 = 0;
// REWRITES-NEXT: ps[(_v2 as usize)].x = 1;
// REWRITES-NEXT: let _v4: i64 = 0;
// REWRITES-NEXT: ps[(_v4 as usize)].y = 2;
// REWRITES-NEXT: let _v6: i64 = 1;
// REWRITES-NEXT: ps[(_v6 as usize)].x = 3;
// REWRITES-NEXT: let _v8: i64 = 1;
// REWRITES-NEXT: ps[(_v8 as usize)].y = 4;
// REWRITES-NEXT: let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i64 = 0;
// REWRITES-NEXT: let _v12: i64 = 1;
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v9 as *const i8, ps[(_v10 as usize)].x + ps[(_v12 as usize)].y) };
// REWRITES-NEXT: *init = [Point { x: 10, y: 20 }, Point { x: 30, y: 40 }];
// REWRITES-NEXT: let _v16: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: i64 = 0;
// REWRITES-NEXT: let _v19: i64 = 1;
// REWRITES-NEXT: let _v22: i32 = unsafe { printf(_v16 as *const i8, init[(_v17 as usize)].y + init[(_v19 as usize)].x) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
