#include <stdio.h>

void scale(int n, int arr[*]);

void scale(int n, int arr[n]) {
  for (int i = 0; i < n; ++i) {
    arr[i] *= 2;
  }
}

int add(int, int);
int add(int a, int b) { return a + b; }

int main(void) {
  int values[3] = {1, 2, 3};
  scale(3, values);

  double separated = 1'000.5;
  char escape_e = '\e';
  char escape_unknown = '\%';

  printf("%d %d %d\n", values[0], values[1], values[2]);
  printf("%f\n", separated);
  printf("%d %d\n", (int)escape_e, (int)escape_unknown);
  printf("%zu\n", sizeof(add));
  printf("%d\n", add(3, 4));
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
// LOWERING-NEXT: unsafe fn scale(arg2: i32, arg3: *mut i32) {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut arr: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     n = arg2;
// LOWERING-NEXT:     arr = arg3;
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
// LOWERING-NEXT:                 let _v4: i32 = 2;
// LOWERING-NEXT:                 let _v5: i32 = i;
// LOWERING-NEXT:                 let _v6: i64 = _v5 as i64;
// LOWERING-NEXT:                 let _v7: *mut i32 = arr;
// LOWERING-NEXT:                 let _v8: *mut i32 = unsafe { _v7.offset(_v6 as isize) };
// LOWERING-NEXT:                 let _v9: i32 = unsafe { *_v8 };
// LOWERING-NEXT:                 let _v10: i32 = _v9 * _v4;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *_v8 = _v10;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v11: i32 = i;
// LOWERING-NEXT:             let _v12: i32 = _v11 + 1;
// LOWERING-NEXT:             i = _v12;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add(arg0: i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: i32 = a;
// LOWERING-NEXT:     let _v1: i32 = b;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut values: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let mut separated: f64 = 0.0;
// LOWERING-NEXT:     let mut escape_e: i8 = 0;
// LOWERING-NEXT:     let mut escape_unknown: i8 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     values = [1, 2, 3];
// LOWERING-NEXT:     let _v1: i32 = 3;
// LOWERING-NEXT:     let _v2: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     unsafe { scale(_v1, _v2) };
// LOWERING-NEXT:     let _v3: f64 = 1000.5;
// LOWERING-NEXT:     separated = _v3;
// LOWERING-NEXT:     let _v4: i8 = 27;
// LOWERING-NEXT:     escape_e = _v4;
// LOWERING-NEXT:     let _v5: i8 = 37;
// LOWERING-NEXT:     escape_unknown = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i64 = 0;
// LOWERING-NEXT:     let _v8: i32 = values[(_v7 as usize)];
// LOWERING-NEXT:     let _v9: i64 = 1;
// LOWERING-NEXT:     let _v10: i32 = values[(_v9 as usize)];
// LOWERING-NEXT:     let _v11: i64 = 2;
// LOWERING-NEXT:     let _v12: i32 = values[(_v11 as usize)];
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v6 as *const i8, _v8, _v10, _v12) };
// LOWERING-NEXT:     let _v14: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v15: f64 = separated;
// LOWERING-NEXT:     let _v16: i32 = unsafe { printf(_v14 as *const i8, _v15) };
// LOWERING-NEXT:     let _v17: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v18: i8 = escape_e;
// LOWERING-NEXT:     let _v19: i32 = _v18 as i32;
// LOWERING-NEXT:     let _v20: i8 = escape_unknown;
// LOWERING-NEXT:     let _v21: i32 = _v20 as i32;
// LOWERING-NEXT:     let _v22: i32 = unsafe { printf(_v17 as *const i8, _v19, _v21) };
// LOWERING-NEXT:     let _v23: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v24: u64 = 1;
// LOWERING-NEXT:     let _v25: i32 = unsafe { printf(_v23 as *const i8, _v24) };
// LOWERING-NEXT:     let _v26: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v27: i32 = 3;
// LOWERING-NEXT:     let _v28: i32 = 4;
// LOWERING-NEXT:     let _v29: i32 = add(_v27, _v28);
// LOWERING-NEXT:     let _v30: i32 = unsafe { printf(_v26 as *const i8, _v29) };
// LOWERING-NEXT:     let _v31: i32 = 0;
// LOWERING-NEXT:     __retval = _v31;
// LOWERING-NEXT:     let _v32: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v32 as i32);
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
// REWRITES-NEXT: unsafe fn scale(arg2: i32, arg3: *mut i32) {
// REWRITES-NEXT: let mut n: i32 = arg2;
// REWRITES-NEXT: let mut arr: *mut i32 = arg3;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i < n) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v4: i32 = 2;
// REWRITES-NEXT:                                     let _v7: *mut i32 = arr;
// REWRITES-NEXT:                                     let _v8: *mut i32 = unsafe { _v7.offset((i as i64) as isize) };
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         *_v8 = (unsafe { *_v8 }) * _v4;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add(arg0: i32, arg1: i32) -> i32 {
// REWRITES-NEXT: let mut a: i32 = arg0;
// REWRITES-NEXT: let mut b: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = a + b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut values: [i32; 3] = [0; 3];
// REWRITES-NEXT: let mut separated: f64 = 0.0;
// REWRITES-NEXT: let mut escape_e: i8 = 0;
// REWRITES-NEXT: let mut escape_unknown: i8 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: values = [1, 2, 3];
// REWRITES-NEXT: let _v1: i32 = 3;
// REWRITES-NEXT: let _v2: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: unsafe { scale(_v1, _v2) };
// REWRITES-NEXT: separated = 1000.5;
// REWRITES-NEXT: escape_e = 27;
// REWRITES-NEXT: escape_unknown = 37;
// REWRITES-NEXT: let _v6: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i64 = 0;
// REWRITES-NEXT: let _v9: i64 = 1;
// REWRITES-NEXT: let _v11: i64 = 2;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v6 as *const i8, values[(_v7 as usize)], values[(_v9 as usize)], values[(_v11 as usize)]) };
// REWRITES-NEXT: let _v14: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: i32 = unsafe { printf(_v14 as *const i8, separated) };
// REWRITES-NEXT: let _v17: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: i32 = unsafe { printf(_v17 as *const i8, escape_e as i32, escape_unknown as i32) };
// REWRITES-NEXT: let _v23: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v24: u64 = 1;
// REWRITES-NEXT: let _v25: i32 = unsafe { printf(_v23 as *const i8, _v24) };
// REWRITES-NEXT: let _v26: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v27: i32 = 3;
// REWRITES-NEXT: let _v28: i32 = 4;
// REWRITES-NEXT: let _v29: i32 = add(_v27, _v28);
// REWRITES-NEXT: let _v30: i32 = unsafe { printf(_v26 as *const i8, _v29) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
