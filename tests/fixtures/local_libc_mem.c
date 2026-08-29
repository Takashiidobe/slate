#include <stdio.h>
#include <string.h>

int main(void) {
  char dst[16];
  char src[8] = "hello";
  memcpy(dst, src, 6);
  memset(dst + 5, 'A', 3);
  dst[8] = 0;
  char moved[16];
  memmove(moved, dst, 9);
  printf("%s\n", moved);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memmove(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut dst: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut src: [i8; 8] = [0; 8];
// LOWERING-NEXT:     let mut moved: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     src = [104, 101, 108, 108, 111, 0, 0, 0];
// LOWERING-NEXT:     let _v1: *mut i8 = dst.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v3: *mut i8 = src.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = _v3 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v5: u64 = 6;
// LOWERING-NEXT:     let _v6: *mut core::ffi::c_void = unsafe { memcpy(_v2 as *mut core::ffi::c_void, _v4 as *const core::ffi::c_void, _v5 as usize) };
// LOWERING-NEXT:     let _v7: *mut i8 = dst.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v8: i32 = 5;
// LOWERING-NEXT:     let _v9: *mut i8 = unsafe { _v7.add(5) };
// LOWERING-NEXT:     let _v10: *mut core::ffi::c_void = _v9 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v11: i32 = 65;
// LOWERING-NEXT:     let _v12: u64 = 3;
// LOWERING-NEXT:     let _v13: *mut core::ffi::c_void = unsafe { memset(_v10 as *mut core::ffi::c_void, _v11 as i32, _v12 as usize) };
// LOWERING-NEXT:     let _v14: i8 = 0;
// LOWERING-NEXT:     let _v15: i64 = 8;
// LOWERING-NEXT:     dst[(_v15 as usize)] = _v14;
// LOWERING-NEXT:     let _v16: *mut i8 = moved.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: *mut core::ffi::c_void = _v16 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v18: *mut i8 = dst.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: *mut core::ffi::c_void = _v18 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v20: u64 = 9;
// LOWERING-NEXT:     let _v21: *mut core::ffi::c_void = unsafe { memmove(_v17 as *mut core::ffi::c_void, _v19 as *const core::ffi::c_void, _v20 as usize) };
// LOWERING-NEXT:     let _v22: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v23: *mut i8 = moved.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v24: i32 = unsafe { printf(_v22 as *const i8, _v23) };
// LOWERING-NEXT:     let _v25: i32 = 0;
// LOWERING-NEXT:     __retval = _v25;
// LOWERING-NEXT:     let _v26: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v26 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memmove(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut dst: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut src: [i8; 8] = [0; 8];
// REWRITES-NEXT: let mut moved: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: src = [104, 101, 108, 108, 111, 0, 0, 0];
// REWRITES-NEXT: let _v1: *mut i8 = dst.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v3: *mut i8 = src.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: u64 = 6;
// REWRITES-NEXT: unsafe { std::ptr::copy_nonoverlapping(((_v3 as *mut core::ffi::c_void) as *const core::ffi::c_void) as *const u8, (_v2 as *mut core::ffi::c_void) as *mut u8, (_v5 as usize) as usize) };
// REWRITES-NEXT: let _v7: *mut i8 = dst.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = 5;
// REWRITES-NEXT: let _v9: *mut i8 = unsafe { _v7.add(5) };
// REWRITES-NEXT: let _v11: i32 = 65;
// REWRITES-NEXT: let _v12: u64 = 3;
// REWRITES-NEXT: unsafe { std::ptr::write_bytes(((_v9 as *mut core::ffi::c_void) as *mut core::ffi::c_void) as *mut u8, (_v11 as i32) as u8, (_v12 as usize) as usize) };
// REWRITES-NEXT: let _v15: i64 = 8;
// REWRITES-NEXT: dst[(_v15 as usize)] = 0;
// REWRITES-NEXT: let _v16: *mut i8 = moved.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: *mut core::ffi::c_void = _v16 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v18: *mut i8 = dst.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: u64 = 9;
// REWRITES-NEXT: unsafe { std::ptr::copy(((_v18 as *mut core::ffi::c_void) as *const core::ffi::c_void) as *const u8, (_v17 as *mut core::ffi::c_void) as *mut u8, (_v20 as usize) as usize) };
// REWRITES-NEXT: let _v22: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: *mut i8 = moved.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v24: i32 = unsafe { printf(_v22 as *const i8, _v23) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
