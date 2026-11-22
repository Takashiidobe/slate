#include <stdio.h>
#include <string.h>

static int is_keyword(const char *word) {
  return strcmp(word, "if") == 0 || strcmp(word, "while") == 0;
}

static int keyword_length(const char *word) {
  return is_keyword(word) ? (int)strlen(word) + (word[0] == 'w') : 0;
}

static int prefix(const char *word, const char *start) {
  return strncmp(word, start, strlen(start)) == 0;
}

static int skips(const char *word) {
  while (*word == ' ')
    word++;
  return (int)strlen(word);
}

static int embedded(const char *word) { return (int)strlen(word); }

int main(void) {
  char buffer[8] = "whilst";
  printf("%d %d\n", keyword_length("if"), keyword_length("while"));
  printf("%d %d\n", prefix("while", "wh"), prefix(buffer, "wh"));
  printf("%d %d\n", skips("  if"), embedded("a\0b"));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::strncmp;
// CHECK-NEXT: use slate_support::strlen;
// CHECK-EMPTY:
// CHECK-NEXT: fn is_keyword(word: &core::ffi::CStr) -> i32 {
// CHECK-NEXT:     (word == c"if" || word == c"while") as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn keyword_length(word: &core::ffi::CStr) -> i32 {
// CHECK-NEXT:     if is_keyword(word) != 0 {
// CHECK-NEXT:         ((word.count_bytes() as u64) as i32)
// CHECK-NEXT:             + (((unsafe { *(word.as_ptr() as *const i8).add(0) as i32 }) == 119) as i32)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn prefix(word: *const i8, start: &core::ffi::CStr) -> i32 {
// CHECK-NEXT:     ((unsafe {
// CHECK-NEXT:         strncmp(
// CHECK-NEXT:             word as *const core::ffi::c_char,
// CHECK-NEXT:             (start.as_ptr() as *const i8) as *const core::ffi::c_char,
// CHECK-NEXT:             (start.count_bytes() as u64) as usize,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     }) == 0) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn skips(mut word: *const i8) -> i32 {
// CHECK-NEXT:     while (unsafe { *word as i32 }) == 32 {
// CHECK-NEXT:         word = unsafe { word.add(1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { (strlen(word as *const core::ffi::c_char) as u64) as i32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn embedded(word: *const i8) -> i32 {
// CHECK-NEXT:     unsafe { (strlen(word as *const core::ffi::c_char) as u64) as i32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buffer: [i8; 8] = [119, 104, 105, 108, 115, 116, 0, 0];
// CHECK-NEXT:     println!("{} {}", keyword_length(c"if"), keyword_length(c"while"));
// CHECK-NEXT:     let __slate_printf_arg1 = prefix(buffer.as_mut_ptr() as *const i8, c"wh");
// CHECK-NEXT:     println!("{} {}", prefix(c"while".as_ptr(), c"wh"), __slate_printf_arg1);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         skips(c"  if".as_ptr()),
// CHECK-NEXT:         embedded(b"a\0b\0".as_ptr() as *const i8)
// CHECK-NEXT:     );
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
