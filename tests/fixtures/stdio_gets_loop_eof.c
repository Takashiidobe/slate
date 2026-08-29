#include <stdio.h>

int main(void) {
  remove("slate_stdio_gets_loop_eof.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_eof.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("only\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_eof.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[64];
  while (fgets(line, sizeof line, g) != NULL) {
    fputs(line, stdout);
  }
  fclose(g);
  puts("done");

  FILE *h = fopen("slate_stdio_gets_loop_eof.tmp", "w");
  if (!h) {
    puts("open-fail");
    return 0;
  }
  fclose(h);

  FILE *e = fopen("slate_stdio_gets_loop_eof.tmp", "r");
  if (!e) {
    puts("reopen-fail");
    return 0;
  }
  char empty_line[64];
  while (fgets(empty_line, sizeof empty_line, e) != NULL) {
    fputs(empty_line, stdout);
  }
  fclose(e);
  puts("empty-done");
  remove("slate_stdio_gets_loop_eof.tmp");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     static mut stdout: *mut libc::FILE;
// LOWERING-NEXT:     fn remove(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// LOWERING-NEXT:     fn puts(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn fgets(_0: *mut i8, _1: i32, _2: *mut libc::FILE) -> *mut i8;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut g: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut line: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-NEXT:     let mut h: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut e: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut empty_line: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = unsafe { remove(_v1 as *const i8) };
// LOWERING-NEXT:     let _v3: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
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
// LOWERING-NEXT:     let _v13: *mut i8 = b"only\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v15: i32 = unsafe { fputs(_v13 as *const i8, _v14 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v16: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v17: i32 = unsafe { fclose(_v16 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v18: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
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
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v28: *mut i8 = line.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:             let _v29: i32 = 64;
// LOWERING-NEXT:             let _v30: *mut libc::FILE = g;
// LOWERING-NEXT:             let _v31: *mut i8 = unsafe { fgets(_v28 as *mut i8, _v29 as i32, _v30 as *mut libc::FILE) };
// LOWERING-NEXT:             let _v32: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:             let _v33: bool = _v31 != _v32;
// LOWERING-NEXT:             if !_v33 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v34: *mut i8 = line.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v35: *mut libc::FILE = unsafe { stdout };
// LOWERING-NEXT:                 let _v36: i32 = unsafe { fputs(_v34 as *const i8, _v35 as *mut libc::FILE) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v37: *mut libc::FILE = g;
// LOWERING-NEXT:     let _v38: i32 = unsafe { fclose(_v37 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v39: *mut i8 = b"done\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v40: i32 = unsafe { puts(_v39 as *const i8) };
// LOWERING-NEXT:     let _v41: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v42: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v43: *mut libc::FILE = unsafe { fopen(_v41 as *const i8, _v42 as *const i8) };
// LOWERING-NEXT:     h = _v43;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v44: *mut libc::FILE = h;
// LOWERING-NEXT:         let _v45: bool = _v44 != std::ptr::null_mut();
// LOWERING-NEXT:         let _v46: bool = !_v45;
// LOWERING-NEXT:         if _v46 {
// LOWERING-NEXT:             let _v47: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v48: i32 = unsafe { puts(_v47 as *const i8) };
// LOWERING-NEXT:             let _v49: i32 = 0;
// LOWERING-NEXT:             __retval = _v49;
// LOWERING-NEXT:             let _v50: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v50 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v51: *mut libc::FILE = h;
// LOWERING-NEXT:     let _v52: i32 = unsafe { fclose(_v51 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v53: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v54: *mut i8 = b"r\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v55: *mut libc::FILE = unsafe { fopen(_v53 as *const i8, _v54 as *const i8) };
// LOWERING-NEXT:     e = _v55;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v56: *mut libc::FILE = e;
// LOWERING-NEXT:         let _v57: bool = _v56 != std::ptr::null_mut();
// LOWERING-NEXT:         let _v58: bool = !_v57;
// LOWERING-NEXT:         if _v58 {
// LOWERING-NEXT:             let _v59: *mut i8 = b"reopen-fail\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v60: i32 = unsafe { puts(_v59 as *const i8) };
// LOWERING-NEXT:             let _v61: i32 = 0;
// LOWERING-NEXT:             __retval = _v61;
// LOWERING-NEXT:             let _v62: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v62 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v63: *mut i8 = empty_line.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:             let _v64: i32 = 64;
// LOWERING-NEXT:             let _v65: *mut libc::FILE = e;
// LOWERING-NEXT:             let _v66: *mut i8 = unsafe { fgets(_v63 as *mut i8, _v64 as i32, _v65 as *mut libc::FILE) };
// LOWERING-NEXT:             let _v67: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:             let _v68: bool = _v66 != _v67;
// LOWERING-NEXT:             if !_v68 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v69: *mut i8 = empty_line.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v70: *mut libc::FILE = unsafe { stdout };
// LOWERING-NEXT:                 let _v71: i32 = unsafe { fputs(_v69 as *const i8, _v70 as *mut libc::FILE) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v72: *mut libc::FILE = e;
// LOWERING-NEXT:     let _v73: i32 = unsafe { fclose(_v72 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v74: *mut i8 = b"empty-done\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v75: i32 = unsafe { puts(_v74 as *const i8) };
// LOWERING-NEXT:     let _v76: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v77: i32 = unsafe { remove(_v76 as *const i8) };
// LOWERING-NEXT:     let _v78: i32 = 0;
// LOWERING-NEXT:     __retval = _v78;
// LOWERING-NEXT:     let _v79: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v79 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     static mut stdout: *mut libc::FILE;
// REWRITES-NEXT:     fn remove(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// REWRITES-NEXT:     fn puts(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fgets(_0: *mut i8, _1: i32, _2: *mut libc::FILE) -> *mut i8;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut f: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut g: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut line: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-NEXT: let mut h: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut e: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut empty_line: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = unsafe { remove(_v1 as *const i8) };
// REWRITES-NEXT: let _v3: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
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
// REWRITES-NEXT: let _v13: *mut i8 = b"only\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i32 = unsafe { fputs(_v13 as *const i8, f as *mut libc::FILE) };
// REWRITES-NEXT: let _v17: i32 = unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT: let _v18: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
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
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let _v28: *mut i8 = line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:         let _v29: i32 = 64;
// REWRITES-NEXT:         let _v31: *mut i8 = unsafe { fgets(_v28 as *mut i8, _v29 as i32, g as *mut libc::FILE) };
// REWRITES-NEXT:         let _v32: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         if !(_v31 != _v32) {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let _v34: *mut i8 = line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v36: i32 = unsafe { fputs(_v34 as *const i8, (unsafe { stdout }) as *mut libc::FILE) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v38: i32 = unsafe { fclose(g as *mut libc::FILE) };
// REWRITES-NEXT: let _v39: *mut i8 = b"done\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v40: i32 = unsafe { puts(_v39 as *const i8) };
// REWRITES-NEXT: let _v41: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v42: *mut i8 = b"w\0".as_ptr() as *mut i8;
// REWRITES-NEXT: h = unsafe { fopen(_v41 as *const i8, _v42 as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v45: bool = h != std::ptr::null_mut();
// REWRITES-NEXT:         let _v46: bool = !_v45;
// REWRITES-NEXT:         if _v46 {
// REWRITES-NEXT:                     let _v47: *mut i8 = b"open-fail\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v48: i32 = unsafe { puts(_v47 as *const i8) };
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v52: i32 = unsafe { fclose(h as *mut libc::FILE) };
// REWRITES-NEXT: let _v53: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v54: *mut i8 = b"r\0".as_ptr() as *mut i8;
// REWRITES-NEXT: e = unsafe { fopen(_v53 as *const i8, _v54 as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v57: bool = e != std::ptr::null_mut();
// REWRITES-NEXT:         let _v58: bool = !_v57;
// REWRITES-NEXT:         if _v58 {
// REWRITES-NEXT:                     let _v59: *mut i8 = b"reopen-fail\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v60: i32 = unsafe { puts(_v59 as *const i8) };
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let _v63: *mut i8 = empty_line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:         let _v64: i32 = 64;
// REWRITES-NEXT:         let _v66: *mut i8 = unsafe { fgets(_v63 as *mut i8, _v64 as i32, e as *mut libc::FILE) };
// REWRITES-NEXT:         let _v67: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         if !(_v66 != _v67) {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let _v69: *mut i8 = empty_line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v71: i32 = unsafe { fputs(_v69 as *const i8, (unsafe { stdout }) as *mut libc::FILE) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v73: i32 = unsafe { fclose(e as *mut libc::FILE) };
// REWRITES-NEXT: let _v74: *mut i8 = b"empty-done\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v75: i32 = unsafe { puts(_v74 as *const i8) };
// REWRITES-NEXT: let _v76: *mut i8 = b"slate_stdio_gets_loop_eof.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v77: i32 = unsafe { remove(_v76 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
