#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  char path[] = "slate-XXXXXX";
  int  fd     = mkstemp(path);
  printf("%d\n", fd >= 0);
  if (fd >= 0) {
    close(fd);
    unlink(path);
  }
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn mkstemp(_0: *mut i8) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn close(_0: i32) -> i32;
// LOWERING-NEXT:     fn unlink(_0: *const i8) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut path: [i8; 13] = [0; 13];
// LOWERING-NEXT:     let mut fd: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     path = [115, 108, 97, 116, 101, 45, 88, 88, 88, 88, 88, 88, 0];
// LOWERING-NEXT:     let _v1: *mut i8 = path.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = unsafe { mkstemp(_v1 as *mut i8) };
// LOWERING-NEXT:     fd = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: i32 = fd;
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     let _v6: bool = _v4 >= _v5;
// LOWERING-NEXT:     let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v3 as *const i8, _v7) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v9: i32 = fd;
// LOWERING-NEXT:         let _v10: i32 = 0;
// LOWERING-NEXT:         let _v11: bool = _v9 >= _v10;
// LOWERING-NEXT:         if _v11 {
// LOWERING-NEXT:             let _v12: i32 = fd;
// LOWERING-NEXT:             let _v13: i32 = unsafe { close(_v12 as i32) };
// LOWERING-NEXT:             let _v14: *mut i8 = path.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:             let _v15: i32 = unsafe { unlink(_v14 as *const i8) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v16: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v16 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn mkstemp(_0: *mut i8) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn close(_0: i32) -> i32;
// REWRITES-NEXT:     fn unlink(_0: *const i8) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut path: [i8; 13] = [0; 13];
// REWRITES-NEXT: let mut fd: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: path = [115, 108, 97, 116, 101, 45, 88, 88, 88, 88, 88, 88, 0];
// REWRITES-NEXT: let _v1: *mut i8 = path.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: fd = unsafe { mkstemp(_v1 as *mut i8) };
// REWRITES-NEXT: let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = 0;
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v3 as *const i8, (fd >= _v5) as i32) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v10: i32 = 0;
// REWRITES-NEXT:         let _v11: bool = fd >= _v10;
// REWRITES-NEXT:         if _v11 {
// REWRITES-NEXT:                     let _v13: i32 = unsafe { close(fd as i32) };
// REWRITES-NEXT:                     let _v14: *mut i8 = path.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v15: i32 = unsafe { unlink(_v14 as *const i8) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
