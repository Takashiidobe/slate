#include <stdio.h>

static void fill(int *p, int n, int val) {
  for (int i = 0; i < n; i++) {
    *(p + i) = val;
  }
}

int main(void) {
  int buf[4] = {0};
  fill(buf, 4, 7);
  printf("%d %d %d %d\n", buf[0], buf[1], buf[2], buf[3]);
  return buf[0] + buf[1] + buf[2] + buf[3];
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill(arg0: *mut i32, arg1: i32, arg2: i32) {
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut val: i32 = 0;
// LOWERING-NEXT:     p = arg0;
// LOWERING-NEXT:     n = arg1;
// LOWERING-NEXT:     val = arg2;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v0: i32 = 0;
// LOWERING-NEXT:         i = _v0;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v1: i32 = i;
// LOWERING-NEXT:             let _v2: i32 = n;
// LOWERING-NEXT:             let _v3: bool = _v1 < _v2;
// LOWERING-NEXT:             if !_v3 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v4: i32 = val;
// LOWERING-NEXT:                 let _v5: *mut i32 = p;
// LOWERING-NEXT:                 let _v6: i32 = i;
// LOWERING-NEXT:                 let _v7: *mut i32 = unsafe { _v5.offset(_v6 as isize) };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *_v7 = _v4;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v8: i32 = i;
// LOWERING-NEXT:             let _v9: i32 = _v8 + 1;
// LOWERING-NEXT:             i = _v9;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     *buf = [0, 0, 0, 0];
// LOWERING-NEXT:     let _v1: *mut i32 = buf.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v2: i32 = 4;
// LOWERING-NEXT:     let _v3: i32 = 7;
// LOWERING-NEXT:     fill(_v1, _v2, _v3);
// LOWERING-NEXT:     let _v4: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i64 = 0;
// LOWERING-NEXT:     let _v6: i32 = buf[(_v5 as usize)];
// LOWERING-NEXT:     let _v7: i64 = 1;
// LOWERING-NEXT:     let _v8: i32 = buf[(_v7 as usize)];
// LOWERING-NEXT:     let _v9: i64 = 2;
// LOWERING-NEXT:     let _v10: i32 = buf[(_v9 as usize)];
// LOWERING-NEXT:     let _v11: i64 = 3;
// LOWERING-NEXT:     let _v12: i32 = buf[(_v11 as usize)];
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v4 as *const i8, _v6, _v8, _v10, _v12) };
// LOWERING-NEXT:     let _v14: i64 = 0;
// LOWERING-NEXT:     let _v15: i32 = buf[(_v14 as usize)];
// LOWERING-NEXT:     let _v16: i64 = 1;
// LOWERING-NEXT:     let _v17: i32 = buf[(_v16 as usize)];
// LOWERING-NEXT:     let _v18: i32 = _v15 + _v17;
// LOWERING-NEXT:     let _v19: i64 = 2;
// LOWERING-NEXT:     let _v20: i32 = buf[(_v19 as usize)];
// LOWERING-NEXT:     let _v21: i32 = _v18 + _v20;
// LOWERING-NEXT:     let _v22: i64 = 3;
// LOWERING-NEXT:     let _v23: i32 = buf[(_v22 as usize)];
// LOWERING-NEXT:     let _v24: i32 = _v21 + _v23;
// LOWERING-NEXT:     __retval = _v24;
// LOWERING-NEXT:     let _v25: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v25 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill(arg0: &mut [i32], arg2: i32) {
// REWRITES-NEXT: let mut p: *mut i32 = arg0.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let mut n: i32 = arg0.len() as i32;
// REWRITES-NEXT: let mut val: i32 = arg2;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i < n) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v4: i32 = val;
// REWRITES-NEXT:                                     let _v5: *mut i32 = p;
// REWRITES-NEXT:                                     let _v7: *mut i32 = unsafe { _v5.offset(i as isize) };
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         *_v7 = _v4;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut buf: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *buf = [0, 0, 0, 0];
// REWRITES-NEXT: let _v1: *mut i32 = buf.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v2: i32 = 4;
// REWRITES-NEXT: let _v3: i32 = 7;
// REWRITES-NEXT: fill(unsafe { std::slice::from_raw_parts_mut(_v1 as *mut i32, _v2 as usize) }, _v3);
// REWRITES-NEXT: let _v4: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i64 = 0;
// REWRITES-NEXT: let _v7: i64 = 1;
// REWRITES-NEXT: let _v9: i64 = 2;
// REWRITES-NEXT: let _v11: i64 = 3;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v4 as *const i8, buf[(_v5 as usize)], buf[(_v7 as usize)], buf[(_v9 as usize)], buf[(_v11 as usize)]) };
// REWRITES-NEXT: let _v14: i64 = 0;
// REWRITES-NEXT: let _v16: i64 = 1;
// REWRITES-NEXT: let _v19: i64 = 2;
// REWRITES-NEXT: let _v22: i64 = 3;
// REWRITES-NEXT: __retval = buf[(_v14 as usize)] + buf[(_v16 as usize)] + buf[(_v19 as usize)] + buf[(_v22 as usize)];
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
