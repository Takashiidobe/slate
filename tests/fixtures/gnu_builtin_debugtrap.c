#include <stdio.h>

static int volatile debugtrap_never;

int main(void) {
  if (debugtrap_never) {
    __builtin_debugtrap();
  }
  printf("ok\n");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(abi_unadjusted)]
// LOWERING-NEXT: #![feature(link_llvm_intrinsics)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut debugtrap_never: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v1: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(debugtrap_never)) };
// LOWERING-NEXT:         let _v2: bool = _v1 != 0;
// LOWERING-NEXT:         if _v2 {
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 unsafe { __slate_intrinsic_debugtrap_2b8d277b395439e1() };
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v3: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: i32 = unsafe { printf(_v3 as *const i8) };
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v6 as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "unadjusted" {
// LOWERING-NEXT:     #[link_name = "llvm.debugtrap"]
// LOWERING-NEXT:     fn __slate_intrinsic_debugtrap_2b8d277b395439e1();
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(abi_unadjusted)]
// REWRITES-NEXT: #![feature(link_llvm_intrinsics)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut debugtrap_never: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v1: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(debugtrap_never)) };
// REWRITES-NEXT:         let _v2: bool = _v1 != 0;
// REWRITES-NEXT:         if _v2 {
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     unsafe { __slate_intrinsic_debugtrap_2b8d277b395439e1() };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v3: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: i32 = unsafe { printf(_v3 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "unadjusted" {
// REWRITES-NEXT:     #[link_name = "llvm.debugtrap"]
// REWRITES-NEXT:     fn __slate_intrinsic_debugtrap_2b8d277b395439e1();
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
