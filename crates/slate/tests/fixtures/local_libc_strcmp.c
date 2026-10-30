#include <stdio.h>
#include <string.h>

static int cmp_texts(const char *a, int alen, const char *b, int blen) {
  int sa = 0, sb = 0;
  for (int i = 0; i < alen; i++)
    sa += a[i];
  for (int i = 0; i < blen; i++)
    sb += b[i];
  int order = strcmp(a, b);
  int sign  = (order > 0) - (order < 0);
  int eq    = strcmp(a, b) == 0;
  return sign * 1000 + eq * 100 + (sa - sb);
}

int main(void) {
  const char x[] = "abc";
  const char y[] = "abd";
  printf("%d %d %d\n", cmp_texts(x, 3, y, 3), cmp_texts(y, 3, x, 3),
         cmp_texts(x, 3, x, 3));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strcmp([[ARG_0]]: *const i8, [[ARG_1:arg[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn cmp_texts(a: &str, b: &str) -> i32 {
// CHECK-NEXT:     let mut sa: i32 = 0;
// CHECK-NEXT:     let mut sb: i32 = 0;
// CHECK-NEXT:     for i in 0..a.len() as i32 {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = sa;
// CHECK-NEXT:         sa = [[V_0]] + ((unsafe { *(a.as_ptr() as *const i8).offset(i as isize) }) as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0..b.len() as i32 {
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = sb;
// CHECK-NEXT:         sb = [[V_1]] + ((unsafe { *(b.as_ptr() as *const i8).offset(i as isize) }) as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let order: i32 = unsafe { a.cmp(b) as i32 };
// CHECK-NEXT:     let sign: i32 = ((order > 0) as i32) - ((order < 0) as i32);
// CHECK-NEXT:     let eq: i32 = ((unsafe { strcmp(a.as_ptr() as *const i8, b.as_ptr() as *const i8) })
// CHECK-NEXT:         == 0) as i32;
// CHECK-NEXT:     sign * 1000 + eq * 100 + (sa - sb)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: [i8; 4] = [97, 98, 99, 0];
// CHECK-NEXT:     let mut y: [i8; 4] = [97, 98, 100, 0];
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", cmp_texts(unsafe {
// CHECK-NEXT:         std::str::from_utf8_unchecked(std::slice::from_raw_parts(x.as_ptr() as * const
// CHECK-NEXT:         u8, 3 as usize)) }, unsafe {
// CHECK-NEXT:         std::str::from_utf8_unchecked(std::slice::from_raw_parts(y.as_ptr() as * const
// CHECK-NEXT:         u8, 3 as usize)) }), cmp_texts(unsafe {
// CHECK-NEXT:         std::str::from_utf8_unchecked(std::slice::from_raw_parts(y.as_ptr() as * const
// CHECK-NEXT:         u8, 3 as usize)) }, unsafe {
// CHECK-NEXT:         std::str::from_utf8_unchecked(std::slice::from_raw_parts(x.as_ptr() as * const
// CHECK-NEXT:         u8, 3 as usize)) }), cmp_texts(unsafe {
// CHECK-NEXT:         std::str::from_utf8_unchecked(std::slice::from_raw_parts(x.as_ptr() as * const
// CHECK-NEXT:         u8, 3 as usize)) }, unsafe {
// CHECK-NEXT:         std::str::from_utf8_unchecked(std::slice::from_raw_parts(x.as_ptr() as * const
// CHECK-NEXT:         u8, 3 as usize)) })
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
