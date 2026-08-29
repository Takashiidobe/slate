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
// LOWERING-NEXT: fn add128(arg3: i128, arg4: i128) -> i128 {
// LOWERING-NEXT:     let mut a: i128 = 0;
// LOWERING-NEXT:     let mut b: i128 = 0;
// LOWERING-NEXT:     let mut __retval: i128 = 0;
// LOWERING-NEXT:     a = arg3;
// LOWERING-NEXT:     b = arg4;
// LOWERING-NEXT:     let _v0: i128 = a;
// LOWERING-NEXT:     let _v1: i128 = b;
// LOWERING-NEXT:     let _v2: i128 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i128 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn print128(arg2: u128) {
// LOWERING-NEXT:     let mut v: u128 = 0;
// LOWERING-NEXT:     let mut hi: u64 = 0;
// LOWERING-NEXT:     let mut lo: u64 = 0;
// LOWERING-NEXT:     v = arg2;
// LOWERING-NEXT:     let _v0: u128 = v;
// LOWERING-NEXT:     let _v1: i32 = 64;
// LOWERING-NEXT:     let _v2: u128 = _v0 >> _v1;
// LOWERING-NEXT:     let _v3: u64 = _v2 as u64;
// LOWERING-NEXT:     hi = _v3;
// LOWERING-NEXT:     let _v4: u128 = v;
// LOWERING-NEXT:     let _v5: u64 = _v4 as u64;
// LOWERING-NEXT:     lo = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%llu:%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: u64 = hi;
// LOWERING-NEXT:     let _v8: u64 = lo;
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v6 as *const i8, _v7, _v8) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mul128(arg0: u128, arg1: u128) -> u128 {
// LOWERING-NEXT:     let mut a: u128 = 0;
// LOWERING-NEXT:     let mut b: u128 = 0;
// LOWERING-NEXT:     let mut __retval: u128 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: u128 = a;
// LOWERING-NEXT:     let _v1: u128 = b;
// LOWERING-NEXT:     let _v2: u128 = _v0 * _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: u128 = __retval;
// LOWERING-NEXT:     return _v3;
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
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i128 = 9000000000000000000i128;
// LOWERING-NEXT:     a = _v1;
// LOWERING-NEXT:     let _v2: i128 = 9000000000000000000i128;
// LOWERING-NEXT:     b = _v2;
// LOWERING-NEXT:     let _v3: i128 = a;
// LOWERING-NEXT:     let _v4: i128 = b;
// LOWERING-NEXT:     let _v5: i128 = add128(_v3, _v4);
// LOWERING-NEXT:     sum = _v5;
// LOWERING-NEXT:     let _v6: i128 = sum;
// LOWERING-NEXT:     let _v7: u128 = _v6 as u128;
// LOWERING-NEXT:     print128(_v7);
// LOWERING-NEXT:     let _v8: u128 = 1000000000000u128;
// LOWERING-NEXT:     x = _v8;
// LOWERING-NEXT:     let _v9: u128 = 1000000000000u128;
// LOWERING-NEXT:     y = _v9;
// LOWERING-NEXT:     let _v10: u128 = x;
// LOWERING-NEXT:     let _v11: u128 = y;
// LOWERING-NEXT:     let _v12: u128 = mul128(_v10, _v11);
// LOWERING-NEXT:     prod = _v12;
// LOWERING-NEXT:     let _v13: u128 = prod;
// LOWERING-NEXT:     print128(_v13);
// LOWERING-NEXT:     let _v14: i128 = sum;
// LOWERING-NEXT:     let _v15: i128 = 0;
// LOWERING-NEXT:     let _v16: bool = _v14 > _v15;
// LOWERING-NEXT:     let _v17: i32 = 1;
// LOWERING-NEXT:     let _v18: i32 = 0;
// LOWERING-NEXT:     let _v19: i32 = if _v16 { _v17 } else { _v18 };
// LOWERING-NEXT:     cmp = _v19;
// LOWERING-NEXT:     let _v20: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v21: i32 = cmp;
// LOWERING-NEXT:     let _v22: i32 = unsafe { printf(_v20 as *const i8, _v21) };
// LOWERING-NEXT:     let _v23: i32 = 0;
// LOWERING-NEXT:     __retval = _v23;
// LOWERING-NEXT:     let _v24: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v24 as i32);
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
// REWRITES-NEXT: fn add128(arg3: i128, arg4: i128) -> i128 {
// REWRITES-NEXT: let mut a: i128 = arg3;
// REWRITES-NEXT: let mut b: i128 = arg4;
// REWRITES-NEXT: let mut __retval: i128 = 0;
// REWRITES-NEXT: __retval = a + b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn print128(arg2: u128) {
// REWRITES-NEXT: let mut v: u128 = arg2;
// REWRITES-NEXT: let mut hi: u64 = 0;
// REWRITES-NEXT: let mut lo: u64 = 0;
// REWRITES-NEXT: let _v1: i32 = 64;
// REWRITES-NEXT: hi = (v >> _v1) as u64;
// REWRITES-NEXT: lo = v as u64;
// REWRITES-NEXT: let _v6: *mut i8 = b"%llu:%llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v6 as *const i8, hi, lo) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mul128(arg0: u128, arg1: u128) -> u128 {
// REWRITES-NEXT: let mut a: u128 = arg0;
// REWRITES-NEXT: let mut b: u128 = arg1;
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
// REWRITES-NEXT: let _v15: i128 = 0;
// REWRITES-NEXT: let _v17: i32 = 1;
// REWRITES-NEXT: let _v18: i32 = 0;
// REWRITES-NEXT: cmp = if sum > _v15 { _v17 } else { _v18 };
// REWRITES-NEXT: let _v20: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: i32 = unsafe { printf(_v20 as *const i8, cmp) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
