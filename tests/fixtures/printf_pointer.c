#include <stdint.h>
#include <stdio.h>

int main(void) {
  void *stable = (void *)(uintptr_t)0x1234;
  printf("%p\n", stable);
  printf("addr=%p", (void *)(uintptr_t)0xABCD);
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
// LOWERING-NEXT:     let mut stable: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u64 = 4660;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// LOWERING-NEXT:     stable = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%p\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = stable;
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v3 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"addr=%p\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: u64 = 43981;
// LOWERING-NEXT:     let _v8: *mut core::ffi::c_void = _v7 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v6 as *const i8, _v8) };
// LOWERING-NEXT:     let _v10: i32 = 0;
// LOWERING-NEXT:     __retval = _v10;
// LOWERING-NEXT:     let _v11: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v11 as i32);
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
// REWRITES-NEXT: let mut stable: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u64 = 4660;
// REWRITES-NEXT: stable = _v1 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v3: *mut i8 = b"%p\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v3 as *const i8, stable) };
// REWRITES-NEXT: let _v6: *mut i8 = b"addr=%p\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: u64 = 43981;
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v6 as *const i8, _v7 as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
