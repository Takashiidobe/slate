#include <stdio.h>

int main(void) {
  int elifdef_value  = 0;
  int elifndef_value = 0;
  int inactive_value = 0;

#if 0
  elifdef_value = 1;
#elifdef __STDC__
  elifdef_value = 23;
#elifndef __STDC__
  elifdef_value = 2;
#else
  elifdef_value = 3;
#endif

#if 0
  elifndef_value = 1;
#elifdef C23_MISSING
  elifndef_value = 2;
#elifndef C23_MISSING
  elifndef_value = 29;
#else
  elifndef_value = 3;
#endif

#ifdef __STDC__
  inactive_value = 31;
#elifdef __STDC__
  inactive_value = 2;
#elifndef C23_MISSING
  inactive_value = 3;
#else
  inactive_value = 4;
#endif

  printf("%d %d %d\n", elifdef_value, elifndef_value, inactive_value);
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
// LOWERING-NEXT:     let mut elifdef_value: i32 = 0;
// LOWERING-NEXT:     let mut elifndef_value: i32 = 0;
// LOWERING-NEXT:     let mut inactive_value: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 0;
// LOWERING-NEXT:     elifdef_value = _v1;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     elifndef_value = _v2;
// LOWERING-NEXT:     let _v3: i32 = 0;
// LOWERING-NEXT:     inactive_value = _v3;
// LOWERING-NEXT:     let _v4: i32 = 23;
// LOWERING-NEXT:     elifdef_value = _v4;
// LOWERING-NEXT:     let _v5: i32 = 29;
// LOWERING-NEXT:     elifndef_value = _v5;
// LOWERING-NEXT:     let _v6: i32 = 31;
// LOWERING-NEXT:     inactive_value = _v6;
// LOWERING-NEXT:     let _v7: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v8: i32 = elifdef_value;
// LOWERING-NEXT:     let _v9: i32 = elifndef_value;
// LOWERING-NEXT:     let _v10: i32 = inactive_value;
// LOWERING-NEXT:     let _v11: i32 = unsafe { printf(_v7 as *const i8, _v8, _v9, _v10) };
// LOWERING-NEXT:     let _v12: i32 = 0;
// LOWERING-NEXT:     __retval = _v12;
// LOWERING-NEXT:     let _v13: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v13 as i32);
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
// REWRITES-NEXT: let mut elifdef_value: i32 = 0;
// REWRITES-NEXT: let mut elifndef_value: i32 = 0;
// REWRITES-NEXT: let mut inactive_value: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: elifdef_value = 0;
// REWRITES-NEXT: elifndef_value = 0;
// REWRITES-NEXT: inactive_value = 0;
// REWRITES-NEXT: elifdef_value = 23;
// REWRITES-NEXT: elifndef_value = 29;
// REWRITES-NEXT: inactive_value = 31;
// REWRITES-NEXT: let _v7: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: i32 = unsafe { printf(_v7 as *const i8, elifdef_value, elifndef_value, inactive_value) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
