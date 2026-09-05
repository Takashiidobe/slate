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
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut sum: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 5] = [1, 2, 3, 4, 5];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [7, 8, 0, 0];
// COMMON-LOWERING-NEXT:     s = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     padded = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     sum = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = a[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 sum = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = partial[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = partial[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-X86_64-GNU-NEXT:     let mut partial: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     let mut s: [i8; 6] = [0; 6];
// LOWERING-X86_64-GNU-NEXT:     let mut padded: [i8; 8] = [0; 8];
// LOWERING-X86_64-GNU-NEXT:     *a = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     *partial = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 6] = [104, 101, 108, 108, 111, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 8] = [104, 105, 0, 0, 0, 0, 0, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = s.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = padded.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = padded[({{__v[0-9]+}} as usize)];
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut a: [i32; 5] = [0; 5];
// LOWERING-AARCH64-GNU-NEXT:     let mut partial: [i32; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut s: [u8; 6] = [0; 6];
// LOWERING-AARCH64-GNU-NEXT:     let mut padded: [u8; 8] = [0; 8];
// LOWERING-AARCH64-GNU-NEXT:     a = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     partial = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 6] = [104, 101, 108, 108, 111, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 8] = [104, 105, 0, 0, 0, 0, 0, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%s\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = s.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%s %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = padded.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = padded[({{__v[0-9]+}} as usize)];
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
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut sum: i32 = 0;
// COMMON-REWRITES-NEXT:     for i in a.iter().copied() {
// COMMON-REWRITES-NEXT:         sum += i;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), sum) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), partial[1], partial[3]) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%s %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             padded[4] as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-X86_64-GNU-NEXT:     let mut partial: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     let mut s: [i8; 6] = [0; 6];
// REWRITES-X86_64-GNU-NEXT:     let mut padded: [i8; 8] = [0; 8];
// REWRITES-X86_64-GNU-NEXT:     *a = [1, 2, 3, 4, 5];
// REWRITES-X86_64-GNU-NEXT:     *partial = [7, 8, 0, 0];
// REWRITES-X86_64-GNU-NEXT:     s = [104, 101, 108, 108, 111, 0];
// REWRITES-X86_64-GNU-NEXT:     padded = [104, 105, 0, 0, 0, 0, 0, 0];
// REWRITES-X86_64-GNU-NEXT:     unsafe { printf(c"%s\n".as_ptr(), s.as_mut_ptr() as *mut i8) };
// REWRITES-X86_64-GNU-NEXT:             padded.as_mut_ptr() as *mut i8,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut a: [i32; 5] = [1, 2, 3, 4, 5];
// REWRITES-AARCH64-GNU-NEXT:     let mut partial: [i32; 4] = [7, 8, 0, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut s: [u8; 6] = [104, 101, 108, 108, 111, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut padded: [u8; 8] = [104, 105, 0, 0, 0, 0, 0, 0];
// REWRITES-AARCH64-GNU-NEXT:     unsafe { printf(c"%s\n".as_ptr(), s.as_mut_ptr() as *mut u8) };
// REWRITES-AARCH64-GNU-NEXT:             padded.as_mut_ptr() as *mut u8,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
