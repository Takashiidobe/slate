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
  printf("%d %d %d %zu %zu %zu %zu %zu %zu\n",
         bounded_cmp(abc, abd, 2) == 0, bounded_cmp(abc, abd, 3) < 0,
         bounded_cmp(abc, abd, 0) == 0, bounded_len(abc, 99),
         bounded_len(text, 3), bounded_len(empty, 7), spans(span, accept),
         spans(span, empty), spans(empty, reject));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn strncmp(_0: *const i8, _1: *const i8, _2: usize) -> i32;
// LOWERING-NEXT:     fn strnlen(_0: *const i8, _1: usize) -> usize;
// LOWERING-NEXT:     fn strspn(_0: *const i8, _1: *const i8) -> usize;
// LOWERING-NEXT:     fn strcspn(_0: *const i8, _1: *const i8) -> usize;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn bounded_cmp({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strncmp({{arg[0-9]+}} as *const i8, {{arg[0-9]+}} as *const i8, {{arg[0-9]+}} as usize) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn bounded_len({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> u64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { strnlen({{arg[0-9]+}} as *const i8, {{arg[0-9]+}} as usize) }) as u64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn spans({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> u64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { strspn({{arg[0-9]+}} as *const i8, {{arg[0-9]+}} as *const i8) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { strcspn({{arg[0-9]+}} as *const i8, {{arg[0-9]+}} as *const i8) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut abc: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut abd: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut text: [i8; 7] = [0; 7];
// LOWERING-NEXT:     let mut span: [i8; 7] = [0; 7];
// LOWERING-NEXT:     let mut accept: [i8; 3] = [0; 3];
// LOWERING-NEXT:     let mut empty: [i8; 1] = [0; 1];
// LOWERING-NEXT:     let mut reject: [i8; 3] = [0; 3];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     abc = [97, 98, 99, 0];
// LOWERING-NEXT:     abd = [97, 98, 100, 0];
// LOWERING-NEXT:     text = [97, 98, 99, 100, 101, 102, 0];
// LOWERING-NEXT:     span = [97, 98, 97, 99, 97, 100, 0];
// LOWERING-NEXT:     accept = [97, 98, 0];
// LOWERING-NEXT:     empty = [0];
// LOWERING-NEXT:     reject = [99, 100, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %zu %zu %zu %zu %zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = abd.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = bounded_cmp({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = abd.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = bounded_cmp({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = abd.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = bounded_cmp({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 99;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = bounded_len({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = text.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = bounded_len({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = bounded_len({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = span.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = accept.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = spans({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = span.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = spans({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = reject.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = spans({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-NOT: unsafe { strncmp(
// REWRITES-NOT: unsafe { strnlen(
// REWRITES-NOT: unsafe { strspn(
// REWRITES-NOT: unsafe { strcspn(
