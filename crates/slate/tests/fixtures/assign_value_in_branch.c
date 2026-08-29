#include <stdio.h>

unsigned long bump(unsigned long *p, int c) {
  return c ? (*p += 2) : (*p += 1);
}

int main() {
  unsigned long x = 10;
  unsigned long a = bump(&x, 1);
  unsigned long b = bump(&x, 0);
  printf("%lu %lu %lu\n", a, b, x);
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
// LOWERING-NEXT: unsafe fn bump(arg0: *mut u64, arg1: i32) -> u64 {
// LOWERING-NEXT:     let mut p: *mut u64 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut c: i32 = 0;
// LOWERING-NEXT:     let mut __retval: u64 = 0;
// LOWERING-NEXT:     p = arg0;
// LOWERING-NEXT:     c = arg1;
// LOWERING-NEXT:     let _v0: i32 = c;
// LOWERING-NEXT:     let _v1: bool = _v0 != 0;
// LOWERING-NEXT:     let _v2: u64 = if _v1 {
// LOWERING-NEXT:         let _v3: u64 = 2;
// LOWERING-NEXT:         let _v4: *mut u64 = p;
// LOWERING-NEXT:         let _v5: u64 = unsafe { *_v4 };
// LOWERING-NEXT:         let _v6: u64 = _v5 + _v3;
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             *_v4 = _v6;
// LOWERING-NEXT:         }
// LOWERING-NEXT:         _v6
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v7: u64 = 1;
// LOWERING-NEXT:         let _v8: *mut u64 = p;
// LOWERING-NEXT:         let _v9: u64 = unsafe { *_v8 };
// LOWERING-NEXT:         let _v10: u64 = _v9 + _v7;
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             *_v8 = _v10;
// LOWERING-NEXT:         }
// LOWERING-NEXT:         _v10
// LOWERING-NEXT:     };
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v11: u64 = __retval;
// LOWERING-NEXT:     return _v11;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: u64 = 0;
// LOWERING-NEXT:     let mut a: u64 = 0;
// LOWERING-NEXT:     let mut b: u64 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u64 = 10;
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: i32 = 1;
// LOWERING-NEXT:     let _v3: u64 = unsafe { bump(std::ptr::addr_of_mut!(x), _v2) };
// LOWERING-NEXT:     a = _v3;
// LOWERING-NEXT:     let _v4: i32 = 0;
// LOWERING-NEXT:     let _v5: u64 = unsafe { bump(std::ptr::addr_of_mut!(x), _v4) };
// LOWERING-NEXT:     b = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%lu %lu %lu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: u64 = a;
// LOWERING-NEXT:     let _v8: u64 = b;
// LOWERING-NEXT:     let _v9: u64 = x;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v6 as *const i8, _v7, _v8, _v9) };
// LOWERING-NEXT:     let _v11: i32 = 0;
// LOWERING-NEXT:     __retval = _v11;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v12 as i32);
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
// REWRITES-NEXT: unsafe fn bump(arg0: *mut u64, arg1: i32) -> u64 {
// REWRITES-NEXT: let mut c: i32 = arg1;
// REWRITES-NEXT: let mut __retval: u64 = 0;
// REWRITES-NEXT: let _v1: bool = c != 0;
// REWRITES-NEXT: let _v2: u64 = if _v1 {
// REWRITES-NEXT:         let _v3: u64 = 2;
// REWRITES-NEXT:         let _v4: *mut u64 = arg0;
// REWRITES-NEXT:         let _v6: u64 = (unsafe { *_v4 }) + _v3;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:                     *_v4 = _v6;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     _v6
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v7: u64 = 1;
// REWRITES-NEXT:         let _v8: *mut u64 = arg0;
// REWRITES-NEXT:         let _v10: u64 = (unsafe { *_v8 }) + _v7;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:                     *_v8 = _v10;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     _v10
// REWRITES-NEXT: };
// REWRITES-NEXT: __retval = _v2;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: u64 = 0;
// REWRITES-NEXT: let mut a: u64 = 0;
// REWRITES-NEXT: let mut b: u64 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = 10;
// REWRITES-NEXT: let _v2: i32 = 1;
// REWRITES-NEXT: a = unsafe { bump(std::ptr::addr_of_mut!(x), _v2) };
// REWRITES-NEXT: let _v4: i32 = 0;
// REWRITES-NEXT: b = unsafe { bump(std::ptr::addr_of_mut!(x), _v4) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%lu %lu %lu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v6 as *const i8, a, b, x) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
