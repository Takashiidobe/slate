#include <stdio.h>

int main(void) {
  static const char text[]    = u8"\u03a9";
  int               alignment = (int)_Alignof(int);
  int               repeated  = (unsigned char)text[1] + (unsigned char)text[1];
  printf("%u %u %u %zu %d %d\n", (unsigned char)text[0], (unsigned char)text[1],
         (unsigned char)text[2], sizeof(text), alignment, repeated);
  return 0;
}

// REWRITES-DAG: static mut main_text: [i8; 3] = [-50, -87, 0];

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
// LOWERING-X86_64-GNU-NEXT: static mut main_text: [i8; 3] = [-50, -87, 0];
// LOWERING-AARCH64-GNU-NEXT: static mut main_text: [u8; 3] = [206, 169, 0];
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u %u %u %zu %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u %u %u %zu %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { main_text[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
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
// REWRITES-X86_64-GNU-NEXT: static mut main_text: [i8; 3] = [-50, -87, 0];
// REWRITES-AARCH64-GNU-NEXT: static mut main_text: [u8; 3] = [206, 169, 0];
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%u %u %u %zu %d %d\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             ((unsafe { main_text[0] }) as u8) as i32,
// REWRITES-X86_64-GNU-NEXT:             ((unsafe { main_text[1] }) as u8) as i32,
// REWRITES-X86_64-GNU-NEXT:             ((unsafe { main_text[2] }) as u8) as i32,
// REWRITES-AARCH64-GNU-NEXT:             (unsafe { main_text[0] }) as i32,
// REWRITES-AARCH64-GNU-NEXT:             (unsafe { main_text[1] }) as i32,
// REWRITES-AARCH64-GNU-NEXT:             (unsafe { main_text[2] }) as i32,
// REWRITES-NEXT:             3 as u64,
// REWRITES-NEXT:             4 as i32,
// REWRITES-X86_64-GNU-NEXT:             (((unsafe { main_text[1] }) as u8) as i32) + (((unsafe { main_text[1] }) as u8) as i32),
// REWRITES-AARCH64-GNU-NEXT:             ((unsafe { main_text[1] }) as i32) + ((unsafe { main_text[1] }) as i32),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
