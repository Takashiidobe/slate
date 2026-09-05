#include <stdio.h>

typedef int v4si __attribute__((vector_size(16)));

int main(void) {
  v4si a = {1, 2, 3, 4};
  v4si b = {5, 6, 7, 8};
  v4si c = a + b;
  c[1]   = 20;
  v4si d = __builtin_shufflevector(c, c, 3, 2, 1, 0);
  printf("%d %d %d\n", c[0], c[1], d[0]);
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
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [1, 2, 3, 4];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [5, 6, 7, 8];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 20;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [{{__v[0-9]+}}[0usize], {{__v[0-9]+}}, {{__v[0-9]+}}[2usize], {{__v[0-9]+}}[3usize]];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [{{__v[0-9]+}}[3usize], {{__v[0-9]+}}[2usize], {{__v[0-9]+}}[1usize], {{__v[0-9]+}}[0usize]];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [1, 2, 3, 4];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [5, 6, 7, 8];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 20;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [{{__v[0-9]+}}[0usize], {{__v[0-9]+}}, {{__v[0-9]+}}[2usize], {{__v[0-9]+}}[3usize]];
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}[0],
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}[1],
// COMMON-REWRITES-NEXT:             [{{__v[0-9]+}}[3usize], {{__v[0-9]+}}[2usize], {{__v[0-9]+}}[1usize], {{__v[0-9]+}}[0usize]][0],
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
