#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int consume_text(char *text, int len) {
  (void)strlen(text);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += (unsigned char)text[i];
  free(text);
  return score;
}

static int forward_text_owner(char *text, int len) {
  return consume_text(text, len);
}

int main(void) {
  int   len  = 4;
  char *text = malloc(len * sizeof(char));
  memcpy(text, "abc", len);
  int score = forward_text_owner(text, len);
  printf("%d\n", score);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: fn consume_text(mut text: String) -> i32 {
// CHECK-NEXT:     unsafe { strlen(text.as_mut_ptr() as *const i8) };
// CHECK-NEXT:     let mut score: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < (text.len() as i32) {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = score;
// CHECK-NEXT:         score = [[V_0]]
// CHECK-NEXT:             + ((((unsafe { *(text.as_mut_ptr() as *mut i8).offset(i as isize) }) as u8)
// CHECK-NEXT:                 as u32) as i32);
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     score
// CHECK-NEXT: }
// CHECK-NEXT: fn forward_text_owner(text: String) -> i32 {
// CHECK-NEXT:     consume_text(text)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let len: i32 = 4;
// CHECK-NEXT:     let text: *mut i8 = (unsafe { malloc(((len as i64) as u64).wrapping_mul(1)) })
// CHECK-NEXT:         as *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             c"abc".as_ptr() as *const u8,
// CHECK-NEXT:             text as *mut u8,
// CHECK-NEXT:             ((len as i64) as u64) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let score: i32 = forward_text_owner(unsafe {
// CHECK-NEXT:         String::from_raw_parts(text as *mut u8, len as usize, len as usize)
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("{}", score);
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
