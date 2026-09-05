#include <stdio.h>

static __int128 add128(__int128 a, __int128 b) { return a + b; }

static unsigned __int128 mul128(unsigned __int128 a, unsigned __int128 b) {
  return a * b;
}

static void print128(unsigned __int128 v) {
  unsigned long long hi = (unsigned long long)(v >> 64);
  unsigned long long lo = (unsigned long long)v;
  printf("%llu:%llu\n", hi, lo);
}

int main(void) {
  __int128 a   = (__int128)9000000000000000000LL;
  __int128 b   = (__int128)9000000000000000000LL;
  __int128 sum = add128(a, b);
  print128((unsigned __int128)sum);

  unsigned __int128 x    = (unsigned __int128)1000000000000ULL;
  unsigned __int128 y    = (unsigned __int128)1000000000000ULL;
  unsigned __int128 prod = mul128(x, y);
  print128(prod);

  int cmp = (sum > 0) ? 1 : 0;
  printf("%d\n", cmp);

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
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = 9000000000000000000i128;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = 9000000000000000000i128;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = add128({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} as u128;
// COMMON-LOWERING-NEXT:     print128({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = 1000000000000u128;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = 1000000000000u128;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = mul128({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     print128({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn add128({{arg[0-9]+}}: i128, {{arg[0-9]+}}: i128) -> i128 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = {{arg[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn print128({{arg[0-9]+}}: u128) {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = {{arg[0-9]+}} >> {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn mul128({{arg[0-9]+}}: u128, {{arg[0-9]+}}: u128) -> u128 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = {{arg[0-9]+}} * {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%llu:%llu\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%llu:%llu\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i128 = 9000000000000000000i128;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i128 = 9000000000000000000i128;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i128 = add128({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:     print128({{__v[0-9]+}} as u128);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u128 = 1000000000000u128;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u128 = 1000000000000u128;
// COMMON-REWRITES-NEXT:     print128(mul128({{__v[0-9]+}}, {{__v[0-9]+}}));
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} > 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn add128({{arg[0-9]+}}: i128, {{arg[0-9]+}}: i128) -> i128 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn print128({{arg[0-9]+}}: u128) {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%llu:%llu\n".as_ptr(),
// COMMON-REWRITES-NEXT:             ({{arg[0-9]+}} >> (64 as i32)) as u64,
// COMMON-REWRITES-NEXT:             {{arg[0-9]+}} as u64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn mul128({{arg[0-9]+}}: u128, {{arg[0-9]+}}: u128) -> u128 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
