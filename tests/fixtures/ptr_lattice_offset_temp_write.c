#include <stdio.h>

static void fill(int *p, int n, int val) {
  for (int i = 0; i < n; i++) {
    *(p + i) = val;
  }
}

int main(void) {
  int buf[4] = {0};
  fill(buf, 4, 7);
  printf("%d %d %d %d\n", buf[0], buf[1], buf[2], buf[3]);
  return buf[0] + buf[1] + buf[2] + buf[3];
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
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [0; 4];
// LOWERING-NEXT:     *buf = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     fill({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = buf[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = buf[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = buf[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = buf[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = buf[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = buf[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = buf[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = buf[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut val: i32 = 0;
// LOWERING-NEXT:     p = {{arg[0-9]+}};
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-NEXT:     val = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = n;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = val;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
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
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT:     *buf = [0; 4];
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     fill(
// REWRITES-NEXT:         unsafe { std::slice::from_raw_parts_mut({{_v[0-9]+}} as *mut i32, (4 as i32) as usize) },
// REWRITES-NEXT:         7,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     unsafe { printf(c"%d %d %d %d\n".as_ptr(), buf[0], buf[1], buf[2], buf[3]) };
// REWRITES-NEXT:     std::process::exit((buf[0] + buf[1] + buf[2] + buf[3]) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill({{arg[0-9]+}}: &mut [i32], mut val: i32) {
// REWRITES-NEXT:     let mut p: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let mut n: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     for i in 0..n {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = val;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = p;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset(i as isize) };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
