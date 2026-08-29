#include <stdio.h>
#include <stdlib.h>

int main(void) {
  remove("slate_stdio_fread_unsupported_buffer.tmp");
  FILE *f = fopen("slate_stdio_fread_unsupported_buffer.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("heap-owned", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_fread_unsupported_buffer.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char *buf = malloc(16);
  buf[0]    = 0;
  size_t n  = fread(buf, 1, 10, g);
  buf[n]    = 0;
  printf("%zu %s\n", n, buf);
  free(buf);
  fclose(g);
  remove("slate_stdio_fread_unsupported_buffer.tmp");
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
// LOWERING-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn fread(_0: *mut core::ffi::c_void, _1: usize, _2: usize, _3: *mut libc::FILE) -> usize;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut g: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut buf: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut n: u64 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"slate_stdio_fread_unsupported_buffer.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = unsafe { remove(_v1 as *const i8) };
// LOWERING-NEXT:     let _v3: *mut i8 = b"slate_stdio_fread_unsupported_buffer.tmp\0".as_ptr() as *mut i8;
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
// LOWERING-NEXT:     let _v13: *mut i8 = b"heap-owned\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v15: i32 = unsafe { fputs(_v13 as *const i8, _v14 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v16: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v17: i32 = unsafe { fclose(_v16 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v18: *mut i8 = b"slate_stdio_fread_unsupported_buffer.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: *mut i8 = b"r\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: *mut libc::FILE = unsafe { fopen(_v18 as *const i8, _v19 as *const i8) };
// LOWERING-NEXT:     g = _v20;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v21: *mut libc::FILE = g;
// LOWERING-NEXT:         let _v22: bool = _v21 != std::ptr::null_mut();
// LOWERING-NEXT:         let _v23: bool = !_v22;
// LOWERING-NEXT:         if _v23 {
// LOWERING-NEXT:             let _v24: *mut i8 = b"reopen-fail\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v25: i32 = unsafe { puts(_v24 as *const i8) };
// LOWERING-NEXT:             let _v26: i32 = 0;
// LOWERING-NEXT:             __retval = _v26;
// LOWERING-NEXT:             let _v27: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v27 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v28: u64 = 16;
// LOWERING-NEXT:     let _v29: *mut core::ffi::c_void = unsafe { malloc(_v28 as usize) };
// LOWERING-NEXT:     let _v30: *mut i8 = _v29 as *mut i8;
// LOWERING-NEXT:     buf = _v30;
// LOWERING-NEXT:     let _v31: i8 = 0;
// LOWERING-NEXT:     let _v32: i64 = 0;
// LOWERING-NEXT:     let _v33: *mut i8 = buf;
// LOWERING-NEXT:     let _v34: *mut i8 = unsafe { _v33.add(0) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v34 = _v31;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v35: *mut i8 = buf;
// LOWERING-NEXT:     let _v36: *mut core::ffi::c_void = _v35 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v37: u64 = 1;
// LOWERING-NEXT:     let _v38: u64 = 10;
// LOWERING-NEXT:     let _v39: *mut libc::FILE = g;
// LOWERING-NEXT:     let _v40: u64 = (unsafe { fread(_v36 as *mut core::ffi::c_void, _v37 as usize, _v38 as usize, _v39 as *mut libc::FILE) }) as u64;
// LOWERING-NEXT:     n = _v40;
// LOWERING-NEXT:     let _v41: i8 = 0;
// LOWERING-NEXT:     let _v42: u64 = n;
// LOWERING-NEXT:     let _v43: *mut i8 = buf;
// LOWERING-NEXT:     let _v44: *mut i8 = unsafe { _v43.add(_v42 as usize) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v44 = _v41;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v45: *mut i8 = b"%zu %s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v46: u64 = n;
// LOWERING-NEXT:     let _v47: *mut i8 = buf;
// LOWERING-NEXT:     let _v48: i32 = unsafe { printf(_v45 as *const i8, _v46, _v47) };
// LOWERING-NEXT:     let _v49: *mut i8 = buf;
// LOWERING-NEXT:     let _v50: *mut core::ffi::c_void = _v49 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v50 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v51: *mut libc::FILE = g;
// LOWERING-NEXT:     let _v52: i32 = unsafe { fclose(_v51 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v53: *mut i8 = b"slate_stdio_fread_unsupported_buffer.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v54: i32 = unsafe { remove(_v53 as *const i8) };
// LOWERING-NEXT:     let _v55: i32 = 0;
// LOWERING-NEXT:     __retval = _v55;
// LOWERING-NEXT:     let _v56: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v56 as i32);
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
// REWRITES-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn fread(_0: *mut core::ffi::c_void, _1: usize, _2: usize, _3: *mut libc::FILE) -> usize;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut f: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut g: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut buf: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut n: u64 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"slate_stdio_fread_unsupported_buffer.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = unsafe { remove(_v1 as *const i8) };
// REWRITES-NEXT: let _v3: *mut i8 = b"slate_stdio_fread_unsupported_buffer.tmp\0".as_ptr() as *mut i8;
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
// REWRITES-NEXT: let _v13: *mut i8 = b"heap-owned\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i32 = unsafe { fputs(_v13 as *const i8, f as *mut libc::FILE) };
// REWRITES-NEXT: let _v17: i32 = unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT: let _v18: *mut i8 = b"slate_stdio_fread_unsupported_buffer.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: *mut i8 = b"r\0".as_ptr() as *mut i8;
// REWRITES-NEXT: g = unsafe { fopen(_v18 as *const i8, _v19 as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v22: bool = g != std::ptr::null_mut();
// REWRITES-NEXT:         let _v23: bool = !_v22;
// REWRITES-NEXT:         if _v23 {
// REWRITES-NEXT:                     let _v24: *mut i8 = b"reopen-fail\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v25: i32 = unsafe { puts(_v24 as *const i8) };
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v28: u64 = 16;
// REWRITES-NEXT: let _v29: *mut core::ffi::c_void = unsafe { malloc(_v28 as usize) };
// REWRITES-NEXT: buf = _v29 as *mut i8;
// REWRITES-NEXT: let _v31: i8 = 0;
// REWRITES-NEXT: let _v32: i64 = 0;
// REWRITES-NEXT: let _v33: *mut i8 = buf;
// REWRITES-NEXT: let _v34: *mut i8 = unsafe { _v33.add(0) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v34 = _v31;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v37: u64 = 1;
// REWRITES-NEXT: let _v38: u64 = 10;
// REWRITES-NEXT: n = (unsafe { fread((buf as *mut core::ffi::c_void) as *mut core::ffi::c_void, _v37 as usize, _v38 as usize, g as *mut libc::FILE) }) as u64;
// REWRITES-NEXT: let _v41: i8 = 0;
// REWRITES-NEXT: let _v43: *mut i8 = buf;
// REWRITES-NEXT: let _v44: *mut i8 = unsafe { _v43.add(n as usize) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v44 = _v41;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v45: *mut i8 = b"%zu %s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v48: i32 = unsafe { printf(_v45 as *const i8, n, buf) };
// REWRITES-NEXT: unsafe { free((buf as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v52: i32 = unsafe { fclose(g as *mut libc::FILE) };
// REWRITES-NEXT: let _v53: *mut i8 = b"slate_stdio_fread_unsupported_buffer.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v54: i32 = unsafe { remove(_v53 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
