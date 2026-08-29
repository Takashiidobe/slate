#include <limits.h>
#include <stdio.h>

typedef unsigned __int128 U128;
typedef _BitInt(200) S200;
typedef unsigned _BitInt(200) U200;
typedef int v4si __attribute__((vector_size(16)));

int main(void) {
  int add_i = __builtin_elementwise_add_sat(INT_MAX - 5, 10);
  printf("%d\n", add_i);

  int add_i_no_sat = __builtin_elementwise_add_sat(2, 3);
  printf("%d\n", add_i_no_sat);

  short sub_s = __builtin_elementwise_sub_sat((short)(SHRT_MIN + 5), (short)10);
  printf("%d\n", sub_s);

  unsigned sub_u = __builtin_elementwise_sub_sat(5u, 10u);
  printf("%u\n", sub_u);

  U128 u128a = (U128)0 - 1;
  U128 add_u128 = __builtin_elementwise_add_sat(u128a, (U128)5);
  printf("%d\n", add_u128 == (U128)0 - 1);

  S200 s200a = -1;
  S200 add_s200 = __builtin_elementwise_add_sat(s200a, (S200)5);
  printf("%d\n", (int)add_s200);

  U200 u200a = 3;
  U200 sub_u200 = __builtin_elementwise_sub_sat(u200a, (U200)10);
  printf("%d\n", sub_u200 == 0);

  v4si va = {2147483647, 1, -2147483647 - 1, 0};
  v4si vb = {10, 1, -10, 0};
  v4si vr = __builtin_elementwise_add_sat(va, vb);
  printf("%d %d %d %d\n", vr[0], vr[1], vr[2], vr[3]);

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
// LOWERING-NEXT:     let mut add_i: i32 = 0;
// LOWERING-NEXT:     let mut add_i_no_sat: i32 = 0;
// LOWERING-NEXT:     let mut sub_s: i16 = 0;
// LOWERING-NEXT:     let mut sub_u: u32 = 0;
// LOWERING-NEXT:     let mut u128a: u128 = 0;
// LOWERING-NEXT:     let mut add_u128: u128 = 0;
// LOWERING-NEXT:     let mut s200a: aligned::Aligned<aligned::A8, bitint::BInt<200, 4, 32>> = aligned::Aligned(bitint::BInt::<200, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut add_s200: aligned::Aligned<aligned::A8, bitint::BInt<200, 4, 32>> = aligned::Aligned(bitint::BInt::<200, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut u200a: aligned::Aligned<aligned::A8, bitint::BUint<200, 4, 32>> = aligned::Aligned(bitint::BUint::<200, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut sub_u200: aligned::Aligned<aligned::A8, bitint::BUint<200, 4, 32>> = aligned::Aligned(bitint::BUint::<200, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut va: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut vb: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut vr: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = i32::MAX as i32;
// LOWERING-NEXT:     add_i = _v1;
// LOWERING-NEXT:     let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: i32 = add_i;
// LOWERING-NEXT:     let _v4: i32 = unsafe { printf(_v2 as *const i8, _v3) };
// LOWERING-NEXT:     let _v5: i32 = 5;
// LOWERING-NEXT:     add_i_no_sat = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = add_i_no_sat;
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v6 as *const i8, _v7) };
// LOWERING-NEXT:     let _v9: i16 = -32768;
// LOWERING-NEXT:     sub_s = _v9;
// LOWERING-NEXT:     let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: i16 = sub_s;
// LOWERING-NEXT:     let _v12: i32 = _v11 as i32;
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v10 as *const i8, _v12) };
// LOWERING-NEXT:     let _v14: u32 = 0;
// LOWERING-NEXT:     sub_u = _v14;
// LOWERING-NEXT:     let _v15: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: u32 = sub_u;
// LOWERING-NEXT:     let _v17: i32 = unsafe { printf(_v15 as *const i8, _v16) };
// LOWERING-NEXT:     let _v18: u128 = 0;
// LOWERING-NEXT:     let _v19: u128 = 1;
// LOWERING-NEXT:     let _v20: u128 = _v18 - _v19;
// LOWERING-NEXT:     u128a = _v20;
// LOWERING-NEXT:     let _v21: u128 = u128a;
// LOWERING-NEXT:     let _v22: u128 = 5;
// LOWERING-NEXT:     let _v23: u128 = _v21.saturating_add(_v22);
// LOWERING-NEXT:     add_u128 = _v23;
// LOWERING-NEXT:     let _v24: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: u128 = add_u128;
// LOWERING-NEXT:     let _v26: u128 = 0;
// LOWERING-NEXT:     let _v27: u128 = 1;
// LOWERING-NEXT:     let _v28: u128 = _v26 - _v27;
// LOWERING-NEXT:     let _v29: bool = _v25 == _v28;
// LOWERING-NEXT:     let _v30: i32 = _v29 as i32;
// LOWERING-NEXT:     let _v31: i32 = unsafe { printf(_v24 as *const i8, _v30) };
// LOWERING-NEXT:     let _v32: bitint::BInt<200, 4, 32> = bitint::BInt::<200, 4, 32>::from_decimal_str("-1");
// LOWERING-NEXT:     *s200a = _v32;
// LOWERING-NEXT:     let _v33: bitint::BInt<200, 4, 32> = *s200a;
// LOWERING-NEXT:     let _v34: bitint::BInt<200, 4, 32> = bitint::BInt::<200, 4, 32>::from_decimal_str("5");
// LOWERING-NEXT:     let _v35: bitint::BInt<200, 4, 32> = _v33.saturating_add(_v34);
// LOWERING-NEXT:     *add_s200 = _v35;
// LOWERING-NEXT:     let _v36: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v37: bitint::BInt<200, 4, 32> = *add_s200;
// LOWERING-NEXT:     let _v38: i32 = _v37.to_i128() as i32;
// LOWERING-NEXT:     let _v39: i32 = unsafe { printf(_v36 as *const i8, _v38) };
// LOWERING-NEXT:     let _v40: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("3");
// LOWERING-NEXT:     *u200a = _v40;
// LOWERING-NEXT:     let _v41: bitint::BUint<200, 4, 32> = *u200a;
// LOWERING-NEXT:     let _v42: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("10");
// LOWERING-NEXT:     let _v43: bitint::BUint<200, 4, 32> = _v41.saturating_sub(_v42);
// LOWERING-NEXT:     *sub_u200 = _v43;
// LOWERING-NEXT:     let _v44: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v45: bitint::BUint<200, 4, 32> = *sub_u200;
// LOWERING-NEXT:     let _v46: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("0");
// LOWERING-NEXT:     let _v47: bool = _v45 == _v46;
// LOWERING-NEXT:     let _v48: i32 = _v47 as i32;
// LOWERING-NEXT:     let _v49: i32 = unsafe { printf(_v44 as *const i8, _v48) };
// LOWERING-NEXT:     let _v50: [i32; 4] = [2147483647, 1, -2147483648, 0];
// LOWERING-NEXT:     *va = _v50;
// LOWERING-NEXT:     let _v51: [i32; 4] = [10, 1, -10, 0];
// LOWERING-NEXT:     *vb = _v51;
// LOWERING-NEXT:     let _v52: [i32; 4] = *va;
// LOWERING-NEXT:     let _v53: [i32; 4] = *vb;
// LOWERING-NEXT:     let _v54: [i32; 4] = [_v52[0usize].saturating_add(_v53[0usize]), _v52[1usize].saturating_add(_v53[1usize]), _v52[2usize].saturating_add(_v53[2usize]), _v52[3usize].saturating_add(_v53[3usize])];
// LOWERING-NEXT:     *vr = _v54;
// LOWERING-NEXT:     let _v55: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v56: [i32; 4] = *vr;
// LOWERING-NEXT:     let _v57: i32 = 0;
// LOWERING-NEXT:     let _v58: i32 = _v56[(_v57 as usize)];
// LOWERING-NEXT:     let _v59: [i32; 4] = *vr;
// LOWERING-NEXT:     let _v60: i32 = 1;
// LOWERING-NEXT:     let _v61: i32 = _v59[(_v60 as usize)];
// LOWERING-NEXT:     let _v62: [i32; 4] = *vr;
// LOWERING-NEXT:     let _v63: i32 = 2;
// LOWERING-NEXT:     let _v64: i32 = _v62[(_v63 as usize)];
// LOWERING-NEXT:     let _v65: [i32; 4] = *vr;
// LOWERING-NEXT:     let _v66: i32 = 3;
// LOWERING-NEXT:     let _v67: i32 = _v65[(_v66 as usize)];
// LOWERING-NEXT:     let _v68: i32 = unsafe { printf(_v55 as *const i8, _v58, _v61, _v64, _v67) };
// LOWERING-NEXT:     let _v69: i32 = 0;
// LOWERING-NEXT:     __retval = _v69;
// LOWERING-NEXT:     let _v70: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v70 as i32);
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
// REWRITES-NEXT: let mut add_i: i32 = 0;
// REWRITES-NEXT: let mut add_i_no_sat: i32 = 0;
// REWRITES-NEXT: let mut sub_s: i16 = 0;
// REWRITES-NEXT: let mut sub_u: u32 = 0;
// REWRITES-NEXT: let mut u128a: u128 = 0;
// REWRITES-NEXT: let mut add_u128: u128 = 0;
// REWRITES-NEXT: let mut s200a: aligned::Aligned<aligned::A8, bitint::BInt<200, 4, 32>> = aligned::Aligned(bitint::BInt::<200, 4, 32>::ZERO);
// REWRITES-NEXT: let mut add_s200: aligned::Aligned<aligned::A8, bitint::BInt<200, 4, 32>> = aligned::Aligned(bitint::BInt::<200, 4, 32>::ZERO);
// REWRITES-NEXT: let mut u200a: aligned::Aligned<aligned::A8, bitint::BUint<200, 4, 32>> = aligned::Aligned(bitint::BUint::<200, 4, 32>::ZERO);
// REWRITES-NEXT: let mut sub_u200: aligned::Aligned<aligned::A8, bitint::BUint<200, 4, 32>> = aligned::Aligned(bitint::BUint::<200, 4, 32>::ZERO);
// REWRITES-NEXT: let mut va: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut vb: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut vr: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: add_i = i32::MAX as i32;
// REWRITES-NEXT: let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: i32 = unsafe { printf(_v2 as *const i8, add_i) };
// REWRITES-NEXT: add_i_no_sat = 5;
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v6 as *const i8, add_i_no_sat) };
// REWRITES-NEXT: sub_s = -32768;
// REWRITES-NEXT: let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v10 as *const i8, sub_s as i32) };
// REWRITES-NEXT: sub_u = 0;
// REWRITES-NEXT: let _v15: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: i32 = unsafe { printf(_v15 as *const i8, sub_u) };
// REWRITES-NEXT: let _v18: u128 = 0;
// REWRITES-NEXT: let _v19: u128 = 1;
// REWRITES-NEXT: u128a = _v18 - _v19;
// REWRITES-NEXT: let _v21: u128 = u128a;
// REWRITES-NEXT: let _v22: u128 = 5;
// REWRITES-NEXT: add_u128 = _v21.saturating_add(_v22);
// REWRITES-NEXT: let _v24: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v26: u128 = 0;
// REWRITES-NEXT: let _v27: u128 = 1;
// REWRITES-NEXT: let _v31: i32 = unsafe { printf(_v24 as *const i8, (add_u128 == _v26 - _v27) as i32) };
// REWRITES-NEXT: let _v32: bitint::BInt<200, 4, 32> = bitint::BInt::<200, 4, 32>::from_decimal_str("-1");
// REWRITES-NEXT: *s200a = _v32;
// REWRITES-NEXT: let _v33: bitint::BInt<200, 4, 32> = *s200a;
// REWRITES-NEXT: let _v34: bitint::BInt<200, 4, 32> = bitint::BInt::<200, 4, 32>::from_decimal_str("5");
// REWRITES-NEXT: let _v35: bitint::BInt<200, 4, 32> = _v33.saturating_add(_v34);
// REWRITES-NEXT: *add_s200 = _v35;
// REWRITES-NEXT: let _v36: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v37: bitint::BInt<200, 4, 32> = *add_s200;
// REWRITES-NEXT: let _v38: i32 = _v37.to_i128() as i32;
// REWRITES-NEXT: let _v39: i32 = unsafe { printf(_v36 as *const i8, _v38) };
// REWRITES-NEXT: let _v40: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("3");
// REWRITES-NEXT: *u200a = _v40;
// REWRITES-NEXT: let _v41: bitint::BUint<200, 4, 32> = *u200a;
// REWRITES-NEXT: let _v42: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("10");
// REWRITES-NEXT: let _v43: bitint::BUint<200, 4, 32> = _v41.saturating_sub(_v42);
// REWRITES-NEXT: *sub_u200 = _v43;
// REWRITES-NEXT: let _v44: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v45: bitint::BUint<200, 4, 32> = *sub_u200;
// REWRITES-NEXT: let _v46: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("0");
// REWRITES-NEXT: let _v49: i32 = unsafe { printf(_v44 as *const i8, (_v45 == _v46) as i32) };
// REWRITES-NEXT: *va = [2147483647, 1, -2147483648, 0];
// REWRITES-NEXT: *vb = [10, 1, -10, 0];
// REWRITES-NEXT: let _v52: [i32; 4] = *va;
// REWRITES-NEXT: let _v53: [i32; 4] = *vb;
// REWRITES-NEXT: let _v54: [i32; 4] = [_v52[0usize].saturating_add(_v53[0usize]), _v52[1usize].saturating_add(_v53[1usize]), _v52[2usize].saturating_add(_v53[2usize]), _v52[3usize].saturating_add(_v53[3usize])];
// REWRITES-NEXT: *vr = _v54;
// REWRITES-NEXT: let _v55: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v57: i32 = 0;
// REWRITES-NEXT: let _v60: i32 = 1;
// REWRITES-NEXT: let _v63: i32 = 2;
// REWRITES-NEXT: let _v66: i32 = 3;
// REWRITES-NEXT: let _v68: i32 = unsafe { printf(_v55 as *const i8, (*vr)[(_v57 as usize)], (*vr)[(_v60 as usize)], (*vr)[(_v63 as usize)], (*vr)[(_v66 as usize)]) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
