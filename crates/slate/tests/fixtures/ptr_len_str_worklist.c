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
  int score = forward_text(bytes, 3);
  printf("%d\n", score);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn strlen(_0: *const i8) -> usize;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn score_text({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut bytes: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     let mut score: i32 = 0;
// LOWERING-NEXT:     bytes = {{arg[0-9]+}};
// LOWERING-NEXT:     len = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = bytes;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const i8) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     score = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = len;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut u8 = bytes;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = score;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             score = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = score;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn forward_text({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = score_text({{arg[0-9]+}}, {{arg[0-9]+}});
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut bytes: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     bytes = [97, 98, 99, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = bytes.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = forward_text({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: fn score_text(arg{{[0-9]+}}: &str) -> i32
// REWRITES-DAG: fn forward_text(arg{{[0-9]+}}: &str) -> i32
// REWRITES: std::str::from_utf8_unchecked(
