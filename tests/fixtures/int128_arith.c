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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add128({{arg[0-9]+}}: i128, {{arg[0-9]+}}: i128) -> i128 {
// LOWERING-NEXT:     let mut a: i128 = 0;
// LOWERING-NEXT:     let mut b: i128 = 0;
// LOWERING-NEXT:     let mut __retval: i128 = 0;
// LOWERING-NEXT:     a = {{arg[0-9]+}};
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn print128({{arg[0-9]+}}: u128) {
// LOWERING-NEXT:     let mut v: u128 = 0;
// LOWERING-NEXT:     let mut hi: u64 = 0;
// LOWERING-NEXT:     let mut lo: u64 = 0;
// LOWERING-NEXT:     v = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = v;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}} >> {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     hi = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = v;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     lo = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%llu:%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = hi;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = lo;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mul128({{arg[0-9]+}}: u128, {{arg[0-9]+}}: u128) -> u128 {
// LOWERING-NEXT:     let mut a: u128 = 0;
// LOWERING-NEXT:     let mut b: u128 = 0;
// LOWERING-NEXT:     let mut __retval: u128 = 0;
// LOWERING-NEXT:     a = {{arg[0-9]+}};
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: i128 = 0;
// LOWERING-NEXT:     let mut b: i128 = 0;
// LOWERING-NEXT:     let mut sum: i128 = 0;
// LOWERING-NEXT:     let mut x: u128 = 0;
// LOWERING-NEXT:     let mut y: u128 = 0;
// LOWERING-NEXT:     let mut prod: u128 = 0;
// LOWERING-NEXT:     let mut cmp: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = 9000000000000000000i128;
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = 9000000000000000000i128;
// LOWERING-NEXT:     b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = add128({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     sum = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = sum;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}} as u128;
// LOWERING-NEXT:     print128({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = 1000000000000u128;
// LOWERING-NEXT:     x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = 1000000000000u128;
// LOWERING-NEXT:     y = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = mul128({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     prod = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = prod;
// LOWERING-NEXT:     print128({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = sum;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i128 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     cmp = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = cmp;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
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
// REWRITES-NEXT: fn add128({{arg[0-9]+}}: i128, {{arg[0-9]+}}: i128) -> i128 {
// REWRITES-NEXT: let mut a: i128 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut b: i128 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i128 = 0;
// REWRITES-NEXT: __retval = a + b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn print128({{arg[0-9]+}}: u128) {
// REWRITES-NEXT: let mut v: u128 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut hi: u64 = 0;
// REWRITES-NEXT: let mut lo: u64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 64;
// REWRITES-NEXT: hi = (v >> {{_v[0-9]+}}) as u64;
// REWRITES-NEXT: lo = v as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%llu:%llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, hi, lo) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mul128({{arg[0-9]+}}: u128, {{arg[0-9]+}}: u128) -> u128 {
// REWRITES-NEXT: let mut a: u128 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut b: u128 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: u128 = 0;
// REWRITES-NEXT: __retval = a * b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: i128 = 0;
// REWRITES-NEXT: let mut b: i128 = 0;
// REWRITES-NEXT: let mut sum: i128 = 0;
// REWRITES-NEXT: let mut x: u128 = 0;
// REWRITES-NEXT: let mut y: u128 = 0;
// REWRITES-NEXT: let mut prod: u128 = 0;
// REWRITES-NEXT: let mut cmp: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: a = 9000000000000000000i128;
// REWRITES-NEXT: b = 9000000000000000000i128;
// REWRITES-NEXT: sum = add128(a, b);
// REWRITES-NEXT: print128(sum as u128);
// REWRITES-NEXT: x = 1000000000000u128;
// REWRITES-NEXT: y = 1000000000000u128;
// REWRITES-NEXT: prod = mul128(x, y);
// REWRITES-NEXT: print128(prod);
// REWRITES-NEXT: let {{_v[0-9]+}}: i128 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: cmp = if sum > {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, cmp) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
