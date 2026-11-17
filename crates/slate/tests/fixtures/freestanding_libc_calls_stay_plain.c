/* { dg-options "-ffreestanding" } */
#include <stdlib.h>
#include <string.h>

static int parse_num(char *s) { return atoi(s); }

static int text_len(const char *s) { return (int)strlen(s); }

int main(void) {
  char dst[8];
  char src[8] = "1234567";
  memcpy(dst, src, sizeof dst);
  memset(src, 0, sizeof src);
  int same = memcmp(dst, "1234567", 8) == 0 && strcmp(dst, "1234567") == 0;
  return same && src[0] == 0 && text_len(dst) == 7 && parse_num(dst) == 1234567 ? 0 : 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-ffreestanding" }
// CHECK-NEXT: use slate_support::atoi;
// CHECK-NEXT: use slate_support::memcmp;
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: use slate_support::memset;
// CHECK-NEXT: use slate_support::strcmp;
// CHECK-NEXT: use slate_support::strlen;
// CHECK-EMPTY:
// CHECK-NEXT: fn parse_num(s: *mut i8) -> i32 {
// CHECK-NEXT:     unsafe { atoi((s as *const i8) as *const core::ffi::c_char) as i32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn text_len(s: *const i8) -> i32 {
// CHECK-NEXT:     (unsafe { strlen(s as *const core::ffi::c_char) as u64 }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C" fn main() -> i32 {
// CHECK-NEXT:     let mut dst: [i8; 8] = [0i8; 8];
// CHECK-NEXT:     let mut src: [i8; 8] = [49, 50, 51, 52, 53, 54, 55, 0];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memcpy(
// CHECK-NEXT:             (dst.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void,
// CHECK-NEXT:             (src.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             (core::mem::size_of::<[i8; 8]>() as u64) as usize,
// CHECK-NEXT:         ) as *mut ()
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (src.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void,
// CHECK-NEXT:             0i32 as i32,
// CHECK-NEXT:             (core::mem::size_of::<[i8; 8]>() as u64) as usize,
// CHECK-NEXT:         ) as *mut ()
// CHECK-NEXT:     };
// CHECK-NEXT:     let same: i32 = ((unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             (dst.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             (b"1234567\0".as_ptr() as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             8u64 as usize,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) == 0
// CHECK-NEXT:         && (unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 (dst.as_mut_ptr() as *const i8) as *const core::ffi::c_char,
// CHECK-NEXT:                 c"1234567".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         }) == 0) as i32;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool =
// CHECK-NEXT:         same != 0 && (src[0] as i32) == 0 && text_len(dst.as_mut_ptr() as *const i8) == 7;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = [[V_0]] && parse_num(dst.as_mut_ptr() as *mut i8) == 1234567;
// CHECK-NEXT:     if [[V_1]] { 0 } else { 1 }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
