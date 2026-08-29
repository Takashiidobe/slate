#include <stdio.h>

int main(void) {
  remove("slate_stdio_gets_loop_lines.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_lines.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("first\n", f);
  fputs("second\n", f);
  fputs("third", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_lines.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[64];
  while (fgets(line, sizeof line, g) != NULL) {
    fputs(line, stdout);
  }
  fclose(g);
  remove("slate_stdio_gets_loop_lines.tmp");
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
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"slate_stdio_gets_loop_lines.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = unsafe { remove(_v1 as *const i8) };
// LOWERING-NEXT:     let _v3: *mut i8 = b"slate_stdio_gets_loop_lines.tmp\0".as_ptr() as *mut i8;
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
// LOWERING-NEXT:     let _v13: *mut i8 = b"first\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v15: i32 = unsafe { fputs(_v13 as *const i8, _v14 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v16: *mut i8 = b"second\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v18: i32 = unsafe { fputs(_v16 as *const i8, _v17 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v19: *mut i8 = b"third\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v21: i32 = unsafe { fputs(_v19 as *const i8, _v20 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v22: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v23: i32 = unsafe { fclose(_v22 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v24: *mut i8 = b"slate_stdio_gets_loop_lines.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: *mut i8 = b"r\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v26: *mut libc::FILE = unsafe { fopen(_v24 as *const i8, _v25 as *const i8) };
// LOWERING-NEXT:     g = _v26;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v27: *mut libc::FILE = g;
// LOWERING-NEXT:         let _v28: bool = _v27 != std::ptr::null_mut();
// LOWERING-NEXT:         let _v29: bool = !_v28;
// LOWERING-NEXT:         if _v29 {
// LOWERING-NEXT:             let _v30: *mut i8 = b"reopen-fail\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v31: i32 = unsafe { puts(_v30 as *const i8) };
// LOWERING-NEXT:             let _v32: i32 = 0;
// LOWERING-NEXT:             __retval = _v32;
// LOWERING-NEXT:             let _v33: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v33 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v34: *mut i8 = line.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:             let _v35: i32 = 64;
// LOWERING-NEXT:             let _v36: *mut libc::FILE = g;
// LOWERING-NEXT:             let _v37: *mut i8 = unsafe { fgets(_v34 as *mut i8, _v35 as i32, _v36 as *mut libc::FILE) };
// LOWERING-NEXT:             let _v38: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:             let _v39: bool = _v37 != _v38;
// LOWERING-NEXT:             if !_v39 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v40: *mut i8 = line.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v41: *mut libc::FILE = unsafe { stdout };
// LOWERING-NEXT:                 let _v42: i32 = unsafe { fputs(_v40 as *const i8, _v41 as *mut libc::FILE) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v43: *mut libc::FILE = g;
// LOWERING-NEXT:     let _v44: i32 = unsafe { fclose(_v43 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v45: *mut i8 = b"slate_stdio_gets_loop_lines.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v46: i32 = unsafe { remove(_v45 as *const i8) };
// LOWERING-NEXT:     let _v47: i32 = 0;
// LOWERING-NEXT:     __retval = _v47;
// LOWERING-NEXT:     let _v48: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v48 as i32);
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
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"slate_stdio_gets_loop_lines.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = unsafe { remove(_v1 as *const i8) };
// REWRITES-NEXT: let _v3: *mut i8 = b"slate_stdio_gets_loop_lines.tmp\0".as_ptr() as *mut i8;
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
// REWRITES-NEXT: let _v13: *mut i8 = b"first\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i32 = unsafe { fputs(_v13 as *const i8, f as *mut libc::FILE) };
// REWRITES-NEXT: let _v16: *mut i8 = b"second\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: i32 = unsafe { fputs(_v16 as *const i8, f as *mut libc::FILE) };
// REWRITES-NEXT: let _v19: *mut i8 = b"third\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v21: i32 = unsafe { fputs(_v19 as *const i8, f as *mut libc::FILE) };
// REWRITES-NEXT: let _v23: i32 = unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT: let _v24: *mut i8 = b"slate_stdio_gets_loop_lines.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v25: *mut i8 = b"r\0".as_ptr() as *mut i8;
// REWRITES-NEXT: g = unsafe { fopen(_v24 as *const i8, _v25 as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v28: bool = g != std::ptr::null_mut();
// REWRITES-NEXT:         let _v29: bool = !_v28;
// REWRITES-NEXT:         if _v29 {
// REWRITES-NEXT:                     let _v30: *mut i8 = b"reopen-fail\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v31: i32 = unsafe { puts(_v30 as *const i8) };
// REWRITES-NEXT:                     __retval = 0;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let _v34: *mut i8 = line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:         let _v35: i32 = 64;
// REWRITES-NEXT:         let _v37: *mut i8 = unsafe { fgets(_v34 as *mut i8, _v35 as i32, g as *mut libc::FILE) };
// REWRITES-NEXT:         let _v38: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         if !(_v37 != _v38) {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let _v40: *mut i8 = line.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v42: i32 = unsafe { fputs(_v40 as *const i8, (unsafe { stdout }) as *mut libc::FILE) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v44: i32 = unsafe { fclose(g as *mut libc::FILE) };
// REWRITES-NEXT: let _v45: *mut i8 = b"slate_stdio_gets_loop_lines.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v46: i32 = unsafe { remove(_v45 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
