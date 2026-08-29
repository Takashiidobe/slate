#include <stdio.h>

__attribute__((nonnull(1, 3))) static int combine(int *left, int scale,
                                                  int *right, int *optional) {
  return *left + scale * *right + (optional ? *optional : 0);
}

__attribute__((nonnull)) static int difference(int *left, int scale,
                                               int *right) {
  return *left - scale * *right;
}

int main(void) {
  int left  = 11;
  int right = 3;
  printf("%d %d\n", combine(&left, 2, &right, NULL),
         difference(&left, 2, &right));
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
// LOWERING-NEXT: fn combine(arg3: *mut i32, arg4: i32, arg5: *mut i32, arg6: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut left: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut scale: i32 = 0;
// LOWERING-NEXT:     let mut right: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut optional: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     left = arg3;
// LOWERING-NEXT:     scale = arg4;
// LOWERING-NEXT:     right = arg5;
// LOWERING-NEXT:     optional = arg6;
// LOWERING-NEXT:     let _v0: *mut i32 = left;
// LOWERING-NEXT:     let _v1: i32 = unsafe { *_v0 };
// LOWERING-NEXT:     let _v2: i32 = scale;
// LOWERING-NEXT:     let _v3: *mut i32 = right;
// LOWERING-NEXT:     let _v4: i32 = unsafe { *_v3 };
// LOWERING-NEXT:     let _v5: i32 = _v2 * _v4;
// LOWERING-NEXT:     let _v6: i32 = _v1 + _v5;
// LOWERING-NEXT:     let _v7: *mut i32 = optional;
// LOWERING-NEXT:     let _v8: bool = _v7 != std::ptr::null_mut();
// LOWERING-NEXT:     let _v9: i32 = if _v8 {
// LOWERING-NEXT:         let _v10: *mut i32 = optional;
// LOWERING-NEXT:         let _v11: i32 = unsafe { *_v10 };
// LOWERING-NEXT:         _v11
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v12: i32 = 0;
// LOWERING-NEXT:         _v12
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v13: i32 = _v6 + _v9;
// LOWERING-NEXT:     __retval = _v13;
// LOWERING-NEXT:     let _v14: i32 = __retval;
// LOWERING-NEXT:     return _v14;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn difference(arg0: *mut i32, arg1: i32, arg2: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut left: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut scale: i32 = 0;
// LOWERING-NEXT:     let mut right: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     left = arg0;
// LOWERING-NEXT:     scale = arg1;
// LOWERING-NEXT:     right = arg2;
// LOWERING-NEXT:     let _v0: *mut i32 = left;
// LOWERING-NEXT:     let _v1: i32 = unsafe { *_v0 };
// LOWERING-NEXT:     let _v2: i32 = scale;
// LOWERING-NEXT:     let _v3: *mut i32 = right;
// LOWERING-NEXT:     let _v4: i32 = unsafe { *_v3 };
// LOWERING-NEXT:     let _v5: i32 = _v2 * _v4;
// LOWERING-NEXT:     let _v6: i32 = _v1 - _v5;
// LOWERING-NEXT:     __retval = _v6;
// LOWERING-NEXT:     let _v7: i32 = __retval;
// LOWERING-NEXT:     return _v7;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut left: i32 = 0;
// LOWERING-NEXT:     let mut right: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 11;
// LOWERING-NEXT:     left = _v1;
// LOWERING-NEXT:     let _v2: i32 = 3;
// LOWERING-NEXT:     right = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: i32 = 2;
// LOWERING-NEXT:     let _v5: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v6: i32 = combine(std::ptr::addr_of_mut!(left), _v4, std::ptr::addr_of_mut!(right), _v5);
// LOWERING-NEXT:     let _v7: i32 = 2;
// LOWERING-NEXT:     let _v8: i32 = difference(std::ptr::addr_of_mut!(left), _v7, std::ptr::addr_of_mut!(right));
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v3 as *const i8, _v6, _v8) };
// LOWERING-NEXT:     let _v10: i32 = 0;
// LOWERING-NEXT:     __retval = _v10;
// LOWERING-NEXT:     let _v11: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v11 as i32);
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
// REWRITES-NEXT: fn combine(arg3: &i32, arg4: i32, arg5: &i32, arg6: *mut i32) -> i32 {
// REWRITES-NEXT: let mut scale: i32 = arg4;
// REWRITES-NEXT: let mut optional: *mut i32 = arg6;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v6: i32 = (unsafe { *((arg3 as *const i32) as *mut i32) }) + scale * unsafe { *((arg5 as *const i32) as *mut i32) };
// REWRITES-NEXT: let _v8: bool = optional != std::ptr::null_mut();
// REWRITES-NEXT: let _v9: i32 = if _v8 {
// REWRITES-NEXT:         let _v11: i32 = unsafe { *optional };
// REWRITES-NEXT:     _v11
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v12: i32 = 0;
// REWRITES-NEXT:     _v12
// REWRITES-NEXT: };
// REWRITES-NEXT: __retval = _v6 + _v9;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn difference(arg0: &i32, arg1: i32, arg2: &i32) -> i32 {
// REWRITES-NEXT: let mut scale: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = (unsafe { *((arg0 as *const i32) as *mut i32) }) - scale * unsafe { *((arg2 as *const i32) as *mut i32) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut left: i32 = 0;
// REWRITES-NEXT: let mut right: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: left = 11;
// REWRITES-NEXT: right = 3;
// REWRITES-NEXT: let _v3: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: i32 = 2;
// REWRITES-NEXT: let _v5: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let _v6: i32 = combine(unsafe { &(*std::ptr::addr_of_mut!(left)) }, _v4, unsafe { &(*std::ptr::addr_of_mut!(right)) }, _v5);
// REWRITES-NEXT: let _v7: i32 = 2;
// REWRITES-NEXT: let _v8: i32 = difference(unsafe { &(*std::ptr::addr_of_mut!(left)) }, _v7, unsafe { &(*std::ptr::addr_of_mut!(right)) });
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v3 as *const i8, _v6, _v8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
