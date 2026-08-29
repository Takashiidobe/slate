#include <stdio.h>

static int weighted_sum(int *items, int len) {
  int total = 0;
  for (int i = 0; i < len; i++) {
    int item  = items[i];
    total    += item * i;
  }
  return total;
}

int main(void) {
  int values[4] = {2, 4, 6, 8};
  printf("%d\n", weighted_sum(values, 4));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn weighted_sum(arg0: *mut i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut items: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     items = arg0;
// LOWERING-NEXT:     len = arg1;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     total = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = len;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut item: i32 = 0;
// LOWERING-NEXT:                 let _v5: i32 = i;
// LOWERING-NEXT:                 let _v6: i64 = _v5 as i64;
// LOWERING-NEXT:                 let _v7: *mut i32 = items;
// LOWERING-NEXT:                 let _v8: *mut i32 = unsafe { _v7.offset(_v6 as isize) };
// LOWERING-NEXT:                 let _v9: i32 = unsafe { *_v8 };
// LOWERING-NEXT:                 item = _v9;
// LOWERING-NEXT:                 let _v10: i32 = item;
// LOWERING-NEXT:                 let _v11: i32 = i;
// LOWERING-NEXT:                 let _v12: i32 = _v10 * _v11;
// LOWERING-NEXT:                 let _v13: i32 = total;
// LOWERING-NEXT:                 let _v14: i32 = _v13 + _v12;
// LOWERING-NEXT:                 total = _v14;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v15: i32 = i;
// LOWERING-NEXT:             let _v16: i32 = _v15 + 1;
// LOWERING-NEXT:             i = _v16;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v17: i32 = total;
// LOWERING-NEXT:     __retval = _v17;
// LOWERING-NEXT:     let _v18: i32 = __retval;
// LOWERING-NEXT:     return _v18;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     *values = [2, 4, 6, 8];
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v3: i32 = 4;
// LOWERING-NEXT:     let _v4: i32 = weighted_sum(_v2, _v3);
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     __retval = _v6;
// LOWERING-NEXT:     let _v7: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v7 as i32);
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
// REWRITES-NEXT: fn weighted_sum(arg0: &[i32]) -> i32 {
// REWRITES-NEXT: let mut items: *mut i32 = arg0.as_ptr() as *mut i32;
// REWRITES-NEXT: let mut len: i32 = arg0.len() as i32;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i < len) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let mut item: i32 = 0;
// REWRITES-NEXT:                                     let _v7: *mut i32 = items;
// REWRITES-NEXT:                                     let _v8: *mut i32 = unsafe { _v7.offset((i as i64) as isize) };
// REWRITES-NEXT:                                     item = unsafe { *_v8 };
// REWRITES-NEXT:                                     total = total + item * i;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *values = [2, 4, 6, 8];
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v3: i32 = 4;
// REWRITES-NEXT: let _v4: i32 = weighted_sum(unsafe { std::slice::from_raw_parts(_v2 as *const i32, _v3 as usize) });
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
