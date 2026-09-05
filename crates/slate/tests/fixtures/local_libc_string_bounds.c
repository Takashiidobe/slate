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

// REWRITES-NOT: unsafe { strncmp(
// REWRITES-NOT: unsafe { strnlen(
// REWRITES-NOT: unsafe { strspn(
// REWRITES-NOT: unsafe { strcspn(

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn strncmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: usize) -> i32;
// COMMON-LOWERING-NEXT:     fn strnlen(_0: *const core::ffi::c_char, _1: usize) -> usize;
// COMMON-LOWERING-NEXT:     fn strspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// COMMON-LOWERING-NEXT:     fn strcspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     abc = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     abd = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     text = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     span = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     accept = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     empty = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     reject = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 99;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = spans({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = spans({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = spans({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strncmp(
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strnlen({{arg[0-9]+}} as *const core::ffi::c_char, {{arg[0-9]+}} as usize) }) as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         strspn(
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 100;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         strcspn(
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut abc: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let mut abd: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let mut text: [i8; 7] = [0; 7];
// LOWERING-X86_64-GNU-NEXT:     let mut span: [i8; 7] = [0; 7];
// LOWERING-X86_64-GNU-NEXT:     let mut accept: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let mut empty: [i8; 1] = [0; 1];
// LOWERING-X86_64-GNU-NEXT:     let mut reject: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 99, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 100, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 7] = [97, 98, 99, 100, 101, 102, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 7] = [97, 98, 97, 99, 97, 100, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 3] = [97, 98, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 1] = [0; 1];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 3] = [99, 100, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %zu %zu %zu %zu %zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abd.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abd.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abd.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = text.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = span.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = accept.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = span.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = reject.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: fn bounded_cmp({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-X86_64-GNU-NEXT: fn bounded_len({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> u64 {
// LOWERING-X86_64-GNU-NEXT: fn spans({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> u64 {
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut abc: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut abd: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut text: [u8; 7] = [0; 7];
// LOWERING-AARCH64-GNU-NEXT:     let mut span: [u8; 7] = [0; 7];
// LOWERING-AARCH64-GNU-NEXT:     let mut accept: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let mut empty: [u8; 1] = [0; 1];
// LOWERING-AARCH64-GNU-NEXT:     let mut reject: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 99, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 7] = [97, 98, 99, 100, 101, 102, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 7] = [97, 98, 97, 99, 97, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [97, 98, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 1] = [0; 1];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [99, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %zu %zu %zu %zu %zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abc.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abd.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abc.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abd.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abc.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abd.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abc.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = text.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = empty.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = span.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = accept.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = span.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = empty.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = empty.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = reject.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT: fn bounded_cmp({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn bounded_len({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: u64) -> u64 {
// LOWERING-AARCH64-GNU-NEXT: fn spans({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> u64 {
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn strncmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: usize) -> i32;
// COMMON-REWRITES-NEXT:     fn strnlen(_0: *const core::ffi::c_char, _1: usize) -> usize;
// COMMON-REWRITES-NEXT:     fn strspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// COMMON-REWRITES-NEXT:     fn strcspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp("abc", "abd", 2);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp("abc", "abd", 3);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} < 0) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp("abc", "abd", 0);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len("abc", 99);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len("abcdef", 3);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len("", 7);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = spans("abacad", "ab");
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = spans("abacad", "");
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             spans("", "cd"),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn bounded_cmp({{arg[0-9]+}}: &str, {{arg[0-9]+}}: &str, {{arg[0-9]+}}: u64) -> i32 {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         {{arg[0-9]+}}.as_bytes()[(0usize
// COMMON-REWRITES-NEXT:             ..{{arg[0-9]+}}
// COMMON-REWRITES-NEXT:                 .as_bytes()
// COMMON-REWRITES-NEXT:                 .iter()
// COMMON-REWRITES-NEXT:                 .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:                 .unwrap_or({{arg[0-9]+}}.as_bytes().len())
// COMMON-REWRITES-NEXT:                 .min({{arg[0-9]+}} as usize) as usize)]
// COMMON-REWRITES-NEXT:             .cmp(
// COMMON-REWRITES-NEXT:                 &{{arg[0-9]+}}.as_bytes()[(0usize
// COMMON-REWRITES-NEXT:                     ..{{arg[0-9]+}}
// COMMON-REWRITES-NEXT:                         .as_bytes()
// COMMON-REWRITES-NEXT:                         .iter()
// COMMON-REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len())
// COMMON-REWRITES-NEXT:                         .min({{arg[0-9]+}} as usize) as usize)],
// COMMON-REWRITES-NEXT:             ) as i32
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn bounded_len({{arg[0-9]+}}: &str, {{arg[0-9]+}}: u64) -> u64 {
// COMMON-REWRITES-NEXT:     (unsafe {
// COMMON-REWRITES-NEXT:         {{arg[0-9]+}}.as_bytes()
// COMMON-REWRITES-NEXT:             .iter()
// COMMON-REWRITES-NEXT:             .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:             .unwrap_or({{arg[0-9]+}}.as_bytes().len())
// COMMON-REWRITES-NEXT:             .min({{arg[0-9]+}} as usize)
// COMMON-REWRITES-NEXT:     }) as u64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn spans({{arg[0-9]+}}: &str, {{arg[0-9]+}}: &str) -> u64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         {{arg[0-9]+}}.as_bytes()[(0usize
// COMMON-REWRITES-NEXT:             ..{{arg[0-9]+}}
// COMMON-REWRITES-NEXT:                 .as_bytes()
// COMMON-REWRITES-NEXT:                 .iter()
// COMMON-REWRITES-NEXT:                 .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:                 .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// COMMON-REWRITES-NEXT:             .iter()
// COMMON-REWRITES-NEXT:             .position(|__slate_byte| {
// COMMON-REWRITES-NEXT:                 !{{arg[0-9]+}}.as_bytes()[(0usize
// COMMON-REWRITES-NEXT:                     ..{{arg[0-9]+}}
// COMMON-REWRITES-NEXT:                         .as_bytes()
// COMMON-REWRITES-NEXT:                         .iter()
// COMMON-REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// COMMON-REWRITES-NEXT:                     .contains(__slate_byte)
// COMMON-REWRITES-NEXT:             })
// COMMON-REWRITES-NEXT:             .unwrap_or(
// COMMON-REWRITES-NEXT:                 {{arg[0-9]+}}.as_bytes()[(0usize
// COMMON-REWRITES-NEXT:                     ..{{arg[0-9]+}}
// COMMON-REWRITES-NEXT:                         .as_bytes()
// COMMON-REWRITES-NEXT:                         .iter()
// COMMON-REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// COMMON-REWRITES-NEXT:                     .len(),
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * 100;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         {{arg[0-9]+}}.as_bytes()[(0usize
// COMMON-REWRITES-NEXT:             ..{{arg[0-9]+}}
// COMMON-REWRITES-NEXT:                 .as_bytes()
// COMMON-REWRITES-NEXT:                 .iter()
// COMMON-REWRITES-NEXT:                 .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:                 .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// COMMON-REWRITES-NEXT:             .iter()
// COMMON-REWRITES-NEXT:             .position(|__slate_byte| {
// COMMON-REWRITES-NEXT:                 {{arg[0-9]+}}.as_bytes()[(0usize
// COMMON-REWRITES-NEXT:                     ..{{arg[0-9]+}}
// COMMON-REWRITES-NEXT:                         .as_bytes()
// COMMON-REWRITES-NEXT:                         .iter()
// COMMON-REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// COMMON-REWRITES-NEXT:                     .contains(__slate_byte)
// COMMON-REWRITES-NEXT:             })
// COMMON-REWRITES-NEXT:             .unwrap_or(
// COMMON-REWRITES-NEXT:                 {{arg[0-9]+}}.as_bytes()[(0usize
// COMMON-REWRITES-NEXT:                     ..{{arg[0-9]+}}
// COMMON-REWRITES-NEXT:                         .as_bytes()
// COMMON-REWRITES-NEXT:                         .iter()
// COMMON-REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// COMMON-REWRITES-NEXT:                     .len(),
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut abc: [i8; 4] = [97, 98, 99, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut abd: [i8; 4] = [97, 98, 100, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut text: [i8; 7] = [97, 98, 99, 100, 101, 102, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut span: [i8; 7] = [97, 98, 97, 99, 97, 100, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut accept: [i8; 3] = [97, 98, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut empty: [i8; 1] = [0; 1];
// REWRITES-X86_64-GNU-NEXT:     let mut reject: [i8; 3] = [99, 100, 0];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %zu %zu %zu %zu %zu %zu\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     abc.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     abd.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     abc.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     abd.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     abc.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     abd.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     abc.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     text.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     empty.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     span.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     accept.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     span.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     empty.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     empty.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     reject.as_mut_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut abc: [u8; 4] = [97, 98, 99, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut abd: [u8; 4] = [97, 98, 100, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut text: [u8; 7] = [97, 98, 99, 100, 101, 102, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut span: [u8; 7] = [97, 98, 97, 99, 97, 100, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut accept: [u8; 3] = [97, 98, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut empty: [u8; 1] = [0; 1];
// REWRITES-AARCH64-GNU-NEXT:     let mut reject: [u8; 3] = [99, 100, 0];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %zu %zu %zu %zu %zu %zu\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     abc.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     abd.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     abc.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     abd.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     abc.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     abd.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     abc.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     text.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     empty.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     span.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     accept.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     span.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     empty.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     empty.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     reject.as_mut_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
