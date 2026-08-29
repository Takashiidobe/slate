#include <stdint.h>

static int frame_address_is_plausible(void) {
  void *fp = __builtin_frame_address(0);
  return (uintptr_t)fp > 0x1000;
}

int main(void) { return frame_address_is_plausible() ? 0 : 1; }
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: fn frame_address_is_plausible() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut fp: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: u32 = 0;
// LOWERING-NEXT:     let mut _v1: u8 = 0u8;
// LOWERING-NEXT:     let _v2: *mut u8 = std::ptr::addr_of_mut!(_v1) as *mut u8;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = _v2 as *mut core::ffi::c_void;
// LOWERING-NEXT:     fp = _v3;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = fp;
// LOWERING-NEXT:     let _v5: u64 = _v4 as u64;
// LOWERING-NEXT:     let _v6: u64 = 4096;
// LOWERING-NEXT:     let _v7: bool = _v5 > _v6;
// LOWERING-NEXT:     let _v8: i32 = _v7 as i32;
// LOWERING-NEXT:     __retval = _v8;
// LOWERING-NEXT:     let _v9: i32 = __retval;
// LOWERING-NEXT:     return _v9;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = frame_address_is_plausible();
// LOWERING-NEXT:     let _v2: bool = _v1 != 0;
// LOWERING-NEXT:     let _v3: i32 = 0;
// LOWERING-NEXT:     let _v4: i32 = 1;
// LOWERING-NEXT:     let _v5: i32 = if _v2 { _v3 } else { _v4 };
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v6 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: fn frame_address_is_plausible() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut fp: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v0: u32 = 0;
// REWRITES-NEXT: let mut _v1: u8 = 0u8;
// REWRITES-NEXT: let _v2: *mut u8 = std::ptr::addr_of_mut!(_v1) as *mut u8;
// REWRITES-NEXT: fp = _v2 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v6: u64 = 4096;
// REWRITES-NEXT: __retval = ((fp as u64) > _v6) as i32;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: i32 = frame_address_is_plausible();
// REWRITES-NEXT: let _v2: bool = _v1 != 0;
// REWRITES-NEXT: let _v3: i32 = 0;
// REWRITES-NEXT: let _v4: i32 = 1;
// REWRITES-NEXT: __retval = if _v2 { _v3 } else { _v4 };
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
