#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  printf("%d %d %d\n", argc, argv[0] != NULL, argv[argc] == NULL);
  size_t length = strlen(argv[0]);
  printf("%d\n", length > 0);
  for (int i = 1; i < argc; i++)
    printf("%s\n", argv[i]);
  return argc - 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strlen([[ARG_0]]: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(argc: i32, argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", argc, ! unsafe { * argv.add(0) } .is_null() as i32, unsafe { * argv
// CHECK-NEXT:         .offset(argc as isize) } .is_null() as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let length: u64 = unsafe { strlen(*argv.add(0) as *const i8) };
// CHECK-NEXT:     println!("{}", (length > 0) as i32);
// CHECK-NEXT:     for i in 1..argc {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%s\n".as_ptr(), *argv.offset(i as isize)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     argc - 1
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
