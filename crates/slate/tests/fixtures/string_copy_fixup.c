#include <stdio.h>
#include <string.h>

int main(void) {
  char copy[16]         = {0};
  char append[16]       = "foo";
  char trunc_copy[16]   = {0};
  char trunc_append[16] = "pre";

  strcpy(copy, "abc");
  strcat(append, "bar");
  strncpy(trunc_copy, "abcdef", 3);
  strncat(trunc_append, "suffix", 3);

  printf("%s %s %s %s %zu\n", copy, append, trunc_copy, trunc_append,
         strlen(trunc_append));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strcpy(__dest: *mut i8, __src: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strncpy(__dest: *mut i8, __src: *const i8, __n: u64) -> *mut i8;
// CHECK-NEXT:     fn strcat(__dest: *mut i8, __src: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strncat(__dest: *mut i8, __src: *const i8, __n: u64) -> *mut i8;
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut copy: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0 as i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut append: __SlateAlign16<[i8; 16]> = __SlateAlign16([
// CHECK-NEXT:         102 as i8,
// CHECK-NEXT:         111 as i8,
// CHECK-NEXT:         111 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut trunc_copy: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]][0] = 0 as i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut trunc_append: __SlateAlign16<[i8; 16]> = __SlateAlign16([
// CHECK-NEXT:         112 as i8,
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         101 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     unsafe { strcpy(copy.0.as_mut_ptr() as *mut i8, c"abc".as_ptr()) };
// CHECK-NEXT:     unsafe { strcat(append.0.as_mut_ptr() as *mut i8, c"bar".as_ptr()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strncpy(trunc_copy.0.as_mut_ptr() as *mut i8, c"abcdef".as_ptr(), 3 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strncat(trunc_append.0.as_mut_ptr() as *mut i8, c"suffix".as_ptr(), 3 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %s %s %s %zu\n".as_ptr(),
// CHECK-NEXT:             copy.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             append.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             trunc_copy.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             trunc_append.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             unsafe { strlen(trunc_append.0.as_mut_ptr() as *const i8) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
