
#include <stdio.h>

static int cached_value = 0;
static int computed     = 0;

static int compute(void) { return 42; }

int get_value(void) {
  if (!computed) {
    cached_value = compute();
    computed     = 1;
  }
  return cached_value;
}

int main(void) {
  printf("%d\n", get_value());
  printf("%d\n", get_value());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut cached_value: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut computed: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn compute() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 42;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn get_value() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v0: i32 = unsafe { computed };
// LOWERING-NEXT:         let _v1: bool = _v0 != 0;
// LOWERING-NEXT:         let _v2: bool = !_v1;
// LOWERING-NEXT:         if _v2 {
// LOWERING-NEXT:             let _v3: i32 = compute();
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 cached_value = _v3;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v4: i32 = 1;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 computed = _v4;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: i32 = unsafe { cached_value };
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i32 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = get_value();
// LOWERING-NEXT:     let _v3: i32 = unsafe { printf(_v1 as *const i8, _v2) };
// LOWERING-NEXT:     let _v4: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i32 = get_value();
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v4 as *const i8, _v5) };
// LOWERING-NEXT:     let _v7: i32 = 0;
// LOWERING-NEXT:     __retval = _v7;
// LOWERING-NEXT:     let _v8: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v8 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut cached_value: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut computed: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn compute() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 42;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn get_value() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v1: bool = (unsafe { computed }) != 0;
// REWRITES-NEXT:         let _v2: bool = !_v1;
// REWRITES-NEXT:         if _v2 {
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     cached_value = compute();
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     computed = 1;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = unsafe { cached_value };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = get_value();
// REWRITES-NEXT: let _v3: i32 = unsafe { printf(_v1 as *const i8, _v2) };
// REWRITES-NEXT: let _v4: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = get_value();
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v4 as *const i8, _v5) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
