#include <stdio.h>
#include <string.h>

static int score_text(const unsigned char *bytes, int len) {
  (void)strlen((const char *)bytes);
  int score = 0;
  for (int i = 0; i < len; ++i)
    score += bytes[i];
  return score;
}

static int forward_text(const unsigned char *bytes, int len) {
  return score_text(bytes, len);
}

int main(void) {
  const unsigned char bytes[] = "abc";
  int                 score   = forward_text(bytes, 3);
  printf("%d\n", score);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: fn score_text(bytes: *const u8, len: i32) -> i32 {
// CHECK-NEXT:     unsafe { strlen(bytes as *const i8) };
// CHECK-NEXT:     let mut score: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         score += ((unsafe { *bytes.offset(i as isize) }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     score
// CHECK-NEXT: }
// CHECK-NEXT: fn forward_text(bytes: *const u8, len: i32) -> i32 {
// CHECK-NEXT:     score_text(bytes, len)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut bytes: [u8; 4] = [97, 98, 99, 0];
// CHECK-NEXT:     let score: i32 = forward_text(bytes.as_ptr() as *const u8, 3 as i32);
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
