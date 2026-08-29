#include <stdio.h>

__attribute__((returns_nonnull)) int *get(int *a) { return a; }

int main(void) {
  int x = 5;
  printf("%d\n", *get(&x));
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
// LOWERING-NEXT: fn get(arg0: *mut i32) -> *mut i32 {
// LOWERING-NEXT:     let mut a: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     let _v0: *mut i32 = a;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 5;
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut i32 = get(std::ptr::addr_of_mut!(x));
// LOWERING-NEXT:     let _v4: i32 = unsafe { *_v3 };
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v2 as *const i8, _v4) };
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
// REWRITES-NEXT: fn get(arg0: &i32) -> *mut i32 {
// REWRITES-NEXT: let mut a: *mut i32 = (arg0 as *const i32) as *mut i32;
// REWRITES-NEXT: let mut __retval: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = a;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = 5;
// REWRITES-NEXT: let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: *mut i32 = get(unsafe { &(*std::ptr::addr_of_mut!(x)) });
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v2 as *const i8, unsafe { *_v3 }) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
