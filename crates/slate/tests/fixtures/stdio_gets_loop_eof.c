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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn remove([[ARG_0:arg[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     fn fgets([[ARG_0]]: *mut i8, [[ARG_1:arg[0-9]+]]: i32, [[ARG_2:arg[0-9]+]]: *mut slate_support::FILE) -> *mut i8;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_gets_loop_eof.tmp".as_ptr()) };
// CHECK-NEXT:     let f: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(c"slate_stdio_gets_loop_eof.tmp".as_ptr(), c"w".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     if f.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(c"open-fail".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::fputs(c"only\n".as_ptr(), f) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { slate_support::fclose(f) };
// CHECK-NEXT:     let g: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(c"slate_stdio_gets_loop_eof.tmp".as_ptr(), c"r".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     if g.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(c"reopen-fail".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut line: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     while !unsafe { fgets(line.0.as_mut_ptr() as *mut i8, 64i32, g) }.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::fputs(
// CHECK-NEXT:                 line.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { slate_support::fclose(g) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::puts(c"done".as_ptr()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let h: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(c"slate_stdio_gets_loop_eof.tmp".as_ptr(), c"w".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     if h.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(c"open-fail".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { slate_support::fclose(h) };
// CHECK-NEXT:     let e: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(c"slate_stdio_gets_loop_eof.tmp".as_ptr(), c"r".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     if e.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(c"reopen-fail".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut empty_line: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     while !unsafe { fgets(empty_line.0.as_mut_ptr() as *mut i8, 64i32, e) }.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::fputs(
// CHECK-NEXT:                 empty_line.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { slate_support::fclose(e) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::puts(c"empty-done".as_ptr()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_gets_loop_eof.tmp".as_ptr()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
