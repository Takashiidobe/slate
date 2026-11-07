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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fread([[ARG_0:arg[0-9]+]]: *mut (), [[ARG_1:arg[0-9]+]]: u64, [[ARG_2:arg[0-9]+]]: u64, [[ARG_3:arg[0-9]+]]: *mut slate_support::FILE) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::remove;
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         remove(
// CHECK-NEXT:             c"slate_stdio_fread_unsupported_buffer.tmp".as_ptr()
// CHECK-NEXT:                 as *const core::ffi::c_char,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let f: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(
// CHECK-NEXT:             c"slate_stdio_fread_unsupported_buffer.tmp".as_ptr(),
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
// CHECK-NEXT:     unsafe { slate_support::fputs(c"heap-owned".as_ptr(), f) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { slate_support::fclose(f) };
// CHECK-NEXT:     let g: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(
// CHECK-NEXT:             c"slate_stdio_fread_unsupported_buffer.tmp".as_ptr(),
// CHECK-NEXT:             c"r".as_ptr(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if g.is_null() {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::puts(c"reopen-fail".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let buf: *mut i8 = (unsafe { malloc(16u64 as usize) as *mut () }) as *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *buf.add(0) = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let n: u64 = unsafe { fread(buf as *mut (), 1u64, 10u64, g) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *buf.add(n as usize) = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%zu %s\n".as_ptr(), n, buf) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { free((buf as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     unsafe { slate_support::fclose(g) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         remove(
// CHECK-NEXT:             c"slate_stdio_fread_unsupported_buffer.tmp".as_ptr()
// CHECK-NEXT:                 as *const core::ffi::c_char,
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
