#include <stdio.h>

int main(void) {
  FILE *fp = fopen("slate_perror_fopen_missing.tmp", "r");
  if (fp == NULL) {
    perror("open failed");
    return 1;
  }
  fclose(fp);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// LOWERING-NEXT:     fn perror(_0: *const i8);
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut fp: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"slate_perror_fopen_missing.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i8 = b"r\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut libc::FILE = unsafe { fopen(_v1 as *const i8, _v2 as *const i8) };
// LOWERING-NEXT:     fp = _v3;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v4: *mut libc::FILE = fp;
// LOWERING-NEXT:         let _v5: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:         let _v6: bool = _v4 == _v5;
// LOWERING-NEXT:         if _v6 {
// LOWERING-NEXT:             let _v7: *mut i8 = b"open failed\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             unsafe { perror(_v7 as *const i8) };
// LOWERING-NEXT:             let _v8: i32 = 1;
// LOWERING-NEXT:             __retval = _v8;
// LOWERING-NEXT:             let _v9: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v9 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v10: *mut libc::FILE = fp;
// LOWERING-NEXT:     let _v11: i32 = unsafe { fclose(_v10 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v12: i32 = 0;
// LOWERING-NEXT:     __retval = _v12;
// LOWERING-NEXT:     let _v13: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v13 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// REWRITES-NEXT:     fn perror(_0: *const i8);
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut fp: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"slate_perror_fopen_missing.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = b"r\0".as_ptr() as *mut i8;
// REWRITES-NEXT: fp = unsafe { fopen(_v1 as *const i8, _v2 as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v5: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT:         let _v6: bool = fp == _v5;
// REWRITES-NEXT:         if _v6 {
// REWRITES-NEXT:                     let _v7: *mut i8 = b"open failed\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     unsafe { perror(_v7 as *const i8) };
// REWRITES-NEXT:                     __retval = 1;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v11: i32 = unsafe { fclose(fp as *mut libc::FILE) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
