#include <stdio.h>

unsigned add_with_carry(unsigned a, unsigned b, unsigned carry_in,
                         unsigned *carry_out) {
  return __builtin_addc(a, b, carry_in, carry_out);
}

unsigned sub_with_borrow(unsigned a, unsigned b, unsigned borrow_in,
                          unsigned *borrow_out) {
  return __builtin_subc(a, b, borrow_in, borrow_out);
}

int main(void) {
  unsigned carry, borrow;

  unsigned sum = add_with_carry(0xFFFFFFFFu, 1u, 0u, &carry);
  printf("%u %u\n", sum, carry);

  unsigned sum2 = add_with_carry(1u, 1u, 0u, &carry);
  printf("%u %u\n", sum2, carry);

  unsigned diff = sub_with_borrow(0u, 1u, 0u, &borrow);
  printf("%u %u\n", diff, borrow);

  unsigned diff2 = sub_with_borrow(5u, 3u, 0u, &borrow);
  printf("%u %u\n", diff2, borrow);

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
// LOWERING-NEXT: unsafe fn add_with_carry(arg4: u32, arg5: u32, arg6: u32, arg7: *mut u32) -> u32 {
// LOWERING-NEXT:     let mut a: u32 = 0;
// LOWERING-NEXT:     let mut b: u32 = 0;
// LOWERING-NEXT:     let mut carry_in: u32 = 0;
// LOWERING-NEXT:     let mut carry_out: *mut u32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: u32 = 0;
// LOWERING-NEXT:     a = arg4;
// LOWERING-NEXT:     b = arg5;
// LOWERING-NEXT:     carry_in = arg6;
// LOWERING-NEXT:     carry_out = arg7;
// LOWERING-NEXT:     let _v0: u32 = a;
// LOWERING-NEXT:     let _v1: u32 = b;
// LOWERING-NEXT:     let _v2: u32 = carry_in;
// LOWERING-NEXT:     let _v3: *mut u32 = carry_out;
// LOWERING-NEXT:     let _v4 = _v0.overflowing_add(_v1);
// LOWERING-NEXT:     let _v5: u32 = _v4.0 as u32;
// LOWERING-NEXT:     let _v6: bool = _v4.1 || _v4.0 > 4294967295;
// LOWERING-NEXT:     let _v7 = _v5.overflowing_add(_v2);
// LOWERING-NEXT:     let _v8: u32 = _v7.0 as u32;
// LOWERING-NEXT:     let _v9: bool = _v7.1 || _v7.0 > 4294967295;
// LOWERING-NEXT:     let _v10: bool = _v6 | _v9;
// LOWERING-NEXT:     let _v11: u32 = _v10 as u32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v3 = _v11;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     __retval = _v8;
// LOWERING-NEXT:     let _v12: u32 = __retval;
// LOWERING-NEXT:     return _v12;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn sub_with_borrow(arg0: u32, arg1: u32, arg2: u32, arg3: *mut u32) -> u32 {
// LOWERING-NEXT:     let mut a: u32 = 0;
// LOWERING-NEXT:     let mut b: u32 = 0;
// LOWERING-NEXT:     let mut borrow_in: u32 = 0;
// LOWERING-NEXT:     let mut borrow_out: *mut u32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: u32 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     borrow_in = arg2;
// LOWERING-NEXT:     borrow_out = arg3;
// LOWERING-NEXT:     let _v0: u32 = a;
// LOWERING-NEXT:     let _v1: u32 = b;
// LOWERING-NEXT:     let _v2: u32 = borrow_in;
// LOWERING-NEXT:     let _v3: *mut u32 = borrow_out;
// LOWERING-NEXT:     let _v4 = _v0.overflowing_sub(_v1);
// LOWERING-NEXT:     let _v5: u32 = _v4.0 as u32;
// LOWERING-NEXT:     let _v6: bool = _v4.1 || _v4.0 > 4294967295;
// LOWERING-NEXT:     let _v7 = _v5.overflowing_sub(_v2);
// LOWERING-NEXT:     let _v8: u32 = _v7.0 as u32;
// LOWERING-NEXT:     let _v9: bool = _v7.1 || _v7.0 > 4294967295;
// LOWERING-NEXT:     let _v10: bool = _v6 | _v9;
// LOWERING-NEXT:     let _v11: u32 = _v10 as u32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v3 = _v11;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     __retval = _v8;
// LOWERING-NEXT:     let _v12: u32 = __retval;
// LOWERING-NEXT:     return _v12;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut carry: u32 = 0;
// LOWERING-NEXT:     let mut borrow: u32 = 0;
// LOWERING-NEXT:     let mut sum: u32 = 0;
// LOWERING-NEXT:     let mut sum2: u32 = 0;
// LOWERING-NEXT:     let mut diff: u32 = 0;
// LOWERING-NEXT:     let mut diff2: u32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u32 = 4294967295u32;
// LOWERING-NEXT:     let _v2: u32 = 1;
// LOWERING-NEXT:     let _v3: u32 = 0;
// LOWERING-NEXT:     let _v4: u32 = unsafe { add_with_carry(_v1, _v2, _v3, std::ptr::addr_of_mut!(carry)) };
// LOWERING-NEXT:     sum = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: u32 = sum;
// LOWERING-NEXT:     let _v7: u32 = carry;
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v5 as *const i8, _v6, _v7) };
// LOWERING-NEXT:     let _v9: u32 = 1;
// LOWERING-NEXT:     let _v10: u32 = 1;
// LOWERING-NEXT:     let _v11: u32 = 0;
// LOWERING-NEXT:     let _v12: u32 = unsafe { add_with_carry(_v9, _v10, _v11, std::ptr::addr_of_mut!(carry)) };
// LOWERING-NEXT:     sum2 = _v12;
// LOWERING-NEXT:     let _v13: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: u32 = sum2;
// LOWERING-NEXT:     let _v15: u32 = carry;
// LOWERING-NEXT:     let _v16: i32 = unsafe { printf(_v13 as *const i8, _v14, _v15) };
// LOWERING-NEXT:     let _v17: u32 = 0;
// LOWERING-NEXT:     let _v18: u32 = 1;
// LOWERING-NEXT:     let _v19: u32 = 0;
// LOWERING-NEXT:     let _v20: u32 = unsafe { sub_with_borrow(_v17, _v18, _v19, std::ptr::addr_of_mut!(borrow)) };
// LOWERING-NEXT:     diff = _v20;
// LOWERING-NEXT:     let _v21: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v22: u32 = diff;
// LOWERING-NEXT:     let _v23: u32 = borrow;
// LOWERING-NEXT:     let _v24: i32 = unsafe { printf(_v21 as *const i8, _v22, _v23) };
// LOWERING-NEXT:     let _v25: u32 = 5;
// LOWERING-NEXT:     let _v26: u32 = 3;
// LOWERING-NEXT:     let _v27: u32 = 0;
// LOWERING-NEXT:     let _v28: u32 = unsafe { sub_with_borrow(_v25, _v26, _v27, std::ptr::addr_of_mut!(borrow)) };
// LOWERING-NEXT:     diff2 = _v28;
// LOWERING-NEXT:     let _v29: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v30: u32 = diff2;
// LOWERING-NEXT:     let _v31: u32 = borrow;
// LOWERING-NEXT:     let _v32: i32 = unsafe { printf(_v29 as *const i8, _v30, _v31) };
// LOWERING-NEXT:     let _v33: i32 = 0;
// LOWERING-NEXT:     __retval = _v33;
// LOWERING-NEXT:     let _v34: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v34 as i32);
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
// REWRITES-NEXT: unsafe fn add_with_carry(arg4: u32, arg5: u32, arg6: u32, arg7: *mut u32) -> u32 {
// REWRITES-NEXT: let mut a: u32 = arg4;
// REWRITES-NEXT: let mut b: u32 = arg5;
// REWRITES-NEXT: let mut carry_in: u32 = arg6;
// REWRITES-NEXT: let mut __retval: u32 = 0;
// REWRITES-NEXT: let _v0: u32 = a;
// REWRITES-NEXT: let _v2: u32 = carry_in;
// REWRITES-NEXT: let _v3: *mut u32 = arg7;
// REWRITES-NEXT: let _v4 = _v0.overflowing_add(b);
// REWRITES-NEXT: let _v5: u32 = _v4.0 as u32;
// REWRITES-NEXT: let _v6: bool = _v4.1 || _v4.0 > 4294967295;
// REWRITES-NEXT: let _v7 = _v5.overflowing_add(_v2);
// REWRITES-NEXT: let _v8: u32 = _v7.0 as u32;
// REWRITES-NEXT: let _v9: bool = _v7.1 || _v7.0 > 4294967295;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v3 = (_v6 | _v9) as u32;
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = _v8;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn sub_with_borrow(arg0: u32, arg1: u32, arg2: u32, arg3: *mut u32) -> u32 {
// REWRITES-NEXT: let mut a: u32 = arg0;
// REWRITES-NEXT: let mut b: u32 = arg1;
// REWRITES-NEXT: let mut borrow_in: u32 = arg2;
// REWRITES-NEXT: let mut __retval: u32 = 0;
// REWRITES-NEXT: let _v0: u32 = a;
// REWRITES-NEXT: let _v2: u32 = borrow_in;
// REWRITES-NEXT: let _v3: *mut u32 = arg3;
// REWRITES-NEXT: let _v4 = _v0.overflowing_sub(b);
// REWRITES-NEXT: let _v5: u32 = _v4.0 as u32;
// REWRITES-NEXT: let _v6: bool = _v4.1 || _v4.0 > 4294967295;
// REWRITES-NEXT: let _v7 = _v5.overflowing_sub(_v2);
// REWRITES-NEXT: let _v8: u32 = _v7.0 as u32;
// REWRITES-NEXT: let _v9: bool = _v7.1 || _v7.0 > 4294967295;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v3 = (_v6 | _v9) as u32;
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = _v8;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut carry: u32 = 0;
// REWRITES-NEXT: let mut borrow: u32 = 0;
// REWRITES-NEXT: let mut sum: u32 = 0;
// REWRITES-NEXT: let mut sum2: u32 = 0;
// REWRITES-NEXT: let mut diff: u32 = 0;
// REWRITES-NEXT: let mut diff2: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u32 = 4294967295u32;
// REWRITES-NEXT: let _v2: u32 = 1;
// REWRITES-NEXT: let _v3: u32 = 0;
// REWRITES-NEXT: sum = unsafe { add_with_carry(_v1, _v2, _v3, std::ptr::addr_of_mut!(carry)) };
// REWRITES-NEXT: let _v5: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v5 as *const i8, sum, carry) };
// REWRITES-NEXT: let _v9: u32 = 1;
// REWRITES-NEXT: let _v10: u32 = 1;
// REWRITES-NEXT: let _v11: u32 = 0;
// REWRITES-NEXT: sum2 = unsafe { add_with_carry(_v9, _v10, _v11, std::ptr::addr_of_mut!(carry)) };
// REWRITES-NEXT: let _v13: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: i32 = unsafe { printf(_v13 as *const i8, sum2, carry) };
// REWRITES-NEXT: let _v17: u32 = 0;
// REWRITES-NEXT: let _v18: u32 = 1;
// REWRITES-NEXT: let _v19: u32 = 0;
// REWRITES-NEXT: diff = unsafe { sub_with_borrow(_v17, _v18, _v19, std::ptr::addr_of_mut!(borrow)) };
// REWRITES-NEXT: let _v21: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v24: i32 = unsafe { printf(_v21 as *const i8, diff, borrow) };
// REWRITES-NEXT: let _v25: u32 = 5;
// REWRITES-NEXT: let _v26: u32 = 3;
// REWRITES-NEXT: let _v27: u32 = 0;
// REWRITES-NEXT: diff2 = unsafe { sub_with_borrow(_v25, _v26, _v27, std::ptr::addr_of_mut!(borrow)) };
// REWRITES-NEXT: let _v29: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v32: i32 = unsafe { printf(_v29 as *const i8, diff2, borrow) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
