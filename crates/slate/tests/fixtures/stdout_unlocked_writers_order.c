#define _GNU_SOURCE
#include <stdio.h>

int main(void) {
  printf("a\n");
  putc_unlocked('b', stdout);
  putc_unlocked('\n', stdout);
  printf("c\n");
  putchar_unlocked('d');
  fputc_unlocked('\n', stdout);
  printf("e\n");
  fputs_unlocked("f\n", stdout);
  printf("g\n");
  fwrite_unlocked("h\n", 1, 2, stdout);
  printf("i\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fputc_unlocked([[ARG_0:arg[0-9]+]]: i32, [[ARG_1:arg[0-9]+]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT:     fn putc_unlocked([[ARG_0]]: i32, [[ARG_1]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT:     fn fputs_unlocked([[ARG_0]]: *const i8, [[ARG_1]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT:     fn fwrite_unlocked(
// CHECK-NEXT:         [[ARG_0]]: *const (),
// CHECK-NEXT:         [[ARG_1]]: u64,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: u64,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *mut slate_support::FILE,
// CHECK-NEXT:     ) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::putchar_unlocked;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("a");
// CHECK-NEXT:     unsafe { putc_unlocked(98i32, slate_support::stdout()) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { putc_unlocked(10i32, slate_support::stdout()) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     println!("c");
// CHECK-NEXT:     unsafe { putchar_unlocked(100i32 as i32) as i32 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { fputc_unlocked(10i32, slate_support::stdout()) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     println!("e");
// CHECK-NEXT:     unsafe { fputs_unlocked(c"f\n".as_ptr(), slate_support::stdout()) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     println!("g");
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         fwrite_unlocked(
// CHECK-NEXT:             b"h\n\0".as_ptr() as *const (),
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             2u64,
// CHECK-NEXT:             slate_support::stdout(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     println!("i");
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
