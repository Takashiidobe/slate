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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut ri: i32 = 0;
// LOWERING-NEXT:     let mut ru: u32 = 0;
// LOWERING-NEXT:     let mut rll: i64 = 0;
// LOWERING-NEXT:     let mut add_i: i32 = 0;
// LOWERING-NEXT:     let mut sub_i: i32 = 0;
// LOWERING-NEXT:     let mut mul_i: i32 = 0;
// LOWERING-NEXT:     let mut add_u: i32 = 0;
// LOWERING-NEXT:     let mut mul_ll: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 0;
// LOWERING-NEXT:     ri = _v1;
// LOWERING-NEXT:     let _v2: u32 = 0;
// LOWERING-NEXT:     ru = _v2;
// LOWERING-NEXT:     let _v3: i64 = 0;
// LOWERING-NEXT:     rll = _v3;
// LOWERING-NEXT:     let _v4: i32 = 2147483647;
// LOWERING-NEXT:     let _v5: i32 = 1;
// LOWERING-NEXT:     let _v6 = _v4.overflowing_add(_v5);
// LOWERING-NEXT:     let _v7: i32 = _v6.0 as i32;
// LOWERING-NEXT:     let _v8: bool = _v6.1 || (_v6.0 < -2147483648 || _v6.0 > 2147483647);
// LOWERING-NEXT:     ri = _v7;
// LOWERING-NEXT:     let _v9: i32 = _v8 as i32;
// LOWERING-NEXT:     add_i = _v9;
// LOWERING-NEXT:     let _v10: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: i32 = add_i;
// LOWERING-NEXT:     let _v12: i32 = ri;
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v10 as *const i8, _v11, _v12) };
// LOWERING-NEXT:     let _v14: i32 = -2147483647;
// LOWERING-NEXT:     let _v15: i32 = 1;
// LOWERING-NEXT:     let _v16: i32 = _v14 - _v15;
// LOWERING-NEXT:     let _v17: i32 = 1;
// LOWERING-NEXT:     let _v18 = _v16.overflowing_sub(_v17);
// LOWERING-NEXT:     let _v19: i32 = _v18.0 as i32;
// LOWERING-NEXT:     let _v20: bool = _v18.1 || (_v18.0 < -2147483648 || _v18.0 > 2147483647);
// LOWERING-NEXT:     ri = _v19;
// LOWERING-NEXT:     let _v21: i32 = _v20 as i32;
// LOWERING-NEXT:     sub_i = _v21;
// LOWERING-NEXT:     let _v22: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v23: i32 = sub_i;
// LOWERING-NEXT:     let _v24: i32 = ri;
// LOWERING-NEXT:     let _v25: i32 = unsafe { printf(_v22 as *const i8, _v23, _v24) };
// LOWERING-NEXT:     let _v26: i32 = 1073741824;
// LOWERING-NEXT:     let _v27: i32 = 2;
// LOWERING-NEXT:     let _v28 = _v26.overflowing_mul(_v27);
// LOWERING-NEXT:     let _v29: i32 = _v28.0 as i32;
// LOWERING-NEXT:     let _v30: bool = _v28.1 || (_v28.0 < -2147483648 || _v28.0 > 2147483647);
// LOWERING-NEXT:     ri = _v29;
// LOWERING-NEXT:     let _v31: i32 = _v30 as i32;
// LOWERING-NEXT:     mul_i = _v31;
// LOWERING-NEXT:     let _v32: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v33: i32 = mul_i;
// LOWERING-NEXT:     let _v34: i32 = ri;
// LOWERING-NEXT:     let _v35: i32 = unsafe { printf(_v32 as *const i8, _v33, _v34) };
// LOWERING-NEXT:     let _v36: u32 = 4294967295u32;
// LOWERING-NEXT:     let _v37: u32 = 1;
// LOWERING-NEXT:     let _v38 = _v36.overflowing_add(_v37);
// LOWERING-NEXT:     let _v39: u32 = _v38.0 as u32;
// LOWERING-NEXT:     let _v40: bool = _v38.1 || _v38.0 > 4294967295;
// LOWERING-NEXT:     ru = _v39;
// LOWERING-NEXT:     let _v41: i32 = _v40 as i32;
// LOWERING-NEXT:     add_u = _v41;
// LOWERING-NEXT:     let _v42: *mut i8 = b"%d %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v43: i32 = add_u;
// LOWERING-NEXT:     let _v44: u32 = ru;
// LOWERING-NEXT:     let _v45: i32 = unsafe { printf(_v42 as *const i8, _v43, _v44) };
// LOWERING-NEXT:     let _v46: i64 = 3037000500i64;
// LOWERING-NEXT:     let _v47: i64 = 3037000500i64;
// LOWERING-NEXT:     let _v48 = _v46.overflowing_mul(_v47);
// LOWERING-NEXT:     let _v49: i64 = _v48.0 as i64;
// LOWERING-NEXT:     let _v50: bool = _v48.1 || (_v48.0 < -9223372036854775808 || _v48.0 > 9223372036854775807);
// LOWERING-NEXT:     rll = _v49;
// LOWERING-NEXT:     let _v51: i32 = _v50 as i32;
// LOWERING-NEXT:     mul_ll = _v51;
// LOWERING-NEXT:     let _v52: *mut i8 = b"%d %lld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v53: i32 = mul_ll;
// LOWERING-NEXT:     let _v54: i64 = rll;
// LOWERING-NEXT:     let _v55: i32 = unsafe { printf(_v52 as *const i8, _v53, _v54) };
// LOWERING-NEXT:     let _v56: i32 = 0;
// LOWERING-NEXT:     __retval = _v56;
// LOWERING-NEXT:     let _v57: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v57 as i32);
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
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut ri: i32 = 0;
// REWRITES-NEXT: let mut ru: u32 = 0;
// REWRITES-NEXT: let mut rll: i64 = 0;
// REWRITES-NEXT: let mut add_i: i32 = 0;
// REWRITES-NEXT: let mut sub_i: i32 = 0;
// REWRITES-NEXT: let mut mul_i: i32 = 0;
// REWRITES-NEXT: let mut add_u: i32 = 0;
// REWRITES-NEXT: let mut mul_ll: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: ri = 0;
// REWRITES-NEXT: ru = 0;
// REWRITES-NEXT: rll = 0;
// REWRITES-NEXT: let _v4: i32 = 2147483647;
// REWRITES-NEXT: let _v5: i32 = 1;
// REWRITES-NEXT: let _v6 = _v4.overflowing_add(_v5);
// REWRITES-NEXT: let _v8: bool = _v6.1 || (_v6.0 < -2147483648 || _v6.0 > 2147483647);
// REWRITES-NEXT: ri = _v6.0 as i32;
// REWRITES-NEXT: add_i = _v8 as i32;
// REWRITES-NEXT: let _v10: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v10 as *const i8, add_i, ri) };
// REWRITES-NEXT: let _v14: i32 = -2147483647;
// REWRITES-NEXT: let _v15: i32 = 1;
// REWRITES-NEXT: let _v16: i32 = _v14 - _v15;
// REWRITES-NEXT: let _v17: i32 = 1;
// REWRITES-NEXT: let _v18 = _v16.overflowing_sub(_v17);
// REWRITES-NEXT: let _v20: bool = _v18.1 || (_v18.0 < -2147483648 || _v18.0 > 2147483647);
// REWRITES-NEXT: ri = _v18.0 as i32;
// REWRITES-NEXT: sub_i = _v20 as i32;
// REWRITES-NEXT: let _v22: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v25: i32 = unsafe { printf(_v22 as *const i8, sub_i, ri) };
// REWRITES-NEXT: let _v26: i32 = 1073741824;
// REWRITES-NEXT: let _v27: i32 = 2;
// REWRITES-NEXT: let _v28 = _v26.overflowing_mul(_v27);
// REWRITES-NEXT: let _v30: bool = _v28.1 || (_v28.0 < -2147483648 || _v28.0 > 2147483647);
// REWRITES-NEXT: ri = _v28.0 as i32;
// REWRITES-NEXT: mul_i = _v30 as i32;
// REWRITES-NEXT: let _v32: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v35: i32 = unsafe { printf(_v32 as *const i8, mul_i, ri) };
// REWRITES-NEXT: let _v36: u32 = 4294967295u32;
// REWRITES-NEXT: let _v37: u32 = 1;
// REWRITES-NEXT: let _v38 = _v36.overflowing_add(_v37);
// REWRITES-NEXT: let _v40: bool = _v38.1 || _v38.0 > 4294967295;
// REWRITES-NEXT: ru = _v38.0 as u32;
// REWRITES-NEXT: add_u = _v40 as i32;
// REWRITES-NEXT: let _v42: *mut i8 = b"%d %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v45: i32 = unsafe { printf(_v42 as *const i8, add_u, ru) };
// REWRITES-NEXT: let _v46: i64 = 3037000500i64;
// REWRITES-NEXT: let _v47: i64 = 3037000500i64;
// REWRITES-NEXT: let _v48 = _v46.overflowing_mul(_v47);
// REWRITES-NEXT: let _v50: bool = _v48.1 || (_v48.0 < -9223372036854775808 || _v48.0 > 9223372036854775807);
// REWRITES-NEXT: rll = _v48.0 as i64;
// REWRITES-NEXT: mul_ll = _v50 as i32;
// REWRITES-NEXT: let _v52: *mut i8 = b"%d %lld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v55: i32 = unsafe { printf(_v52 as *const i8, mul_ll, rll) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
