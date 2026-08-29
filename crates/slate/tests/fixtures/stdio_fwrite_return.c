#include <stdio.h>

int main(void) {
  remove("slate_stdio_fwrite_return.tmp");
  FILE *f = fopen("slate_stdio_fwrite_return.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  char   buf[16] = "0123456789abcde";
  size_t n       = fwrite(buf, 1, 15, f);
  printf("%zu\n", n);
  fclose(f);
  remove("slate_stdio_fwrite_return.tmp");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn remove(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// LOWERING-NEXT:     fn puts(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn fwrite(_0: *const core::ffi::c_void, _1: usize, _2: usize, _3: *mut libc::FILE) -> usize;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut n: u64 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"slate_stdio_fwrite_return.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = unsafe { remove(_v1 as *const i8) };
// LOWERING-NEXT:     let _v3: *mut i8 = b"slate_stdio_fwrite_return.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: *mut libc::FILE = unsafe { fopen(_v3 as *const i8, _v4 as *const i8) };
// LOWERING-NEXT:     f = _v5;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v6: *mut libc::FILE = f;
// LOWERING-NEXT:         let _v7: bool = _v6 != std::ptr::null_mut();
// LOWERING-NEXT:         let _v8: bool = !_v7;
// LOWERING-NEXT:         if _v8 {
// LOWERING-NEXT:             let _v9: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v10: i32 = unsafe { puts(_v9 as *const i8) };
// LOWERING-NEXT:             let _v11: i32 = 0;
// LOWERING-NEXT:             __retval = _v11;
// LOWERING-NEXT:             let _v12: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v12 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     *buf = [48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 0];
// LOWERING-NEXT:     let _v13: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: *mut core::ffi::c_void = _v13 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v15: u64 = 1;
// LOWERING-NEXT:     let _v16: u64 = 15;
// LOWERING-NEXT:     let _v17: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v18: u64 = (unsafe { fwrite(_v14 as *const core::ffi::c_void, _v15 as usize, _v16 as usize, _v17 as *mut libc::FILE) }) as u64;
// LOWERING-NEXT:     n = _v18;
// LOWERING-NEXT:     let _v19: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: u64 = n;
// LOWERING-NEXT:     let _v21: i32 = unsafe { printf(_v19 as *const i8, _v20) };
// LOWERING-NEXT:     let _v22: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v23: i32 = unsafe { fclose(_v22 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v24: *mut i8 = b"slate_stdio_fwrite_return.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: i32 = unsafe { remove(_v24 as *const i8) };
// LOWERING-NEXT:     let _v26: i32 = 0;
// LOWERING-NEXT:     __retval = _v26;
// LOWERING-NEXT:     let _v27: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v27 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn remove(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// REWRITES-NEXT:     fn puts(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fwrite(_0: *const core::ffi::c_void, _1: usize, _2: usize, _3: *mut libc::FILE) -> usize;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut f: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut buf: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut n: u64 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"slate_stdio_fwrite_return.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = unsafe { remove(_v1 as *const i8) };
// REWRITES-NEXT: let _v3: *mut i8 = b"slate_stdio_fwrite_return.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: *mut i8 = b"w\0".as_ptr() as *mut i8;
// REWRITES-NEXT: f = unsafe { fopen(_v3 as *const i8, _v4 as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v7: bool = f != std::ptr::null_mut();
// REWRITES-NEXT:         let _v8: bool = !_v7;
// REWRITES-NEXT:         if _v8 {
// REWRITES-NEXT:                     let _v9: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v10: i32 = unsafe { puts(_v9 as *const i8) };
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: *buf = [48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 0];
// REWRITES-NEXT: let _v13: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: u64 = 1;
// REWRITES-NEXT: let _v16: u64 = 15;
// REWRITES-NEXT: n = (unsafe { fwrite((_v13 as *mut core::ffi::c_void) as *const core::ffi::c_void, _v15 as usize, _v16 as usize, f as *mut libc::FILE) }) as u64;
// REWRITES-NEXT: let _v19: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v21: i32 = unsafe { printf(_v19 as *const i8, n) };
// REWRITES-NEXT: let _v23: i32 = unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT: let _v24: *mut i8 = b"slate_stdio_fwrite_return.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v25: i32 = unsafe { remove(_v24 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
