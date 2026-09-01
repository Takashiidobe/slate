#include <stdio.h>

struct Pair {
  int x;
  int y;
};

struct Bits {
  unsigned a;
  unsigned b;
};

int main(void) {
  struct Pair p = {7, 9};
  struct Bits b = __builtin_bit_cast(struct Bits, p);
  printf("%u %u\n", b.a, b.b);
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Bits {
// LOWERING-NEXT:     a: u32,
// LOWERING-NEXT:     b: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Pair {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut p: Pair = Pair { x: 0, y: 0 };
// LOWERING-NEXT:     let mut b: Bits = Bits { a: 0, b: 0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     p = Pair { x: 7, y: 9 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(p) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(b) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     unsafe { std::ptr::copy_nonoverlapping({{_v[0-9]+}} as *const u8, {{_v[0-9]+}} as *mut u8, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = b.a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = b.b;
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Bits {
// REWRITES-NEXT:     a: u32,
// REWRITES-NEXT:     b: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Pair {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut p: Pair = Pair { x: 7, y: 9 };
// REWRITES-NEXT:     let mut b: Bits = Bits { a: 0, b: 0 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(p) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(b) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// REWRITES-NEXT:     unsafe { std::ptr::copy_nonoverlapping({{_v[0-9]+}} as *const u8, {{_v[0-9]+}} as *mut u8, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = b.a;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = b.b;
// REWRITES-NEXT:     unsafe { printf(c"%u %u\n".as_ptr(), {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
