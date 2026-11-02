#include <stdio.h>

int main(void) {
  remove("slate_stdio_gets_loop_truncation.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_truncation.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("0123456789abcdef\n", f);
  fputs("short\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_truncation.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[8];
  while (fgets(line, sizeof line, g) != NULL) {
    fputs(line, stdout);
  }
  fclose(g);
  remove("slate_stdio_gets_loop_truncation.tmp");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fgets([[ARG_0:arg[0-9]+]]: *mut i8, [[ARG_1:arg[0-9]+]]: i32, [[ARG_2:arg[0-9]+]]: *mut slate_support::FILE) -> *mut i8;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::remove;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         remove(
// CHECK-NEXT:             c"slate_stdio_gets_loop_truncation.tmp".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let f: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(
// CHECK-NEXT:             c"slate_stdio_gets_loop_truncation.tmp".as_ptr(),
// CHECK-NEXT:             c"w".as_ptr(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if f.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(c"open-fail".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::fputs(c"0123456789abcdef\n".as_ptr(), f) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::fputs(c"short\n".as_ptr(), f) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { slate_support::fclose(f) };
// CHECK-NEXT:     let g: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(
// CHECK-NEXT:             c"slate_stdio_gets_loop_truncation.tmp".as_ptr(),
// CHECK-NEXT:             c"r".as_ptr(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if g.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(c"reopen-fail".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut line: [i8; 8] = [0i8; 8];
// CHECK-NEXT:     while !unsafe {
// CHECK-NEXT:         fgets(line.as_mut_ptr() as *mut i8, std::mem::size_of::<[i8; 8]>() as i32, g)
// CHECK-NEXT:     }
// CHECK-NEXT:         .is_null()
// CHECK-NEXT:     {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::fputs(line.as_mut_ptr() as *const i8, slate_support::stdout())
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { slate_support::fclose(g) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         remove(
// CHECK-NEXT:             c"slate_stdio_gets_loop_truncation.tmp".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     };
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
