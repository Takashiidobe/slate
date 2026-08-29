#include <stdio.h>

int main(void) {
  unsigned int u  = 0x12345678u;
  unsigned int z  = 0u;
  int          s  = -12345;
  unsigned int sh = 5u;

  unsigned int rev            = __builtin_bitreverse32(u);
  unsigned int swapped        = __builtin_bswap32(u);
  int          leading        = __builtin_clz(u);
  int          trailing       = __builtin_ctz(u);
  int          first_set      = __builtin_ffs((int)u);
  int          zero_first     = __builtin_ffs((int)z);
  int          ones           = __builtin_popcount(u);
  int          odd            = __builtin_parity(u);
  int          redundant_sign = __builtin_clrsb(s);
  unsigned int left           = __builtin_rotateleft32(u, sh);
  unsigned int right          = __builtin_rotateright32(u, sh);

  printf("%u %u %d %d %d %d %d %d %d %u %u\n", rev, swapped, leading, trailing,
         first_set, zero_first, ones, odd, redundant_sign, left, right);
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
// LOWERING-NEXT:     let mut u: u32 = 0;
// LOWERING-NEXT:     let mut z: u32 = 0;
// LOWERING-NEXT:     let mut s: i32 = 0;
// LOWERING-NEXT:     let mut sh: u32 = 0;
// LOWERING-NEXT:     let mut rev: u32 = 0;
// LOWERING-NEXT:     let mut swapped: u32 = 0;
// LOWERING-NEXT:     let mut leading: i32 = 0;
// LOWERING-NEXT:     let mut trailing: i32 = 0;
// LOWERING-NEXT:     let mut first_set: i32 = 0;
// LOWERING-NEXT:     let mut zero_first: i32 = 0;
// LOWERING-NEXT:     let mut ones: i32 = 0;
// LOWERING-NEXT:     let mut odd: i32 = 0;
// LOWERING-NEXT:     let mut redundant_sign: i32 = 0;
// LOWERING-NEXT:     let mut left: u32 = 0;
// LOWERING-NEXT:     let mut right: u32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u32 = 305419896;
// LOWERING-NEXT:     u = _v1;
// LOWERING-NEXT:     let _v2: u32 = 0;
// LOWERING-NEXT:     z = _v2;
// LOWERING-NEXT:     let _v3: i32 = -12345;
// LOWERING-NEXT:     s = _v3;
// LOWERING-NEXT:     let _v4: u32 = 5;
// LOWERING-NEXT:     sh = _v4;
// LOWERING-NEXT:     let _v5: u32 = u;
// LOWERING-NEXT:     let _v6: u32 = _v5.reverse_bits();
// LOWERING-NEXT:     rev = _v6;
// LOWERING-NEXT:     let _v7: u32 = u;
// LOWERING-NEXT:     let _v8: u32 = _v7.swap_bytes();
// LOWERING-NEXT:     swapped = _v8;
// LOWERING-NEXT:     let _v9: u32 = u;
// LOWERING-NEXT:     let _v10: u32 = _v9.leading_zeros() as u32;
// LOWERING-NEXT:     let _v11: i32 = _v10 as i32;
// LOWERING-NEXT:     leading = _v11;
// LOWERING-NEXT:     let _v12: u32 = u;
// LOWERING-NEXT:     let _v13: u32 = _v12.trailing_zeros() as u32;
// LOWERING-NEXT:     let _v14: i32 = _v13 as i32;
// LOWERING-NEXT:     trailing = _v14;
// LOWERING-NEXT:     let _v15: u32 = u;
// LOWERING-NEXT:     let _v16: i32 = _v15 as i32;
// LOWERING-NEXT:     let _v17: i32 = if _v16 == 0 { 0 } else { (_v16.trailing_zeros() as i32) + 1 };
// LOWERING-NEXT:     first_set = _v17;
// LOWERING-NEXT:     let _v18: u32 = z;
// LOWERING-NEXT:     let _v19: i32 = _v18 as i32;
// LOWERING-NEXT:     let _v20: i32 = if _v19 == 0 { 0 } else { (_v19.trailing_zeros() as i32) + 1 };
// LOWERING-NEXT:     zero_first = _v20;
// LOWERING-NEXT:     let _v21: u32 = u;
// LOWERING-NEXT:     let _v22: u32 = _v21.count_ones() as u32;
// LOWERING-NEXT:     let _v23: i32 = _v22 as i32;
// LOWERING-NEXT:     ones = _v23;
// LOWERING-NEXT:     let _v24: u32 = u;
// LOWERING-NEXT:     let _v25: u32 = _v24.count_ones() & 1;
// LOWERING-NEXT:     let _v26: i32 = _v25 as i32;
// LOWERING-NEXT:     odd = _v26;
// LOWERING-NEXT:     let _v27: i32 = s;
// LOWERING-NEXT:     let _v28: i32 = (if _v27 < 0 { !_v27 } else { _v27 }.leading_zeros() as i32) - 1;
// LOWERING-NEXT:     redundant_sign = _v28;
// LOWERING-NEXT:     let _v29: u32 = u;
// LOWERING-NEXT:     let _v30: u32 = sh;
// LOWERING-NEXT:     let _v31: u32 = _v29.rotate_left(_v30);
// LOWERING-NEXT:     left = _v31;
// LOWERING-NEXT:     let _v32: u32 = u;
// LOWERING-NEXT:     let _v33: u32 = sh;
// LOWERING-NEXT:     let _v34: u32 = _v32.rotate_right(_v33);
// LOWERING-NEXT:     right = _v34;
// LOWERING-NEXT:     let _v35: *mut i8 = b"%u %u %d %d %d %d %d %d %d %u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v36: u32 = rev;
// LOWERING-NEXT:     let _v37: u32 = swapped;
// LOWERING-NEXT:     let _v38: i32 = leading;
// LOWERING-NEXT:     let _v39: i32 = trailing;
// LOWERING-NEXT:     let _v40: i32 = first_set;
// LOWERING-NEXT:     let _v41: i32 = zero_first;
// LOWERING-NEXT:     let _v42: i32 = ones;
// LOWERING-NEXT:     let _v43: i32 = odd;
// LOWERING-NEXT:     let _v44: i32 = redundant_sign;
// LOWERING-NEXT:     let _v45: u32 = left;
// LOWERING-NEXT:     let _v46: u32 = right;
// LOWERING-NEXT:     let _v47: i32 = unsafe { printf(_v35 as *const i8, _v36, _v37, _v38, _v39, _v40, _v41, _v42, _v43, _v44, _v45, _v46) };
// LOWERING-NEXT:     let _v48: i32 = 0;
// LOWERING-NEXT:     __retval = _v48;
// LOWERING-NEXT:     let _v49: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v49 as i32);
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
// REWRITES-NEXT: let mut u: u32 = 0;
// REWRITES-NEXT: let mut z: u32 = 0;
// REWRITES-NEXT: let mut s: i32 = 0;
// REWRITES-NEXT: let mut sh: u32 = 0;
// REWRITES-NEXT: let mut rev: u32 = 0;
// REWRITES-NEXT: let mut swapped: u32 = 0;
// REWRITES-NEXT: let mut leading: i32 = 0;
// REWRITES-NEXT: let mut trailing: i32 = 0;
// REWRITES-NEXT: let mut first_set: i32 = 0;
// REWRITES-NEXT: let mut zero_first: i32 = 0;
// REWRITES-NEXT: let mut ones: i32 = 0;
// REWRITES-NEXT: let mut odd: i32 = 0;
// REWRITES-NEXT: let mut redundant_sign: i32 = 0;
// REWRITES-NEXT: let mut left: u32 = 0;
// REWRITES-NEXT: let mut right: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: u = 305419896;
// REWRITES-NEXT: z = 0;
// REWRITES-NEXT: s = -12345;
// REWRITES-NEXT: sh = 5;
// REWRITES-NEXT: let _v5: u32 = u;
// REWRITES-NEXT: rev = _v5.reverse_bits();
// REWRITES-NEXT: let _v7: u32 = u;
// REWRITES-NEXT: swapped = _v7.swap_bytes();
// REWRITES-NEXT: let _v9: u32 = u;
// REWRITES-NEXT: leading = (_v9.leading_zeros() as u32) as i32;
// REWRITES-NEXT: let _v12: u32 = u;
// REWRITES-NEXT: trailing = (_v12.trailing_zeros() as u32) as i32;
// REWRITES-NEXT: let _v16: i32 = u as i32;
// REWRITES-NEXT: first_set = if _v16 == 0 { 0 } else { (_v16.trailing_zeros() as i32) + 1 };
// REWRITES-NEXT: let _v19: i32 = z as i32;
// REWRITES-NEXT: zero_first = if _v19 == 0 { 0 } else { (_v19.trailing_zeros() as i32) + 1 };
// REWRITES-NEXT: let _v21: u32 = u;
// REWRITES-NEXT: ones = (_v21.count_ones() as u32) as i32;
// REWRITES-NEXT: let _v24: u32 = u;
// REWRITES-NEXT: let _v25: u32 = _v24.count_ones() & 1;
// REWRITES-NEXT: odd = _v25 as i32;
// REWRITES-NEXT: let _v27: i32 = s;
// REWRITES-NEXT: redundant_sign = (if _v27 < 0 { !_v27 } else { _v27 }.leading_zeros() as i32) - 1;
// REWRITES-NEXT: let _v29: u32 = u;
// REWRITES-NEXT: left = _v29.rotate_left(sh);
// REWRITES-NEXT: let _v32: u32 = u;
// REWRITES-NEXT: right = _v32.rotate_right(sh);
// REWRITES-NEXT: let _v35: *mut i8 = b"%u %u %d %d %d %d %d %d %d %u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v47: i32 = unsafe { printf(_v35 as *const i8, rev, swapped, leading, trailing, first_set, zero_first, ones, odd, redundant_sign, left, right) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
