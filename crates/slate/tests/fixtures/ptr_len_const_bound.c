#include <stdio.h>

static int sum_items(int *items, int printable, int length) {
  printf("this is another number: %d %d\n", printable, length);
  int total = 0;
  for (int i = 0; i < 4; i++) {
    total += items[i];
  }
  return total;
}

int main(void) {
  int values[] = {2, 4, 6, 8};
  printf("%d\n", sum_items(values, 5, 4));
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
// LOWERING-NEXT: fn sum_items(arg0: *mut i32, arg1: i32, arg2: i32) -> i32 {
// LOWERING-NEXT:     let mut items: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut printable: i32 = 0;
// LOWERING-NEXT:     let mut length: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     items = arg0;
// LOWERING-NEXT:     printable = arg1;
// LOWERING-NEXT:     length = arg2;
// LOWERING-NEXT:     let _v0: *mut i8 = b"this is another number: %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v1: i32 = printable;
// LOWERING-NEXT:     let _v2: i32 = length;
// LOWERING-NEXT:     let _v3: i32 = unsafe { printf(_v0 as *const i8, _v1, _v2) };
// LOWERING-NEXT:     let _v4: i32 = 0;
// LOWERING-NEXT:     total = _v4;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v5: i32 = 0;
// LOWERING-NEXT:         i = _v5;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v6: i32 = i;
// LOWERING-NEXT:             let _v7: i32 = 4;
// LOWERING-NEXT:             let _v8: bool = _v6 < _v7;
// LOWERING-NEXT:             if !_v8 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v9: i32 = i;
// LOWERING-NEXT:                 let _v10: i64 = _v9 as i64;
// LOWERING-NEXT:                 let _v11: *mut i32 = items;
// LOWERING-NEXT:                 let _v12: *mut i32 = unsafe { _v11.offset(_v10 as isize) };
// LOWERING-NEXT:                 let _v13: i32 = unsafe { *_v12 };
// LOWERING-NEXT:                 let _v14: i32 = total;
// LOWERING-NEXT:                 let _v15: i32 = _v14 + _v13;
// LOWERING-NEXT:                 total = _v15;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v16: i32 = i;
// LOWERING-NEXT:             let _v17: i32 = _v16 + 1;
// LOWERING-NEXT:             i = _v17;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v18: i32 = total;
// LOWERING-NEXT:     __retval = _v18;
// LOWERING-NEXT:     let _v19: i32 = __retval;
// LOWERING-NEXT:     return _v19;
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
// LOWERING-NEXT:     let _v3: i32 = 5;
// LOWERING-NEXT:     let _v4: i32 = 4;
// LOWERING-NEXT:     let _v5: i32 = sum_items(_v2, _v3, _v4);
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v1 as *const i8, _v5) };
// LOWERING-NEXT:     let _v7: i32 = 0;
// LOWERING-NEXT:     __retval = _v7;
// LOWERING-NEXT:     let _v8: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v8 as i32);
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
// REWRITES-NEXT: fn sum_items(arg0: *mut i32, arg1: i32, arg2: i32) -> i32 {
// REWRITES-NEXT: let mut items: *mut i32 = arg0;
// REWRITES-NEXT: let mut printable: i32 = arg1;
// REWRITES-NEXT: let mut length: i32 = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: let _v0: *mut i8 = b"this is another number: %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = unsafe { printf(_v0 as *const i8, printable, length) };
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v7: i32 = 4;
// REWRITES-NEXT:                     if !(i < _v7) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v11: *mut i32 = items;
// REWRITES-NEXT:                                     let _v12: *mut i32 = unsafe { _v11.offset((i as i64) as isize) };
// REWRITES-NEXT:                                     total = total + unsafe { *_v12 };
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
// REWRITES-NEXT: let _v3: i32 = 5;
// REWRITES-NEXT: let _v4: i32 = 4;
// REWRITES-NEXT: let _v5: i32 = sum_items(_v2, _v3, _v4);
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v1 as *const i8, _v5) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
