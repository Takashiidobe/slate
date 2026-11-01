#include <stdio.h>

int main(void) {
  remove("slate_stdio_fread_size_one.tmp");
  FILE *f = fopen("slate_stdio_fread_size_one.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("hello world", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_fread_size_one.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char   buf[16] = {0};
  size_t n       = fread(buf, 1, 11, g);
  printf("%zu %s\n", n, buf);
  fclose(g);
  remove("slate_stdio_fread_size_one.tmp");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn remove([[ARG_0:arg[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     fn fread([[ARG_0]]: *mut (), [[ARG_1:arg[0-9]+]]: u64, [[ARG_2:arg[0-9]+]]: u64, [[ARG_3:arg[0-9]+]]: *mut slate_support::FILE) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_fread_size_one.tmp".as_ptr()) };
// CHECK-NEXT:     let f: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(c"slate_stdio_fread_size_one.tmp".as_ptr(), c"w".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     if f.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(c"open-fail".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::fputs(c"hello world".as_ptr(), f) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { slate_support::fclose(f) };
// CHECK-NEXT:     let g: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(c"slate_stdio_fread_size_one.tmp".as_ptr(), c"r".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     if g.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(c"reopen-fail".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let n: u64 = unsafe { fread(buf.0.as_mut_ptr() as *mut (), 1u64, 11u64, g) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%zu %s\n".as_ptr(), n, buf.0.as_mut_ptr() as *mut i8)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { slate_support::fclose(g) };
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_fread_size_one.tmp".as_ptr()) };
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
