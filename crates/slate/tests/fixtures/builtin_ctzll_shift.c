#include <stddef.h>
#include <stdio.h>

size_t lowest_bit(unsigned long long x) {
  return (size_t)1 << (size_t)__builtin_ctzll(x);
}

int main(void) {
  unsigned long long x = 0;
  scanf("%llu", &x);
  size_t n = lowest_bit(x);
  printf("%zu\n", n);
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
// LOWERING-NEXT:     fn scanf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn lowest_bit({{arg[0-9]+}}: u64) -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}}.trailing_zeros() as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut x: u64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     x = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%llu\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { scanf({{__v[0-9]+}} as *const core::ffi::c_char, std::ptr::addr_of_mut!(x)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = x;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = lowest_bit({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
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
// REWRITES-NEXT:     fn scanf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn lowest_bit({{arg[0-9]+}}: u64) -> u64 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}}.trailing_zeros() as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut x: u64 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// REWRITES-NEXT:     x = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%llu".as_ptr() as *mut i8;
// REWRITES-NEXT:     unsafe { scanf({{__v[0-9]+}} as *const core::ffi::c_char, std::ptr::addr_of_mut!(x)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = x;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = lowest_bit({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%zu\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
