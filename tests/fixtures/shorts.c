#include <stdio.h>

static short sadd(short a, short b) {
  short c = a + b;
  return c;
}

int main(void) {
  short          s   = 30000;
  unsigned short us  = 60000;
  short          neg = -12345;
  printf("%d\n", sadd(s, -30000));
  printf("%d\n", us);
  printf("%d\n", neg);
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
// LOWERING-NEXT: fn sadd({{arg[0-9]+}}: i16, {{arg[0-9]+}}: i16) -> i16 {
// LOWERING-NEXT:     let mut a: i16 = 0;
// LOWERING-NEXT:     let mut b: i16 = 0;
// LOWERING-NEXT:     let mut __retval: i16 = 0;
// LOWERING-NEXT:     let mut c: i16 = 0;
// LOWERING-NEXT:     a = {{arg[0-9]+}};
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = {{_v[0-9]+}} as i16;
// LOWERING-NEXT:     c = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = c;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut s: i16 = 0;
// LOWERING-NEXT:     let mut us: u16 = 0;
// LOWERING-NEXT:     let mut neg: i16 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = 30000;
// LOWERING-NEXT:     s = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 60000;
// LOWERING-NEXT:     us = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = -12345;
// LOWERING-NEXT:     neg = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = s;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = -30000;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = sadd({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = us;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = neg;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
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
// REWRITES-NEXT: fn sadd({{arg[0-9]+}}: i16, {{arg[0-9]+}}: i16) -> i16 {
// REWRITES-NEXT: let mut a: i16 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut b: i16 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i16 = 0;
// REWRITES-NEXT: let mut c: i16 = 0;
// REWRITES-NEXT: c = ((a as i32) + (b as i32)) as i16;
// REWRITES-NEXT: __retval = c;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut s: i16 = 0;
// REWRITES-NEXT: let mut us: u16 = 0;
// REWRITES-NEXT: let mut neg: i16 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: s = 30000;
// REWRITES-NEXT: us = 60000;
// REWRITES-NEXT: neg = -12345;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i16 = -30000;
// REWRITES-NEXT: let {{_v[0-9]+}}: i16 = sadd(s, {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, us as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, neg as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
