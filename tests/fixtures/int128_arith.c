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
// LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = 9000000000000000000i128;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = 9000000000000000000i128;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = add128({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} as u128;
// LOWERING-NEXT:     print128({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = 1000000000000u128;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = 1000000000000u128;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = mul128({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     print128({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add128({{arg[0-9]+}}: i128, {{arg[0-9]+}}: i128) -> i128 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn print128({{arg[0-9]+}}: u128) {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = {{arg[0-9]+}} >> {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%llu:%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mul128({{arg[0-9]+}}: u128, {{arg[0-9]+}}: u128) -> u128 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
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
// REWRITES-NEXT:     let {{__v[0-9]+}}: i128 = 9000000000000000000i128;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i128 = 9000000000000000000i128;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i128 = add128({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-NEXT:     print128({{__v[0-9]+}} as u128);
// REWRITES-NEXT:     let {{__v[0-9]+}}: u128 = 1000000000000u128;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u128 = 1000000000000u128;
// REWRITES-NEXT:     print128(mul128({{__v[0-9]+}}, {{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} > 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add128({{arg[0-9]+}}: i128, {{arg[0-9]+}}: i128) -> i128 {
// REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn print128({{arg[0-9]+}}: u128) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%llu:%llu\n".as_ptr(),
// REWRITES-NEXT:             ({{arg[0-9]+}} >> (64 as i32)) as u64,
// REWRITES-NEXT:             {{arg[0-9]+}} as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mul128({{arg[0-9]+}}: u128, {{arg[0-9]+}}: u128) -> u128 {
// REWRITES-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
