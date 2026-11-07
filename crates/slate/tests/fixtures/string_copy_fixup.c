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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: use slate_support::strcpy;
// CHECK-NEXT: use slate_support::strncpy;
// CHECK-NEXT: use slate_support::strcat;
// CHECK-NEXT: use slate_support::strncat;
// CHECK-NEXT: use slate_support::strlen;
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut copy: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut append: __SlateAlign16<[i8; 16]> = __SlateAlign16([
// CHECK-NEXT:         102i8, 111i8, 111i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT:         0i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut trunc_copy: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]][0] = 0i8;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut trunc_append: __SlateAlign16<[i8; 16]> = __SlateAlign16([
// CHECK-NEXT:         112i8, 114i8, 101i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8, 0i8,
// CHECK-NEXT:         0i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             (copy.0.as_mut_ptr() as *mut i8) as *mut core::ffi::c_char,
// CHECK-NEXT:             c"abc".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcat(
// CHECK-NEXT:             (append.0.as_mut_ptr() as *mut i8) as *mut core::ffi::c_char,
// CHECK-NEXT:             c"bar".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strncpy(
// CHECK-NEXT:             (trunc_copy.0.as_mut_ptr() as *mut i8) as *mut core::ffi::c_char,
// CHECK-NEXT:             c"abcdef".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:             3u64 as usize,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strncat(
// CHECK-NEXT:             (trunc_append.0.as_mut_ptr() as *mut i8) as *mut core::ffi::c_char,
// CHECK-NEXT:             c"suffix".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:             3u64 as usize,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %s %s %s %zu\n".as_ptr(),
// CHECK-NEXT:             copy.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             append.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             trunc_copy.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             trunc_append.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             strlen(
// CHECK-NEXT:                 (trunc_append.0.as_mut_ptr() as *const i8) as *const core::ffi::c_char,
// CHECK-NEXT:             ) as u64,
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
