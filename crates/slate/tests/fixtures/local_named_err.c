#include <stdio.h>

static const char *describe(int code) {
  const char *Err;
  if (code == 0) {
    Err = "ok";
  } else {
    Err = "bad";
  }
  return Err;
}

int main(void) {
  printf("%s\n", describe(0));
  printf("%s\n", describe(1));
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
// LOWERING-NEXT: fn describe(arg0: i32) -> *mut i8 {
// LOWERING-NEXT:     let mut code: i32 = 0;
// LOWERING-NEXT:     let mut __retval: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut Err_: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     code = arg0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v0: i32 = code;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         let _v2: bool = _v0 == _v1;
// LOWERING-NEXT:         if _v2 {
// LOWERING-NEXT:             let _v3: *mut i8 = b"ok\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             Err_ = _v3;
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let _v4: *mut i8 = b"bad\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             Err_ = _v4;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: *mut i8 = Err_;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     let _v3: *mut i8 = describe(_v2);
// LOWERING-NEXT:     let _v4: i32 = unsafe { printf(_v1 as *const i8, _v3) };
// LOWERING-NEXT:     let _v5: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i32 = 1;
// LOWERING-NEXT:     let _v7: *mut i8 = describe(_v6);
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v5 as *const i8, _v7) };
// LOWERING-NEXT:     let _v9: i32 = 0;
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v10 as i32);
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
// REWRITES-NEXT: fn describe(arg0: i32) -> *mut i8 {
// REWRITES-NEXT: let mut code: i32 = arg0;
// REWRITES-NEXT: let mut __retval: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut Err_: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v1: i32 = 0;
// REWRITES-NEXT:         let _v2: bool = code == _v1;
// REWRITES-NEXT:         if _v2 {
// REWRITES-NEXT:                     Err_ = b"ok\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:                     Err_ = b"bad\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = Err_;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 0;
// REWRITES-NEXT: let _v3: *mut i8 = describe(_v2);
// REWRITES-NEXT: let _v4: i32 = unsafe { printf(_v1 as *const i8, _v3) };
// REWRITES-NEXT: let _v5: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i32 = 1;
// REWRITES-NEXT: let _v7: *mut i8 = describe(_v6);
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v5 as *const i8, _v7) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
