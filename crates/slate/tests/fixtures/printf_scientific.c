#include <stdio.h>

int main(void) {
  double pos   = 1234.5678;
  double neg   = -1234.5678;
  double zero  = 0.0;
  double big   = 1e300;
  double small = 1e-300;
  printf("%e %.2e %10.2e %+e\n", pos, pos, pos, pos);
  printf("%E %.2E\n", pos, pos);
  printf("%-10.2e|\n", pos);
  printf("%e %+e\n", neg, neg);
  printf("%.0e\n", pos);
  printf("%e\n", zero);
  printf("%e %E\n", big, small);
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
// LOWERING-NEXT:     let mut pos: f64 = 0.0;
// LOWERING-NEXT:     let mut neg: f64 = 0.0;
// LOWERING-NEXT:     let mut zero: f64 = 0.0;
// LOWERING-NEXT:     let mut big: f64 = 0.0;
// LOWERING-NEXT:     let mut small: f64 = 0.0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f64 = 1234.5678;
// LOWERING-NEXT:     pos = _v1;
// LOWERING-NEXT:     let _v2: f64 = -1234.5678;
// LOWERING-NEXT:     neg = _v2;
// LOWERING-NEXT:     let _v3: f64 = 0.0;
// LOWERING-NEXT:     zero = _v3;
// LOWERING-NEXT:     let _v4: f64 = 1000000000000000052504760255204420248704468581108159154915854115511802457988908195786371375080447864043704443832883878176942523235360430575644792184786706982848387200926575803737830233794788090059368953234970799945081119038967640880074652742780142494579258788820056842838115669472196386865459400540160.0;
// LOWERING-NEXT:     big = _v4;
// LOWERING-NEXT:     let _v5: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001;
// LOWERING-NEXT:     small = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%e %.2e %10.2e %+e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: f64 = pos;
// LOWERING-NEXT:     let _v8: f64 = pos;
// LOWERING-NEXT:     let _v9: f64 = pos;
// LOWERING-NEXT:     let _v10: f64 = pos;
// LOWERING-NEXT:     let _v11: i32 = unsafe { printf(_v6 as *const i8, _v7, _v8, _v9, _v10) };
// LOWERING-NEXT:     let _v12: *mut i8 = b"%E %.2E\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v13: f64 = pos;
// LOWERING-NEXT:     let _v14: f64 = pos;
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v12 as *const i8, _v13, _v14) };
// LOWERING-NEXT:     let _v16: *mut i8 = b"%-10.2e|\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: f64 = pos;
// LOWERING-NEXT:     let _v18: i32 = unsafe { printf(_v16 as *const i8, _v17) };
// LOWERING-NEXT:     let _v19: *mut i8 = b"%e %+e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: f64 = neg;
// LOWERING-NEXT:     let _v21: f64 = neg;
// LOWERING-NEXT:     let _v22: i32 = unsafe { printf(_v19 as *const i8, _v20, _v21) };
// LOWERING-NEXT:     let _v23: *mut i8 = b"%.0e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v24: f64 = pos;
// LOWERING-NEXT:     let _v25: i32 = unsafe { printf(_v23 as *const i8, _v24) };
// LOWERING-NEXT:     let _v26: *mut i8 = b"%e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v27: f64 = zero;
// LOWERING-NEXT:     let _v28: i32 = unsafe { printf(_v26 as *const i8, _v27) };
// LOWERING-NEXT:     let _v29: *mut i8 = b"%e %E\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v30: f64 = big;
// LOWERING-NEXT:     let _v31: f64 = small;
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
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut pos: f64 = 0.0;
// REWRITES-NEXT: let mut neg: f64 = 0.0;
// REWRITES-NEXT: let mut zero: f64 = 0.0;
// REWRITES-NEXT: let mut big: f64 = 0.0;
// REWRITES-NEXT: let mut small: f64 = 0.0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: pos = 1234.5678;
// REWRITES-NEXT: neg = -1234.5678;
// REWRITES-NEXT: zero = 0.0;
// REWRITES-NEXT: big = 1000000000000000052504760255204420248704468581108159154915854115511802457988908195786371375080447864043704443832883878176942523235360430575644792184786706982848387200926575803737830233794788090059368953234970799945081119038967640880074652742780142494579258788820056842838115669472196386865459400540160.0;
// REWRITES-NEXT: small = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001;
// REWRITES-NEXT: let _v6: *mut i8 = b"%e %.2e %10.2e %+e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: i32 = unsafe { printf(_v6 as *const i8, pos, pos, pos, pos) };
// REWRITES-NEXT: let _v12: *mut i8 = b"%E %.2E\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v12 as *const i8, pos, pos) };
// REWRITES-NEXT: let _v16: *mut i8 = b"%-10.2e|\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: i32 = unsafe { printf(_v16 as *const i8, pos) };
// REWRITES-NEXT: let _v19: *mut i8 = b"%e %+e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: i32 = unsafe { printf(_v19 as *const i8, neg, neg) };
// REWRITES-NEXT: let _v23: *mut i8 = b"%.0e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v25: i32 = unsafe { printf(_v23 as *const i8, pos) };
// REWRITES-NEXT: let _v26: *mut i8 = b"%e\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v28: i32 = unsafe { printf(_v26 as *const i8, zero) };
// REWRITES-NEXT: let _v29: *mut i8 = b"%e %E\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v32: i32 = unsafe { printf(_v29 as *const i8, big, small) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
