#include <stdio.h>

int main(void) {
  int a    = 5;
  int post = a--;
  int pre  = --a;
  int sum  = a-- + --pre;
  printf("%d %d %d %d\n", a, post, pre, sum);

  unsigned char c = 0;
  c--;
  printf("%u\n", (unsigned)c);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut post: i32 = 0;
// LOWERING-NEXT:     let mut pre: i32 = 0;
// LOWERING-NEXT:     let mut sum: i32 = 0;
// LOWERING-NEXT:     let mut c: u8 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - 1;
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     post = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - 1;
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     pre = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - 1;
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = pre;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - 1;
// LOWERING-NEXT:     pre = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     sum = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = post;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = pre;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 0;
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = c;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} - 1;
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = c;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: i32 = 0;
// REWRITES-NEXT: let mut post: i32 = 0;
// REWRITES-NEXT: let mut pre: i32 = 0;
// REWRITES-NEXT: let mut sum: i32 = 0;
// REWRITES-NEXT: let mut c: u8 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = a;
// REWRITES-NEXT: a = {{_v[0-9]+}} - 1;
// REWRITES-NEXT: post = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = a - 1;
// REWRITES-NEXT: a = {{_v[0-9]+}};
// REWRITES-NEXT: pre = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = a;
// REWRITES-NEXT: a = {{_v[0-9]+}} - 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = pre - 1;
// REWRITES-NEXT: pre = {{_v[0-9]+}};
// REWRITES-NEXT: sum = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, a, post, pre, sum) };
// REWRITES-NEXT: c = 0;
// REWRITES-NEXT: c = c - 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, c as u32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
