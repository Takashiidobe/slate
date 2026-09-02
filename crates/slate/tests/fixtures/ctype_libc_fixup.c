#include <ctype.h>
#include <stdio.h>

static int next_lower(void) {
  static int c = 'a';
  return c++;
}

int main(void) {
  int upper = 'Q';
  int lower = 'q';
  int digit = '5';
  int punct = '!';
  int eof   = EOF;

  printf("%d %d %d %d %d\n", toupper(lower), toupper(digit), toupper(punct),
         toupper(upper), toupper(eof));
  printf("%d %d %d %d %d\n", tolower(upper), tolower(digit), tolower(punct),
         tolower(lower), tolower(eof));
  printf("%d %d\n", toupper(next_lower()), tolower(next_lower()));
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
// LOWERING-NEXT: static mut next_lower_c: i32 = 97;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn toupper(_0: i32) -> i32;
// LOWERING-NEXT:     fn tolower(_0: i32) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn next_lower() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { next_lower_c };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         next_lower_c = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 81;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 113;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 53;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 33;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { toupper({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { toupper({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { toupper({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { toupper({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { toupper({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { tolower({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { tolower({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { tolower({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { tolower({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { tolower({{_v[0-9]+}} as i32) };
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = next_lower();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { toupper({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = next_lower();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { tolower({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: static mut next_lower_c: i32 = 97;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn toupper(_0: i32) -> i32;
// REWRITES-NEXT:     fn tolower(_0: i32) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn next_lower() -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { next_lower_c };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         next_lower_c = {{_v[0-9]+}} + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 81;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 113;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 53;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 33;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             unsafe { toupper({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:             unsafe { toupper({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:             unsafe { toupper({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:             unsafe { toupper({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:             unsafe { toupper({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             unsafe { tolower({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:             unsafe { tolower({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:             unsafe { tolower({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:             unsafe { tolower({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:             unsafe { tolower({{_v[0-9]+}} as i32) },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"%d %d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = next_lower();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { toupper({{_v[0-9]+}} as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = next_lower();
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, unsafe {
// REWRITES-NEXT:             tolower({{_v[0-9]+}} as i32)
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
