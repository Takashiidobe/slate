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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [1, 2, 3, 4];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [5, 6, 7, 8];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [
// LOWERING-NEXT:         {{_v[0-9]+}}[0usize] + {{_v[0-9]+}}[0usize],
// LOWERING-NEXT:         {{_v[0-9]+}}[1usize] + {{_v[0-9]+}}[1usize],
// LOWERING-NEXT:         {{_v[0-9]+}}[2usize] + {{_v[0-9]+}}[2usize],
// LOWERING-NEXT:         {{_v[0-9]+}}[3usize] + {{_v[0-9]+}}[3usize],
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [{{_v[0-9]+}}[0usize], {{_v[0-9]+}}, {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[3usize]];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [{{_v[0-9]+}}[3usize], {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[1usize], {{_v[0-9]+}}[0usize]];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [1, 2, 3, 4];
// REWRITES-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [5, 6, 7, 8];
// REWRITES-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [
// REWRITES-NEXT:         {{_v[0-9]+}}[0usize] + {{_v[0-9]+}}[0usize],
// REWRITES-NEXT:         {{_v[0-9]+}}[1usize] + {{_v[0-9]+}}[1usize],
// REWRITES-NEXT:         {{_v[0-9]+}}[2usize] + {{_v[0-9]+}}[2usize],
// REWRITES-NEXT:         {{_v[0-9]+}}[3usize] + {{_v[0-9]+}}[3usize],
// REWRITES-NEXT:     ];
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 20;
// REWRITES-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [{{_v[0-9]+}}[0usize], {{_v[0-9]+}}, {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[3usize]];
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// REWRITES-NEXT:             {{_v[0-9]+}}[((0 as i32) as usize)],
// REWRITES-NEXT:             {{_v[0-9]+}}[((1 as i32) as usize)],
// REWRITES-NEXT:             [{{_v[0-9]+}}[3usize], {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[1usize], {{_v[0-9]+}}[0usize]][((0 as i32) as usize)],
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
