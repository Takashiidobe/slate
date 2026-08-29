#include <stdio.h>

static int first_plus_last(char *s) { return s[0] + s[10]; }

int main(void) {
  char *message = "write error";
  printf("%d\n", first_plus_last(message));
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
// LOWERING-NEXT: fn first_plus_last(arg0: *mut i8) -> i32 {
// LOWERING-NEXT:     let mut s: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     s = arg0;
// LOWERING-NEXT:     let _v0: i64 = 0;
// LOWERING-NEXT:     let _v1: *mut i8 = s;
// LOWERING-NEXT:     let _v2: *mut i8 = unsafe { _v1.add(0) };
// LOWERING-NEXT:     let _v3: i8 = unsafe { *_v2 };
// LOWERING-NEXT:     let _v4: i32 = _v3 as i32;
// LOWERING-NEXT:     let _v5: i64 = 10;
// LOWERING-NEXT:     let _v6: *mut i8 = s;
// LOWERING-NEXT:     let _v7: *mut i8 = unsafe { _v6.add(10) };
// LOWERING-NEXT:     let _v8: i8 = unsafe { *_v7 };
// LOWERING-NEXT:     let _v9: i32 = _v8 as i32;
// LOWERING-NEXT:     let _v10: i32 = _v4 + _v9;
// LOWERING-NEXT:     __retval = _v10;
// LOWERING-NEXT:     let _v11: i32 = __retval;
// LOWERING-NEXT:     return _v11;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut message: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"write error\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     message = _v1;
// LOWERING-NEXT:     let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut i8 = message;
// LOWERING-NEXT:     let _v4: i32 = first_plus_last(_v3);
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
// REWRITES-NEXT: fn first_plus_last(arg0: *mut i8) -> i32 {
// REWRITES-NEXT: let mut s: *mut i8 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v0: i64 = 0;
// REWRITES-NEXT: let _v1: *mut i8 = s;
// REWRITES-NEXT: let _v2: *mut i8 = unsafe { _v1.add(0) };
// REWRITES-NEXT: let _v4: i32 = (unsafe { *_v2 }) as i32;
// REWRITES-NEXT: let _v5: i64 = 10;
// REWRITES-NEXT: let _v6: *mut i8 = s;
// REWRITES-NEXT: let _v7: *mut i8 = unsafe { _v6.add(10) };
// REWRITES-NEXT: __retval = _v4 + ((unsafe { *_v7 }) as i32);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut message: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: message = b"write error\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: i32 = first_plus_last(message);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v2 as *const i8, _v4) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
