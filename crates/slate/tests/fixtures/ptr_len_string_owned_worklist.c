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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: use slate_support::strlen;
// CHECK-EMPTY:
// CHECK-NEXT: fn consume_text(text: *mut i8, len: i32) -> i32 {
// CHECK-NEXT:     unsafe { strlen((text as *const i8) as *const core::ffi::c_char) as u64 };
// CHECK-NEXT:     let mut score: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         score += ((unsafe { *text.offset(i as isize) }) as u8) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free((text as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     score
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn forward_text_owner(text: *mut i8, len: i32) -> i32 {
// CHECK-NEXT:     consume_text(text, len)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let len: i32 = 4;
// CHECK-NEXT:     let text: *mut i8 = (unsafe {
// CHECK-NEXT:         malloc((len as u64).wrapping_mul(core::mem::size_of::<i8>() as u64) as usize)
// CHECK-NEXT:             as *mut ()
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((b"abc\0".as_ptr() as *const ()) as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((text as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((len as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let score: i32 = forward_text_owner(text, len);
// CHECK-NEXT:     println!("{}", score);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
