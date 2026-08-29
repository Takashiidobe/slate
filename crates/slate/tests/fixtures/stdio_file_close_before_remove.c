#include <stdio.h>

int main(void) {
  FILE *f = fopen("slate_stdio_close_before_remove.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("owned\n", f);
  fclose(f);
  remove("slate_stdio_close_before_remove.tmp");
  puts("done");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// LOWERING-NEXT:     fn puts(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn remove(_0: *const i8) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"slate_stdio_close_before_remove.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut libc::FILE = unsafe { fopen(_v1 as *const i8, _v2 as *const i8) };
// LOWERING-NEXT:     f = _v3;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v4: *mut libc::FILE = f;
// LOWERING-NEXT:         let _v5: bool = _v4 != std::ptr::null_mut();
// LOWERING-NEXT:         let _v6: bool = !_v5;
// LOWERING-NEXT:         if _v6 {
// LOWERING-NEXT:             let _v7: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v8: i32 = unsafe { puts(_v7 as *const i8) };
// LOWERING-NEXT:             let _v9: i32 = 0;
// LOWERING-NEXT:             __retval = _v9;
// LOWERING-NEXT:             let _v10: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v10 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v11: *mut i8 = b"owned\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v13: i32 = unsafe { fputs(_v11 as *const i8, _v12 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v14: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v15: i32 = unsafe { fclose(_v14 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v16: *mut i8 = b"slate_stdio_close_before_remove.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: i32 = unsafe { remove(_v16 as *const i8) };
// LOWERING-NEXT:     let _v18: *mut i8 = b"done\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: i32 = unsafe { puts(_v18 as *const i8) };
// LOWERING-NEXT:     let _v20: i32 = 0;
// LOWERING-NEXT:     __retval = _v20;
// LOWERING-NEXT:     let _v21: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v21 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// REWRITES-NEXT:     fn puts(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn remove(_0: *const i8) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut f: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"slate_stdio_close_before_remove.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = b"w\0".as_ptr() as *mut i8;
// REWRITES-NEXT: f = unsafe { fopen(_v1 as *const i8, _v2 as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v5: bool = f != std::ptr::null_mut();
// REWRITES-NEXT:         let _v6: bool = !_v5;
// REWRITES-NEXT:         if _v6 {
// REWRITES-NEXT:                     let _v7: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v8: i32 = unsafe { puts(_v7 as *const i8) };
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v11: *mut i8 = b"owned\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: i32 = unsafe { fputs(_v11 as *const i8, f as *mut libc::FILE) };
// REWRITES-NEXT: let _v15: i32 = unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT: let _v16: *mut i8 = b"slate_stdio_close_before_remove.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: i32 = unsafe { remove(_v16 as *const i8) };
// REWRITES-NEXT: let _v18: *mut i8 = b"done\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: i32 = unsafe { puts(_v18 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
