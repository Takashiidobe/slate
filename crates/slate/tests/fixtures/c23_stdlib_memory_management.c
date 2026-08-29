#include <stdio.h>
#include <stdlib.h>

int main(void) {
  void *p = malloc(16);
  free_sized(p, 16);

  void *q = aligned_alloc(16, 32);
  free_aligned_sized(q, 16, 32);

  void *r = realloc(NULL, 0);
  free(r);

  void *a = aligned_alloc(64, 64);
  printf("%d\n", memalignment(a) % 64 == 0);
  free(a);

  printf("ok\n");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free_sized(_0: *mut core::ffi::c_void, _1: usize);
// LOWERING-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free_aligned_sized(_0: *mut core::ffi::c_void, _1: usize, _2: usize);
// LOWERING-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn memalignment(arg0: *mut core::ffi::c_void) -> u64 {
// LOWERING-NEXT:     let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: u64 = 0;
// LOWERING-NEXT:     let mut v: u64 = 0;
// LOWERING-NEXT:     let mut align: u64 = 0;
// LOWERING-NEXT:     p = arg0;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = p;
// LOWERING-NEXT:     let _v1: u64 = _v0 as u64;
// LOWERING-NEXT:     v = _v1;
// LOWERING-NEXT:     let _v2: u64 = 0;
// LOWERING-NEXT:     align = _v2;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v3: u64 = v;
// LOWERING-NEXT:             let _v4: u64 = 0;
// LOWERING-NEXT:             let _v5: bool = _v3 != _v4;
// LOWERING-NEXT:             let _v6: bool = if _v5 {
// LOWERING-NEXT:                 let _v7: u64 = v;
// LOWERING-NEXT:                 let _v8: u64 = 1;
// LOWERING-NEXT:                 let _v9: u64 = _v7 & _v8;
// LOWERING-NEXT:                 let _v10: u64 = 0;
// LOWERING-NEXT:                 let _v11: bool = _v9 == _v10;
// LOWERING-NEXT:                 _v11
// LOWERING-NEXT:             } else {
// LOWERING-NEXT:                 let _v12: bool = false;
// LOWERING-NEXT:                 _v12
// LOWERING-NEXT:             };
// LOWERING-NEXT:             if !_v6 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v13: i32 = 1;
// LOWERING-NEXT:                 let _v14: u64 = v;
// LOWERING-NEXT:                 let _v15: u64 = _v14 >> _v13;
// LOWERING-NEXT:                 v = _v15;
// LOWERING-NEXT:                 let _v16: u64 = align;
// LOWERING-NEXT:                 let _v17: u64 = _v16 + 1;
// LOWERING-NEXT:                 align = _v17;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v18: u64 = v;
// LOWERING-NEXT:     let _v19: u64 = 0;
// LOWERING-NEXT:     let _v20: bool = _v18 == _v19;
// LOWERING-NEXT:     let _v21: u64 = if _v20 {
// LOWERING-NEXT:         let _v22: u64 = 0;
// LOWERING-NEXT:         _v22
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v23: u64 = 1;
// LOWERING-NEXT:         let _v24: u64 = align;
// LOWERING-NEXT:         let _v25: u64 = _v23 << _v24;
// LOWERING-NEXT:         _v25
// LOWERING-NEXT:     };
// LOWERING-NEXT:     __retval = _v21;
// LOWERING-NEXT:     let _v26: u64 = __retval;
// LOWERING-NEXT:     return _v26;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut q: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut r: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut a: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u64 = 16;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = unsafe { malloc(_v1 as usize) };
// LOWERING-NEXT:     p = _v2;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = p;
// LOWERING-NEXT:     let _v4: u64 = 16;
// LOWERING-NEXT:     unsafe { free_sized(_v3 as *mut core::ffi::c_void, _v4 as usize) };
// LOWERING-NEXT:     let _v5: u64 = 16;
// LOWERING-NEXT:     let _v6: u64 = 32;
// LOWERING-NEXT:     let _v7: *mut core::ffi::c_void = unsafe { aligned_alloc(_v5 as usize, _v6 as usize) };
// LOWERING-NEXT:     q = _v7;
// LOWERING-NEXT:     let _v8: *mut core::ffi::c_void = q;
// LOWERING-NEXT:     let _v9: u64 = 16;
// LOWERING-NEXT:     let _v10: u64 = 32;
// LOWERING-NEXT:     unsafe { free_aligned_sized(_v8 as *mut core::ffi::c_void, _v9 as usize, _v10 as usize) };
// LOWERING-NEXT:     let _v11: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v12: u64 = 0;
// LOWERING-NEXT:     let _v13: *mut core::ffi::c_void = unsafe { realloc(_v11 as *mut core::ffi::c_void, _v12 as usize) };
// LOWERING-NEXT:     r = _v13;
// LOWERING-NEXT:     let _v14: *mut core::ffi::c_void = r;
// LOWERING-NEXT:     unsafe { free(_v14 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v15: u64 = 64;
// LOWERING-NEXT:     let _v16: u64 = 64;
// LOWERING-NEXT:     let _v17: *mut core::ffi::c_void = unsafe { aligned_alloc(_v15 as usize, _v16 as usize) };
// LOWERING-NEXT:     a = _v17;
// LOWERING-NEXT:     let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: *mut core::ffi::c_void = a;
// LOWERING-NEXT:     let _v20: u64 = memalignment(_v19 as *mut core::ffi::c_void);
// LOWERING-NEXT:     let _v21: u64 = 64;
// LOWERING-NEXT:     let _v22: u64 = _v20 % _v21;
// LOWERING-NEXT:     let _v23: u64 = 0;
// LOWERING-NEXT:     let _v24: bool = _v22 == _v23;
// LOWERING-NEXT:     let _v25: i32 = _v24 as i32;
// LOWERING-NEXT:     let _v26: i32 = unsafe { printf(_v18 as *const i8, _v25) };
// LOWERING-NEXT:     let _v27: *mut core::ffi::c_void = a;
// LOWERING-NEXT:     unsafe { free(_v27 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v28: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v29: i32 = unsafe { printf(_v28 as *const i8) };
// LOWERING-NEXT:     let _v30: i32 = 0;
// LOWERING-NEXT:     __retval = _v30;
// LOWERING-NEXT:     let _v31: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v31 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free_sized(_0: *mut core::ffi::c_void, _1: usize);
// REWRITES-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free_aligned_sized(_0: *mut core::ffi::c_void, _1: usize, _2: usize);
// REWRITES-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn memalignment(arg0: *mut core::ffi::c_void) -> u64 {
// REWRITES-NEXT: let mut p: *mut core::ffi::c_void = arg0;
// REWRITES-NEXT: let mut __retval: u64 = 0;
// REWRITES-NEXT: let mut v: u64 = 0;
// REWRITES-NEXT: let mut align: u64 = 0;
// REWRITES-NEXT: v = p as u64;
// REWRITES-NEXT: align = 0;
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let _v4: u64 = 0;
// REWRITES-NEXT:         let _v6: bool = if v != _v4 {
// REWRITES-NEXT:                     let _v8: u64 = 1;
// REWRITES-NEXT:                     let _v10: u64 = 0;
// REWRITES-NEXT:                     let _v11: bool = v & _v8 == _v10;
// REWRITES-NEXT:             _v11
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:                     let _v12: bool = false;
// REWRITES-NEXT:             _v12
// REWRITES-NEXT:         };
// REWRITES-NEXT:         if !_v6 {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let _v13: i32 = 1;
// REWRITES-NEXT:                     v = v >> _v13;
// REWRITES-NEXT:                     align = align + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v19: u64 = 0;
// REWRITES-NEXT: let _v21: u64 = if v == _v19 {
// REWRITES-NEXT:         let _v22: u64 = 0;
// REWRITES-NEXT:     _v22
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v23: u64 = 1;
// REWRITES-NEXT:         let _v25: u64 = _v23 << align;
// REWRITES-NEXT:     _v25
// REWRITES-NEXT: };
// REWRITES-NEXT: __retval = _v21;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut q: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut r: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut a: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u64 = 16;
// REWRITES-NEXT: p = unsafe { malloc(_v1 as usize) };
// REWRITES-NEXT: let _v4: u64 = 16;
// REWRITES-NEXT: unsafe { free_sized(p as *mut core::ffi::c_void, _v4 as usize) };
// REWRITES-NEXT: let _v5: u64 = 16;
// REWRITES-NEXT: let _v6: u64 = 32;
// REWRITES-NEXT: q = unsafe { aligned_alloc(_v5 as usize, _v6 as usize) };
// REWRITES-NEXT: let _v9: u64 = 16;
// REWRITES-NEXT: let _v10: u64 = 32;
// REWRITES-NEXT: unsafe { free_aligned_sized(q as *mut core::ffi::c_void, _v9 as usize, _v10 as usize) };
// REWRITES-NEXT: let _v11: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v12: u64 = 0;
// REWRITES-NEXT: r = unsafe { realloc(_v11 as *mut core::ffi::c_void, _v12 as usize) };
// REWRITES-NEXT: unsafe { free(r as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v15: u64 = 64;
// REWRITES-NEXT: let _v16: u64 = 64;
// REWRITES-NEXT: a = unsafe { aligned_alloc(_v15 as usize, _v16 as usize) };
// REWRITES-NEXT: let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: u64 = memalignment(a as *mut core::ffi::c_void);
// REWRITES-NEXT: let _v21: u64 = 64;
// REWRITES-NEXT: let _v23: u64 = 0;
// REWRITES-NEXT: let _v26: i32 = unsafe { printf(_v18 as *const i8, (_v20 % _v21 == _v23) as i32) };
// REWRITES-NEXT: unsafe { free(a as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v28: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v29: i32 = unsafe { printf(_v28 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
