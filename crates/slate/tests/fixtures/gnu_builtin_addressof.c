#include <stdio.h>

static int address_probe(void) {
  int  value   = 37;
  int *address = __builtin_addressof(value);
  return (address == &value) + (*address == 37);
}

int main(void) {
  printf("%d\n", address_probe());
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
// LOWERING-NEXT: fn address_probe() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut address: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 37;
// LOWERING-NEXT:     value = _v0;
// LOWERING-NEXT:     address = std::ptr::addr_of_mut!(value);
// LOWERING-NEXT:     let _v1: *mut i32 = address;
// LOWERING-NEXT:     let _v2: bool = _v1 == std::ptr::addr_of_mut!(value);
// LOWERING-NEXT:     let _v3: i32 = _v2 as i32;
// LOWERING-NEXT:     let _v4: *mut i32 = address;
// LOWERING-NEXT:     let _v5: i32 = unsafe { *_v4 };
// LOWERING-NEXT:     let _v6: i32 = 37;
// LOWERING-NEXT:     let _v7: bool = _v5 == _v6;
// LOWERING-NEXT:     let _v8: i32 = _v7 as i32;
// LOWERING-NEXT:     let _v9: i32 = _v3 + _v8;
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     return _v10;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = address_probe();
// LOWERING-NEXT:     let _v3: i32 = unsafe { printf(_v1 as *const i8, _v2) };
// LOWERING-NEXT:     let _v4: i32 = 0;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v5 as i32);
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
// REWRITES-NEXT: fn address_probe() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut value: i32 = 0;
// REWRITES-NEXT: let mut address: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: value = 37;
// REWRITES-NEXT: address = std::ptr::addr_of_mut!(value);
// REWRITES-NEXT: let _v2: bool = address == std::ptr::addr_of_mut!(value);
// REWRITES-NEXT: let _v6: i32 = 37;
// REWRITES-NEXT: __retval = (_v2 as i32) + (((unsafe { *address }) == _v6) as i32);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = address_probe();
// REWRITES-NEXT: let _v3: i32 = unsafe { printf(_v1 as *const i8, _v2) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
