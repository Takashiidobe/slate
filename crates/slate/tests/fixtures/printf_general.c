#include <stdio.h>

int main(void) {
  printf("%g %.3g %-10.3g|\n", 1234.5678, 1234.5678, 1234.5678);
  printf("%G %.3G\n", 1234.5678, 1234.5678);
  printf("%#g %#.3g\n", 1234.5678, 1234.5678);
  printf("%+.3g %+.3g\n", 1234.5678, -1234.5678);
  printf("%15.3g|%015.3g\n", 1234.5678, 1234.5678);
  printf("%g %g %g\n", 100.0, 0.0, -0.0);
  printf("%g\n", 1e300);
  printf("%g\n", 1e-300);
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
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%g %.3g %-10.3g|\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: f64 = 1234.5678;
// LOWERING-NEXT:     let _v3: f64 = 1234.5678;
// LOWERING-NEXT:     let _v4: f64 = 1234.5678;
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%G %.3G\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: f64 = 1234.5678;
// LOWERING-NEXT:     let _v8: f64 = 1234.5678;
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v6 as *const i8, _v7, _v8) };
// LOWERING-NEXT:     let _v10: *mut i8 = b"%#g %#.3g\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: f64 = 1234.5678;
// LOWERING-NEXT:     let _v12: f64 = 1234.5678;
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v10 as *const i8, _v11, _v12) };
// LOWERING-NEXT:     let _v14: *mut i8 = b"%+.3g %+.3g\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v15: f64 = 1234.5678;
// LOWERING-NEXT:     let _v16: f64 = -1234.5678;
// LOWERING-NEXT:     let _v17: i32 = unsafe { printf(_v14 as *const i8, _v15, _v16) };
// LOWERING-NEXT:     let _v18: *mut i8 = b"%15.3g|%015.3g\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: f64 = 1234.5678;
// LOWERING-NEXT:     let _v20: f64 = 1234.5678;
// LOWERING-NEXT:     let _v21: i32 = unsafe { printf(_v18 as *const i8, _v19, _v20) };
// LOWERING-NEXT:     let _v22: *mut i8 = b"%g %g %g\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v23: f64 = 100.0;
// LOWERING-NEXT:     let _v24: f64 = 0.0;
// LOWERING-NEXT:     let _v25: f64 = -0.0;
// LOWERING-NEXT:     let _v26: i32 = unsafe { printf(_v22 as *const i8, _v23, _v24, _v25) };
// LOWERING-NEXT:     let _v27: *mut i8 = b"%g\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v28: f64 = 1000000000000000052504760255204420248704468581108159154915854115511802457988908195786371375080447864043704443832883878176942523235360430575644792184786706982848387200926575803737830233794788090059368953234970799945081119038967640880074652742780142494579258788820056842838115669472196386865459400540160.0;
// LOWERING-NEXT:     let _v29: i32 = unsafe { printf(_v27 as *const i8, _v28) };
// LOWERING-NEXT:     let _v30: *mut i8 = b"%g\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v31: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001;
// LOWERING-NEXT:     let _v32: i32 = unsafe { printf(_v30 as *const i8, _v31) };
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
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%g %.3g %-10.3g|\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: f64 = 1234.5678;
// REWRITES-NEXT: let _v3: f64 = 1234.5678;
// REWRITES-NEXT: let _v4: f64 = 1234.5678;
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%G %.3G\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: f64 = 1234.5678;
// REWRITES-NEXT: let _v8: f64 = 1234.5678;
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v6 as *const i8, _v7, _v8) };
// REWRITES-NEXT: let _v10: *mut i8 = b"%#g %#.3g\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: f64 = 1234.5678;
// REWRITES-NEXT: let _v12: f64 = 1234.5678;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v10 as *const i8, _v11, _v12) };
// REWRITES-NEXT: let _v14: *mut i8 = b"%+.3g %+.3g\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: f64 = 1234.5678;
// REWRITES-NEXT: let _v16: f64 = -1234.5678;
// REWRITES-NEXT: let _v17: i32 = unsafe { printf(_v14 as *const i8, _v15, _v16) };
// REWRITES-NEXT: let _v18: *mut i8 = b"%15.3g|%015.3g\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: f64 = 1234.5678;
// REWRITES-NEXT: let _v20: f64 = 1234.5678;
// REWRITES-NEXT: let _v21: i32 = unsafe { printf(_v18 as *const i8, _v19, _v20) };
// REWRITES-NEXT: let _v22: *mut i8 = b"%g %g %g\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: f64 = 100.0;
// REWRITES-NEXT: let _v24: f64 = 0.0;
// REWRITES-NEXT: let _v25: f64 = -0.0;
// REWRITES-NEXT: let _v26: i32 = unsafe { printf(_v22 as *const i8, _v23, _v24, _v25) };
// REWRITES-NEXT: let _v27: *mut i8 = b"%g\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v28: f64 = 1000000000000000052504760255204420248704468581108159154915854115511802457988908195786371375080447864043704443832883878176942523235360430575644792184786706982848387200926575803737830233794788090059368953234970799945081119038967640880074652742780142494579258788820056842838115669472196386865459400540160.0;
// REWRITES-NEXT: let _v29: i32 = unsafe { printf(_v27 as *const i8, _v28) };
// REWRITES-NEXT: let _v30: *mut i8 = b"%g\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v31: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001;
// REWRITES-NEXT: let _v32: i32 = unsafe { printf(_v30 as *const i8, _v31) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
