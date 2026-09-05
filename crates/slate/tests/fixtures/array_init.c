#include <stdio.h>

int main(void) {
  int  a[5]       = {1, 2, 3, 4, 5};
  int  partial[4] = {7, 8};
  char s[6]       = "hello";
  char padded[8]  = "hi";

  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += a[i];
  }
  printf("%d\n", sum);
  printf("%d %d\n", partial[1], partial[3]);
  printf("%s\n", s);
  printf("%s %d\n", padded, padded[4]);
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
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-NEXT:     let mut partial: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut s: [i8; 6] = [0; 6];
// LOWERING-NEXT:     let mut padded: [i8; 8] = [0; 8];
// LOWERING-NEXT:     let mut sum: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 5] = [1, 2, 3, 4, 5];
// LOWERING-NEXT:     *a = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [7, 8, 0, 0];
// LOWERING-NEXT:     *partial = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i8; 6] = [104, 101, 108, 108, 111, 0];
// LOWERING-NEXT:     s = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i8; 8] = [104, 105, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     padded = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     sum = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                 sum = {{__v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = partial[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = partial[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = s.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = padded.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = padded[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-NEXT:     let mut partial: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT:     let mut s: [i8; 6] = [0; 6];
// REWRITES-NEXT:     let mut padded: [i8; 8] = [0; 8];
// REWRITES-NEXT:     let mut sum: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i32; 5] = [1, 2, 3, 4, 5];
// REWRITES-NEXT:     *a = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [7, 8, 0, 0];
// REWRITES-NEXT:     *partial = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i8; 6] = [104, 101, 108, 108, 111, 0];
// REWRITES-NEXT:     s = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i8; 8] = [104, 105, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT:     padded = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     sum = {{__v[0-9]+}};
// REWRITES-NEXT:     let mut i: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     i = {{__v[0-9]+}};
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 5;
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// REWRITES-NEXT:         if !{{__v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = sum;
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-NEXT:         sum = {{__v[0-9]+}};
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = i;
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// REWRITES-NEXT:         i = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = sum;
// REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = partial[({{__v[0-9]+}} as usize)];
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = partial[({{__v[0-9]+}} as usize)];
// REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%s\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = s.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%s %d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = padded.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i8 = padded[({{__v[0-9]+}} as usize)];
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
