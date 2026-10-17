#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>

static int bounded_cmp(const char *a, const char *b, size_t n) {
  return strncmp(a, b, n);
}

static size_t bounded_len(const char *s, size_t n) { return strnlen(s, n); }

static size_t spans(const char *s, const char *set) {
  return strspn(s, set) * 100 + strcspn(s, set);
}

int main(void) {
  const char abc[]    = "abc";
  const char abd[]    = "abd";
  const char text[]   = "abcdef";
  const char span[]   = "abacad";
  const char accept[] = "ab";
  const char empty[]  = "";
  const char reject[] = "cd";
  printf("%d %d %d %zu %zu %zu %zu %zu %zu\n", bounded_cmp(abc, abd, 2) == 0,
         bounded_cmp(abc, abd, 3) < 0, bounded_cmp(abc, abd, 0) == 0,
         bounded_len(abc, 99), bounded_len(text, 3), bounded_len(empty, 7),
         spans(span, accept), spans(span, empty), spans(empty, reject));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strncmp(__s1: *const i8, __s2: *const i8, __n: u64) -> i32;
// CHECK-NEXT:     fn strcspn(__s: *const i8, __reject: *const i8) -> u64;
// CHECK-NEXT:     fn strspn(__s: *const i8, __accept: *const i8) -> u64;
// CHECK-NEXT:     fn strnlen(__string: *const i8, __maxlen: u64) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn bounded_cmp(a: *const i8, b: *const i8, n: u64) -> i32 {
// CHECK-NEXT:     unsafe { strncmp(a, b, n) }
// CHECK-NEXT: }
// CHECK-NEXT: fn bounded_len(s: *const i8, n: u64) -> u64 {
// CHECK-NEXT:     unsafe { strnlen(s, n) }
// CHECK-NEXT: }
// CHECK-NEXT: fn spans(s: *const i8, set: *const i8) -> u64 {
// CHECK-NEXT:     unsafe { strspn(s, set) }.wrapping_mul(100).wrapping_add(unsafe { strcspn(s, set) })
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut abc: [i8; 4] = [97, 98, 99, 0];
// CHECK-NEXT:     let mut abd: [i8; 4] = [97, 98, 100, 0];
// CHECK-NEXT:     let mut text: [i8; 7] = [97, 98, 99, 100, 101, 102, 0];
// CHECK-NEXT:     let mut span: [i8; 7] = [97, 98, 97, 99, 97, 100, 0];
// CHECK-NEXT:     let mut accept: [i8; 3] = [97, 98, 0];
// CHECK-NEXT:     let mut empty: [i8; 1] = [0];
// CHECK-NEXT:     let mut reject: [i8; 3] = [99, 100, 0];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %zu %zu %zu %zu %zu %zu\n".as_ptr(),
// CHECK-NEXT:             (bounded_cmp(abc.as_ptr() as *const i8, abd.as_ptr() as *const i8, 2 as u64)
// CHECK-NEXT:                 == 0) as i32,
// CHECK-NEXT:             (bounded_cmp(abc.as_ptr() as *const i8, abd.as_ptr() as *const i8, 3 as u64)
// CHECK-NEXT:                 < 0) as i32,
// CHECK-NEXT:             (bounded_cmp(abc.as_ptr() as *const i8, abd.as_ptr() as *const i8, 0 as u64)
// CHECK-NEXT:                 == 0) as i32,
// CHECK-NEXT:             bounded_len(abc.as_ptr() as *const i8, 99 as u64),
// CHECK-NEXT:             bounded_len(text.as_ptr() as *const i8, 3 as u64),
// CHECK-NEXT:             bounded_len(empty.as_ptr() as *const i8, 7 as u64),
// CHECK-NEXT:             spans(span.as_ptr() as *const i8, accept.as_ptr() as *const i8),
// CHECK-NEXT:             spans(span.as_ptr() as *const i8, empty.as_ptr() as *const i8),
// CHECK-NEXT:             spans(empty.as_ptr() as *const i8, reject.as_ptr() as *const i8),
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
