#include <stdio.h>
#include <string.h>

int main(void) {
  char copy[16]         = {0};
  char append[16]       = "foo";
  char trunc_copy[16]   = {0};
  char trunc_append[16] = "pre";

  strcpy(copy, "abc");
  strcat(append, "bar");
  strncpy(trunc_copy, "abcdef", 3);
  strncat(trunc_append, "suffix", 3);

  printf("%s %s %s %s %zu\n", copy, append, trunc_copy, trunc_append,
         strlen(trunc_append));
  return 0;
}

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
// LOWERING-NEXT:     fn strcpy(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strcat(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strncpy(
// LOWERING-NEXT:         _0: *mut core::ffi::c_char,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strncat(
// LOWERING-NEXT:         _0: *mut core::ffi::c_char,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut copy: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut append: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut trunc_copy: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut trunc_append: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i8; 16] = [0; 16];
// LOWERING-NEXT:     *copy = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i8; 16] = [102, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *append = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i8; 16] = [0; 16];
// LOWERING-NEXT:     *trunc_copy = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i8; 16] = [112, 114, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *trunc_append = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = copy.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-NEXT:         strcpy(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"bar\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-NEXT:         strcat(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = trunc_copy.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"abcdef\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-NEXT:         strncpy(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"suffix\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-NEXT:         strncat(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%s %s %s %s %zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = copy.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = trunc_copy.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT:     fn strcpy(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strcat(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strncpy(
// REWRITES-NEXT:         _0: *mut core::ffi::c_char,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strncat(
// REWRITES-NEXT:         _0: *mut core::ffi::c_char,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut copy: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT:     let mut append: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT:     let mut trunc_copy: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT:     let mut trunc_append: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT:     *copy = [0; 16];
// REWRITES-NEXT:     *append = [102, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT:     *trunc_copy = [0; 16];
// REWRITES-NEXT:     *trunc_append = [112, 114, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT:     (unsafe { strcpy(copy.as_mut_ptr() as *mut core::ffi::c_char, c"abc".as_ptr()) }) as *mut i8;
// REWRITES-NEXT:     (unsafe {
// REWRITES-NEXT:         strcat(
// REWRITES-NEXT:             append.as_mut_ptr() as *mut core::ffi::c_char,
// REWRITES-NEXT:             c"bar".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as *mut i8;
// REWRITES-NEXT:     (unsafe {
// REWRITES-NEXT:         strncpy(
// REWRITES-NEXT:             trunc_copy.as_mut_ptr() as *mut core::ffi::c_char,
// REWRITES-NEXT:             c"abcdef".as_ptr(),
// REWRITES-NEXT:             (3 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as *mut i8;
// REWRITES-NEXT:     (unsafe {
// REWRITES-NEXT:         strncat(
// REWRITES-NEXT:             trunc_append.as_mut_ptr() as *mut core::ffi::c_char,
// REWRITES-NEXT:             c"suffix".as_ptr(),
// REWRITES-NEXT:             (3 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"%s %s %s %s %zu\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = copy.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = append.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = trunc_copy.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = trunc_append.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             (unsafe { strlen({{_v[0-9]+}} as *const core::ffi::c_char) }) as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
