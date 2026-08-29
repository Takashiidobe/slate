#include <stdio.h>

char *bom_original = "Hi";

int main(void) {
  printf("%c %s\n", *bom_original, bom_original);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut bom_original: *mut i8 = b"Hi\0".as_ptr() as *mut i8;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%c %s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i8 = unsafe { bom_original };
// LOWERING-NEXT:     let _v3: i8 = unsafe { *_v2 };
// LOWERING-NEXT:     let _v4: i32 = _v3 as i32;
// LOWERING-NEXT:     let _v5: *mut i8 = unsafe { bom_original };
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v1 as *const i8, _v4, _v5) };
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
// REWRITES-NEXT: static mut bom_original: *mut i8 = b"Hi\0".as_ptr() as *mut i8;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%c %s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v1 as *const i8, (unsafe { *unsafe { bom_original } }) as i32, unsafe { bom_original }) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
