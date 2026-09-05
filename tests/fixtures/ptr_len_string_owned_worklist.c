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

// REWRITES-DAG: fn consume_text(mut arg{{[0-9]+}}: String) -> i32
// REWRITES-DAG: fn forward_text_owner(mut arg{{[0-9]+}}: String) -> i32
// REWRITES: String::from_raw_parts(
// REWRITES-NOT: unsafe { free(

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
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memcpy(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: *const core::ffi::c_void,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"abc\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         memcpy(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = forward_text_owner({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn forward_text_owner({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn forward_text_owner({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = consume_text({{arg[0-9]+}}, {{arg[0-9]+}});
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn consume_text({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut text: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT: fn consume_text({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-AARCH64-GNU-NEXT:     let mut text: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     let mut score: i32 = 0;
// LOWERING-NEXT:     text = {{arg[0-9]+}};
// LOWERING-NEXT:     len = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = text;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = text;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     score = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = len;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = text;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = text;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = score;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:             score = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = text;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = text;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = score;
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
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memcpy(
// REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-NEXT:         _1: *const core::ffi::c_void,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = ({{__v[0-9]+}} as u64) * 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::ptr::copy_nonoverlapping(
// REWRITES-NEXT:             c"abc".as_ptr() as *const u8,
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut u8,
// REWRITES-NEXT:             ({{__v[0-9]+}} as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d\n".as_ptr(),
// REWRITES-NEXT:             forward_text_owner(unsafe {
// REWRITES-NEXT:                 String::from_raw_parts({{__v[0-9]+}} as *mut u8, {{__v[0-9]+}} as usize, {{__v[0-9]+}} as usize)
// REWRITES-NEXT:             }),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn forward_text_owner(mut {{arg[0-9]+}}: String) -> i32 {
// REWRITES-NEXT:     consume_text({{arg[0-9]+}})
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn consume_text(mut {{arg[0-9]+}}: String) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut text: *mut i8 = {{arg[0-9]+}}.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let mut text: *mut u8 = {{arg[0-9]+}}.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     let mut score: i32 = 0;
// REWRITES-NEXT:     (unsafe { strlen(text as *const core::ffi::c_char) }) as u64;
// REWRITES-NEXT:     for i in 0..len {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = text;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-X86_64-GNU-NEXT:         score += ((unsafe { *{{__v[0-9]+}} }) as u8) as i32;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = text;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-AARCH64-GNU-NEXT:         score += (unsafe { *{{__v[0-9]+}} }) as i32;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     score
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
