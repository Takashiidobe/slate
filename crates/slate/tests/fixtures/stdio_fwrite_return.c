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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn remove([[ARG_0:arg[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     fn fwrite(
// CHECK-NEXT:         [[ARG_0]]: *const (),
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: u64,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: u64,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *mut slate_support::FILE,
// CHECK-NEXT:     ) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_fwrite_return.tmp".as_ptr()) };
// CHECK-NEXT:     let f: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(c"slate_stdio_fwrite_return.tmp".as_ptr(), c"w".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     if f.is_null() {
// CHECK-NEXT:         unsafe { slate_support::puts(c"open-fail".as_ptr()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 16]> = __SlateAlign16([
// CHECK-NEXT:         48i8, 49i8, 50i8, 51i8, 52i8, 53i8, 54i8, 55i8, 56i8, 57i8, 97i8, 98i8, 99i8,
// CHECK-NEXT:         100i8, 101i8, 0i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let n: u64 = unsafe { fwrite(buf.0.as_mut_ptr() as *const (), 1u64, 15u64, f) };
// CHECK-NEXT:     unsafe { slate_support::printf(c"%zu\n".as_ptr(), n) };
// CHECK-NEXT:     unsafe { slate_support::fclose(f) };
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_fwrite_return.tmp".as_ptr()) };
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
