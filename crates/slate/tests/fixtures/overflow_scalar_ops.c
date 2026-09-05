#include <stdio.h>

int main(void) {
  int          ri  = 0;
  unsigned int ru  = 0;
  long long    rll = 0;

  int add_i = __builtin_add_overflow(2147483647, 1, &ri);
  printf("%d %d\n", add_i, ri);

  int sub_i = __builtin_sub_overflow((-2147483647 - 1), 1, &ri);
  printf("%d %d\n", sub_i, ri);

  int mul_i = __builtin_mul_overflow(1073741824, 2, &ri);
  printf("%d %d\n", mul_i, ri);

  int add_u = __builtin_add_overflow(4294967295u, 1u, &ru);
  printf("%d %u\n", add_u, ru);

  int mul_ll = __builtin_mul_overflow(3037000500LL, 3037000500LL, &rll);
  printf("%d %lld\n", mul_ll, rll);

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
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2147483647;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -2147483647;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1073741824;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %u\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %u\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3037000500i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3037000500i64;
// LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}.0 as i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -9223372036854775808 || {{__v[0-9]+}}.0 > 9223372036854775807);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %lld\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %lld\n\0".as_ptr() as *mut u8;
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
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2147483647;
// REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add(1 as i32);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// REWRITES-NEXT:             ({{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647)) as i32,
// REWRITES-NEXT:             {{__v[0-9]+}}.0 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = -2147483647 - {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub(1 as i32);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// REWRITES-NEXT:             ({{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647)) as i32,
// REWRITES-NEXT:             {{__v[0-9]+}}.0 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1073741824;
// REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul(2 as i32);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// REWRITES-NEXT:             ({{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647)) as i32,
// REWRITES-NEXT:             {{__v[0-9]+}}.0 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add(1 as u32);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %u\n".as_ptr(),
// REWRITES-NEXT:             ({{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295) as i32,
// REWRITES-NEXT:             {{__v[0-9]+}}.0 as u32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 3037000500i64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 3037000500i64;
// REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul({{__v[0-9]+}});
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %lld\n".as_ptr(),
// REWRITES-NEXT:             ({{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -9223372036854775808 || {{__v[0-9]+}}.0 > 9223372036854775807)) as i32,
// REWRITES-NEXT:             {{__v[0-9]+}}.0 as i64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
