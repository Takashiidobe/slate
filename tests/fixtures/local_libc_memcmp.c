#include <stdio.h>
#include <string.h>

static int cmp_bytes(const unsigned char *a, int alen, const unsigned char *b,
                     int blen) {
  int sa = 0, sb = 0;
  for (int i = 0; i < alen; i++)
    sa += a[i];
  for (int i = 0; i < blen; i++)
    sb += b[i];
  int order = memcmp(a, b, 3);
  int sign  = (order > 0) - (order < 0);
  return sign * 1000 + (sa - sb);
}

int main(void) {
  unsigned char x[] = {1, 2, 3};
  unsigned char y[] = {1, 2, 4};
  unsigned char z[] = {1, 2, 3};
  printf("%d %d %d\n", cmp_bytes(x, 3, y, 3), cmp_bytes(y, 3, x, 3),
         cmp_bytes(x, 3, z, 3));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn cmp_bytes(arg0: *mut u8, arg1: i32, arg2: *mut u8, arg3: i32) -> i32 {
// LOWERING-NEXT:     let mut a: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut alen: i32 = 0;
// LOWERING-NEXT:     let mut b: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut blen: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut sa: i32 = 0;
// LOWERING-NEXT:     let mut sb: i32 = 0;
// LOWERING-NEXT:     let mut order: i32 = 0;
// LOWERING-NEXT:     let mut sign: i32 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     alen = arg1;
// LOWERING-NEXT:     b = arg2;
// LOWERING-NEXT:     blen = arg3;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     sa = _v0;
// LOWERING-NEXT:     let _v1: i32 = 0;
// LOWERING-NEXT:     sb = _v1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v2: i32 = 0;
// LOWERING-NEXT:         i = _v2;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v3: i32 = i;
// LOWERING-NEXT:             let _v4: i32 = alen;
// LOWERING-NEXT:             let _v5: bool = _v3 < _v4;
// LOWERING-NEXT:             if !_v5 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v6: i32 = i;
// LOWERING-NEXT:             let _v7: i64 = _v6 as i64;
// LOWERING-NEXT:             let _v8: *mut u8 = a;
// LOWERING-NEXT:             let _v9: *mut u8 = unsafe { _v8.offset(_v7 as isize) };
// LOWERING-NEXT:             let _v10: u8 = unsafe { *_v9 };
// LOWERING-NEXT:             let _v11: i32 = _v10 as i32;
// LOWERING-NEXT:             let _v12: i32 = sa;
// LOWERING-NEXT:             let _v13: i32 = _v12 + _v11;
// LOWERING-NEXT:             sa = _v13;
// LOWERING-NEXT:             let _v14: i32 = i;
// LOWERING-NEXT:             let _v15: i32 = _v14 + 1;
// LOWERING-NEXT:             i = _v15;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i2: i32 = 0;
// LOWERING-NEXT:         let _v16: i32 = 0;
// LOWERING-NEXT:         i2 = _v16;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v17: i32 = i2;
// LOWERING-NEXT:             let _v18: i32 = blen;
// LOWERING-NEXT:             let _v19: bool = _v17 < _v18;
// LOWERING-NEXT:             if !_v19 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v20: i32 = i2;
// LOWERING-NEXT:             let _v21: i64 = _v20 as i64;
// LOWERING-NEXT:             let _v22: *mut u8 = b;
// LOWERING-NEXT:             let _v23: *mut u8 = unsafe { _v22.offset(_v21 as isize) };
// LOWERING-NEXT:             let _v24: u8 = unsafe { *_v23 };
// LOWERING-NEXT:             let _v25: i32 = _v24 as i32;
// LOWERING-NEXT:             let _v26: i32 = sb;
// LOWERING-NEXT:             let _v27: i32 = _v26 + _v25;
// LOWERING-NEXT:             sb = _v27;
// LOWERING-NEXT:             let _v28: i32 = i2;
// LOWERING-NEXT:             let _v29: i32 = _v28 + 1;
// LOWERING-NEXT:             i2 = _v29;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v30: *mut u8 = a;
// LOWERING-NEXT:     let _v31: *mut core::ffi::c_void = _v30 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v32: *mut u8 = b;
// LOWERING-NEXT:     let _v33: *mut core::ffi::c_void = _v32 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v34: u64 = 3;
// LOWERING-NEXT:     let _v35: i32 = unsafe { memcmp(_v31 as *const core::ffi::c_void, _v33 as *const core::ffi::c_void, _v34 as usize) };
// LOWERING-NEXT:     order = _v35;
// LOWERING-NEXT:     let _v36: i32 = order;
// LOWERING-NEXT:     let _v37: i32 = 0;
// LOWERING-NEXT:     let _v38: bool = _v36 > _v37;
// LOWERING-NEXT:     let _v39: i32 = _v38 as i32;
// LOWERING-NEXT:     let _v40: i32 = order;
// LOWERING-NEXT:     let _v41: i32 = 0;
// LOWERING-NEXT:     let _v42: bool = _v40 < _v41;
// LOWERING-NEXT:     let _v43: i32 = _v42 as i32;
// LOWERING-NEXT:     let _v44: i32 = _v39 - _v43;
// LOWERING-NEXT:     sign = _v44;
// LOWERING-NEXT:     let _v45: i32 = sign;
// LOWERING-NEXT:     let _v46: i32 = 1000;
// LOWERING-NEXT:     let _v47: i32 = _v45 * _v46;
// LOWERING-NEXT:     let _v48: i32 = sa;
// LOWERING-NEXT:     let _v49: i32 = sb;
// LOWERING-NEXT:     let _v50: i32 = _v48 - _v49;
// LOWERING-NEXT:     let _v51: i32 = _v47 + _v50;
// LOWERING-NEXT:     __retval = _v51;
// LOWERING-NEXT:     let _v52: i32 = __retval;
// LOWERING-NEXT:     return _v52;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: [u8; 3] = [0; 3];
// LOWERING-NEXT:     let mut y: [u8; 3] = [0; 3];
// LOWERING-NEXT:     let mut z: [u8; 3] = [0; 3];
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     x = [1, 2, 3];
// LOWERING-NEXT:     y = [1, 2, 4];
// LOWERING-NEXT:     z = [1, 2, 3];
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut u8 = x.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v3: i32 = 3;
// LOWERING-NEXT:     let _v4: *mut u8 = y.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v5: i32 = 3;
// LOWERING-NEXT:     let _v6: i32 = cmp_bytes(_v2, _v3, _v4, _v5);
// LOWERING-NEXT:     let _v7: *mut u8 = y.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v8: i32 = 3;
// LOWERING-NEXT:     let _v9: *mut u8 = x.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v10: i32 = 3;
// LOWERING-NEXT:     let _v11: i32 = cmp_bytes(_v7, _v8, _v9, _v10);
// LOWERING-NEXT:     let _v12: *mut u8 = x.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v13: i32 = 3;
// LOWERING-NEXT:     let _v14: *mut u8 = z.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v15: i32 = 3;
// LOWERING-NEXT:     let _v16: i32 = cmp_bytes(_v12, _v13, _v14, _v15);
// LOWERING-NEXT:     let _v17: i32 = unsafe { printf(_v1 as *const i8, _v6, _v11, _v16) };
// LOWERING-NEXT:     let _v18: i32 = 0;
// LOWERING-NEXT:     __retval = _v18;
// LOWERING-NEXT:     let _v19: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v19 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn cmp_bytes(arg0: &[u8], arg2: &[u8]) -> i32 {
// REWRITES-NEXT: let mut a: *mut u8 = arg0.as_ptr() as *mut u8;
// REWRITES-NEXT: let mut alen: i32 = arg0.len() as i32;
// REWRITES-NEXT: let mut b: *mut u8 = arg2.as_ptr() as *mut u8;
// REWRITES-NEXT: let mut blen: i32 = arg2.len() as i32;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut sa: i32 = 0;
// REWRITES-NEXT: let mut sb: i32 = 0;
// REWRITES-NEXT: let mut order: i32 = 0;
// REWRITES-NEXT: let mut sign: i32 = 0;
// REWRITES-NEXT: sa = 0;
// REWRITES-NEXT: sb = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i < alen) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let _v8: *mut u8 = a;
// REWRITES-NEXT:                     let _v9: *mut u8 = unsafe { _v8.offset((i as i64) as isize) };
// REWRITES-NEXT:                     sa = sa + ((unsafe { *_v9 }) as i32);
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i2: i32 = 0;
// REWRITES-NEXT:         i2 = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i2 < blen) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let _v22: *mut u8 = b;
// REWRITES-NEXT:                     let _v23: *mut u8 = unsafe { _v22.offset((i2 as i64) as isize) };
// REWRITES-NEXT:                     sb = sb + ((unsafe { *_v23 }) as i32);
// REWRITES-NEXT:                     i2 = i2 + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v34: u64 = 3;
// REWRITES-NEXT: order = unsafe { arg0[(0usize..(_v34 as usize) as usize)].cmp(&arg2[(0usize..(_v34 as usize) as usize)]) as i32 };
// REWRITES-NEXT: let _v37: i32 = 0;
// REWRITES-NEXT: let _v41: i32 = 0;
// REWRITES-NEXT: sign = ((order > _v37) as i32) - ((order < _v41) as i32);
// REWRITES-NEXT: let _v46: i32 = 1000;
// REWRITES-NEXT: __retval = sign * _v46 + (sa - sb);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: [u8; 3] = [0; 3];
// REWRITES-NEXT: let mut y: [u8; 3] = [0; 3];
// REWRITES-NEXT: let mut z: [u8; 3] = [0; 3];
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = [1, 2, 3];
// REWRITES-NEXT: y = [1, 2, 4];
// REWRITES-NEXT: z = [1, 2, 3];
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut u8 = x.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v3: i32 = 3;
// REWRITES-NEXT: let _v4: *mut u8 = y.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v5: i32 = 3;
// REWRITES-NEXT: let _v6: i32 = cmp_bytes(unsafe { std::slice::from_raw_parts(_v2 as *const u8, _v3 as usize) }, unsafe { std::slice::from_raw_parts(_v4 as *const u8, _v5 as usize) });
// REWRITES-NEXT: let _v7: *mut u8 = y.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v8: i32 = 3;
// REWRITES-NEXT: let _v9: *mut u8 = x.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v10: i32 = 3;
// REWRITES-NEXT: let _v11: i32 = cmp_bytes(unsafe { std::slice::from_raw_parts(_v7 as *const u8, _v8 as usize) }, unsafe { std::slice::from_raw_parts(_v9 as *const u8, _v10 as usize) });
// REWRITES-NEXT: let _v12: *mut u8 = x.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v13: i32 = 3;
// REWRITES-NEXT: let _v14: *mut u8 = z.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v15: i32 = 3;
// REWRITES-NEXT: let _v16: i32 = cmp_bytes(unsafe { std::slice::from_raw_parts(_v12 as *const u8, _v13 as usize) }, unsafe { std::slice::from_raw_parts(_v14 as *const u8, _v15 as usize) });
// REWRITES-NEXT: let _v17: i32 = unsafe { printf(_v1 as *const i8, _v6, _v11, _v16) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
