#include <stdio.h>

__attribute__((nonnull)) int use_all(int *a, int *b) { return *a - *b; }

int main(void) {
  int x = 10;
  int y = 4;
  printf("%d\n", use_all(&x, &y));
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
// LOWERING-NEXT: unsafe fn use_all(arg0: *mut i32, arg1: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut a: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut b: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: *mut i32 = a;
// LOWERING-NEXT:     let _v1: i32 = unsafe { *_v0 };
// LOWERING-NEXT:     let _v2: *mut i32 = b;
// LOWERING-NEXT:     let _v3: i32 = unsafe { *_v2 };
// LOWERING-NEXT:     let _v4: i32 = _v1 - _v3;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut y: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 10;
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: i32 = 4;
// LOWERING-NEXT:     y = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: i32 = unsafe { use_all(std::ptr::addr_of_mut!(x), std::ptr::addr_of_mut!(y)) };
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v3 as *const i8, _v4) };
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
// REWRITES-NEXT: unsafe fn use_all(arg0: *mut i32, arg1: *mut i32) -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = (unsafe { *arg0 }) - unsafe { *arg1 };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: i32 = 0;
// REWRITES-NEXT: let mut y: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = 10;
// REWRITES-NEXT: y = 4;
// REWRITES-NEXT: let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: i32 = unsafe { use_all(std::ptr::addr_of_mut!(x), std::ptr::addr_of_mut!(y)) };
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v3 as *const i8, _v4) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
