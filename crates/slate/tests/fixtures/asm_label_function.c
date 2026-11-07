#include <stdio.h>

int lib_cmp(const void *, const void *, unsigned long) __asm__("memcmp");

int main(void) {
  printf("%d %d\n", lib_cmp("abc", "abc", 3) == 0, lib_cmp("abc", "abd", 3) < 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn lib_cmp([[ARG_0:arg[0-9]+]]: *const (), [[ARG_1:arg[0-9]+]]: *const (), [[ARG_2:arg[0-9]+]]: u64) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", ((unsafe { lib_cmp(b"abc\0".as_ptr() as * const (), b"abc\0".as_ptr() as
// CHECK-NEXT:         * const (), 3u64) }) == 0) as i32, ((unsafe { lib_cmp(b"abc\0".as_ptr() as *
// CHECK-NEXT:         const (), b"abd\0".as_ptr() as * const (), 3u64) }) < 0) as i32
// CHECK-NEXT:     );
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
