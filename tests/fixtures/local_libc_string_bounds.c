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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn strncmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: usize) -> i32;
// LOWERING-NEXT:     fn strnlen(_0: *const core::ffi::c_char, _1: usize) -> usize;
// LOWERING-NEXT:     fn strspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT:     fn strcspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut abc: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let mut abd: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let mut text: [i8; 7] = [0; 7];
// LOWERING-X86_64-GNU-NEXT:     let mut span: [i8; 7] = [0; 7];
// LOWERING-X86_64-GNU-NEXT:     let mut accept: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let mut empty: [i8; 1] = [0; 1];
// LOWERING-X86_64-GNU-NEXT:     let mut reject: [i8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let mut abc: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut abd: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut text: [u8; 7] = [0; 7];
// LOWERING-AARCH64-GNU-NEXT:     let mut span: [u8; 7] = [0; 7];
// LOWERING-AARCH64-GNU-NEXT:     let mut accept: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let mut empty: [u8; 1] = [0; 1];
// LOWERING-AARCH64-GNU-NEXT:     let mut reject: [u8; 3] = [0; 3];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 99, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 99, 0];
// LOWERING-NEXT:     abc = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 100, 0];
// LOWERING-NEXT:     abd = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 7] = [97, 98, 99, 100, 101, 102, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 7] = [97, 98, 99, 100, 101, 102, 0];
// LOWERING-NEXT:     text = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 7] = [97, 98, 97, 99, 97, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 7] = [97, 98, 97, 99, 97, 100, 0];
// LOWERING-NEXT:     span = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 3] = [97, 98, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [97, 98, 0];
// LOWERING-NEXT:     accept = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 1] = [0; 1];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 1] = [0; 1];
// LOWERING-NEXT:     empty = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 3] = [99, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [99, 100, 0];
// LOWERING-NEXT:     reject = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %zu %zu %zu %zu %zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abd.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %zu %zu %zu %zu %zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abc.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abd.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abd.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abc.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abd.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abd.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abc.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abd.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = abc.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = abc.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 99;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = text.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = text.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = empty.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = span.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = accept.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = span.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = accept.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = spans({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = span.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = span.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = empty.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = spans({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = reject.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = empty.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = reject.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = spans({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn bounded_cmp({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn bounded_cmp({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strncmp(
// LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn bounded_len({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> u64 {
// LOWERING-AARCH64-GNU-NEXT: fn bounded_len({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: u64) -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strnlen({{arg[0-9]+}} as *const core::ffi::c_char, {{arg[0-9]+}} as usize) }) as u64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn spans({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> u64 {
// LOWERING-AARCH64-GNU-NEXT: fn spans({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         strspn(
// LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 100;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         strcspn(
// LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn strncmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: usize) -> i32;
// REWRITES-NEXT:     fn strnlen(_0: *const core::ffi::c_char, _1: usize) -> usize;
// REWRITES-NEXT:     fn strspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT:     fn strcspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
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
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp("abc", "abd", 2);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// REWRITES-X86_64-GNU-NEXT:     abc.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     abd.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     abc.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     abd.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp("abc", "abd", 3);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} < 0) as i32;
// REWRITES-X86_64-GNU-NEXT:     abc.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     abd.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     abc.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     abd.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bounded_cmp("abc", "abd", 0);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// REWRITES-X86_64-GNU-NEXT:     abc.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     abc.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len("abc", 99);
// REWRITES-X86_64-GNU-NEXT:     text.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     text.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len("abcdef", 3);
// REWRITES-X86_64-GNU-NEXT:     empty.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     empty.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = bounded_len("", 7);
// REWRITES-X86_64-GNU-NEXT:     span.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     accept.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     span.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     accept.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = spans("abacad", "ab");
// REWRITES-X86_64-GNU-NEXT:     span.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     empty.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     span.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     empty.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = spans("abacad", "");
// REWRITES-X86_64-GNU-NEXT:     empty.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     reject.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     empty.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     reject.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             spans("", "cd"),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn bounded_cmp({{arg[0-9]+}}: &str, {{arg[0-9]+}}: &str, {{arg[0-9]+}}: u64) -> i32 {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         {{arg[0-9]+}}.as_bytes()[(0usize
// REWRITES-NEXT:             ..{{arg[0-9]+}}
// REWRITES-NEXT:                 .as_bytes()
// REWRITES-NEXT:                 .iter()
// REWRITES-NEXT:                 .position(|__slate_byte| *__slate_byte == 0u8)
// REWRITES-NEXT:                 .unwrap_or({{arg[0-9]+}}.as_bytes().len())
// REWRITES-NEXT:                 .min({{arg[0-9]+}} as usize) as usize)]
// REWRITES-NEXT:             .cmp(
// REWRITES-NEXT:                 &{{arg[0-9]+}}.as_bytes()[(0usize
// REWRITES-NEXT:                     ..{{arg[0-9]+}}
// REWRITES-NEXT:                         .as_bytes()
// REWRITES-NEXT:                         .iter()
// REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len())
// REWRITES-NEXT:                         .min({{arg[0-9]+}} as usize) as usize)],
// REWRITES-NEXT:             ) as i32
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn bounded_len({{arg[0-9]+}}: &str, {{arg[0-9]+}}: u64) -> u64 {
// REWRITES-NEXT:     (unsafe {
// REWRITES-NEXT:         {{arg[0-9]+}}.as_bytes()
// REWRITES-NEXT:             .iter()
// REWRITES-NEXT:             .position(|__slate_byte| *__slate_byte == 0u8)
// REWRITES-NEXT:             .unwrap_or({{arg[0-9]+}}.as_bytes().len())
// REWRITES-NEXT:             .min({{arg[0-9]+}} as usize)
// REWRITES-NEXT:     }) as u64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn spans({{arg[0-9]+}}: &str, {{arg[0-9]+}}: &str) -> u64 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         {{arg[0-9]+}}.as_bytes()[(0usize
// REWRITES-NEXT:             ..{{arg[0-9]+}}
// REWRITES-NEXT:                 .as_bytes()
// REWRITES-NEXT:                 .iter()
// REWRITES-NEXT:                 .position(|__slate_byte| *__slate_byte == 0u8)
// REWRITES-NEXT:                 .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// REWRITES-NEXT:             .iter()
// REWRITES-NEXT:             .position(|__slate_byte| {
// REWRITES-NEXT:                 !{{arg[0-9]+}}.as_bytes()[(0usize
// REWRITES-NEXT:                     ..{{arg[0-9]+}}
// REWRITES-NEXT:                         .as_bytes()
// REWRITES-NEXT:                         .iter()
// REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// REWRITES-NEXT:                     .contains(__slate_byte)
// REWRITES-NEXT:             })
// REWRITES-NEXT:             .unwrap_or(
// REWRITES-NEXT:                 {{arg[0-9]+}}.as_bytes()[(0usize
// REWRITES-NEXT:                     ..{{arg[0-9]+}}
// REWRITES-NEXT:                         .as_bytes()
// REWRITES-NEXT:                         .iter()
// REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// REWRITES-NEXT:                     .len(),
// REWRITES-NEXT:             )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * 100;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         {{arg[0-9]+}}.as_bytes()[(0usize
// REWRITES-NEXT:             ..{{arg[0-9]+}}
// REWRITES-NEXT:                 .as_bytes()
// REWRITES-NEXT:                 .iter()
// REWRITES-NEXT:                 .position(|__slate_byte| *__slate_byte == 0u8)
// REWRITES-NEXT:                 .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// REWRITES-NEXT:             .iter()
// REWRITES-NEXT:             .position(|__slate_byte| {
// REWRITES-NEXT:                 {{arg[0-9]+}}.as_bytes()[(0usize
// REWRITES-NEXT:                     ..{{arg[0-9]+}}
// REWRITES-NEXT:                         .as_bytes()
// REWRITES-NEXT:                         .iter()
// REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// REWRITES-NEXT:                     .contains(__slate_byte)
// REWRITES-NEXT:             })
// REWRITES-NEXT:             .unwrap_or(
// REWRITES-NEXT:                 {{arg[0-9]+}}.as_bytes()[(0usize
// REWRITES-NEXT:                     ..{{arg[0-9]+}}
// REWRITES-NEXT:                         .as_bytes()
// REWRITES-NEXT:                         .iter()
// REWRITES-NEXT:                         .position(|__slate_byte| *__slate_byte == 0u8)
// REWRITES-NEXT:                         .unwrap_or({{arg[0-9]+}}.as_bytes().len()) as usize)]
// REWRITES-NEXT:                     .len(),
// REWRITES-NEXT:             )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     {{__v[0-9]+}} + {{__v[0-9]+}}
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
