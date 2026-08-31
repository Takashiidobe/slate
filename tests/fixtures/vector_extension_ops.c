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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [1, 2, 3, 4];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [5, 6, 7, 8];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [{{_v[0-9]+}}[0usize] + {{_v[0-9]+}}[0usize], {{_v[0-9]+}}[1usize] + {{_v[0-9]+}}[1usize], {{_v[0-9]+}}[2usize] + {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[3usize] + {{_v[0-9]+}}[3usize]];
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
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
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = [1, 2, 3, 4];
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = [5, 6, 7, 8];
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = [{{_v[0-9]+}}[0usize] + {{_v[0-9]+}}[0usize], {{_v[0-9]+}}[1usize] + {{_v[0-9]+}}[1usize], {{_v[0-9]+}}[2usize] + {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[3usize] + {{_v[0-9]+}}[3usize]];
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 20;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = [{{_v[0-9]+}}[0usize], {{_v[0-9]+}}, {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[3usize]];
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = [{{_v[0-9]+}}[3usize], {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[1usize], {{_v[0-9]+}}[0usize]];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}[({{_v[0-9]+}} as usize)], {{_v[0-9]+}}[({{_v[0-9]+}} as usize)], {{_v[0-9]+}}[({{_v[0-9]+}} as usize)]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
