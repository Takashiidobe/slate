#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((malloc, alloc_size(1), alloc_align(2))) static void *
allocate(size_t size, size_t alignment) {
  (void)alignment;
  return malloc(size);
}

__attribute__((malloc, alloc_size(1, 2))) static void *
allocate_array(size_t count, size_t size) {
  return calloc(count, size);
}

__attribute__((assume_aligned(16, 4))) static void *
offset_aligned(void *pointer) {
  return (char *)pointer + 4;
}

int main(void) {
  int *values = allocate(3 * sizeof(int), _Alignof(int));
  int *zeroed = allocate_array(2, sizeof(int));
  values[0]   = 7;
  values[1]   = 9;
  int *second = offset_aligned(values);
  printf("%d %d %d\n", values[0], *second, zeroed[1]);
  free(zeroed);
  free(values);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn calloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn allocate(arg3: u64, arg4: u64) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     let mut alignment: u64 = 0;
// LOWERING-NEXT:     let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     size = arg3;
// LOWERING-NEXT:     alignment = arg4;
// LOWERING-NEXT:     let _v0: u64 = alignment;
// LOWERING-NEXT:     let _v1: u64 = size;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = unsafe { malloc(_v1 as usize) };
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn allocate_array(arg1: u64, arg2: u64) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     count = arg1;
// LOWERING-NEXT:     size = arg2;
// LOWERING-NEXT:     let _v0: u64 = count;
// LOWERING-NEXT:     let _v1: u64 = size;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = unsafe { calloc(_v0 as usize, _v1 as usize) };
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn offset_aligned(arg0: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut pointer: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     pointer = arg0;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = pointer;
// LOWERING-NEXT:     let _v1: *mut i8 = _v0 as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 4;
// LOWERING-NEXT:     let _v3: *mut i8 = unsafe { _v1.add(4) };
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = _v3 as *mut core::ffi::c_void;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: *mut core::ffi::c_void = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut zeroed: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut second: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u64 = 3;
// LOWERING-NEXT:     let _v2: u64 = 4;
// LOWERING-NEXT:     let _v3: u64 = _v1 * _v2;
// LOWERING-NEXT:     let _v4: u64 = 4;
// LOWERING-NEXT:     let _v5: *mut core::ffi::c_void = allocate(_v3, _v4);
// LOWERING-NEXT:     let _v6: *mut i32 = _v5 as *mut i32;
// LOWERING-NEXT:     values = _v6;
// LOWERING-NEXT:     let _v7: u64 = 2;
// LOWERING-NEXT:     let _v8: u64 = 4;
// LOWERING-NEXT:     let _v9: *mut core::ffi::c_void = allocate_array(_v7, _v8);
// LOWERING-NEXT:     let _v10: *mut i32 = _v9 as *mut i32;
// LOWERING-NEXT:     zeroed = _v10;
// LOWERING-NEXT:     let _v11: i32 = 7;
// LOWERING-NEXT:     let _v12: i64 = 0;
// LOWERING-NEXT:     let _v13: *mut i32 = values;
// LOWERING-NEXT:     let _v14: *mut i32 = unsafe { _v13.add(0) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v14 = _v11;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v15: i32 = 9;
// LOWERING-NEXT:     let _v16: i64 = 1;
// LOWERING-NEXT:     let _v17: *mut i32 = values;
// LOWERING-NEXT:     let _v18: *mut i32 = unsafe { _v17.add(1) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v18 = _v15;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v19: *mut i32 = values;
// LOWERING-NEXT:     let _v20: *mut core::ffi::c_void = _v19 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v21: *mut core::ffi::c_void = offset_aligned(_v20 as *mut core::ffi::c_void);
// LOWERING-NEXT:     let _v22: *mut i32 = _v21 as *mut i32;
// LOWERING-NEXT:     second = _v22;
// LOWERING-NEXT:     let _v23: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v24: i64 = 0;
// LOWERING-NEXT:     let _v25: *mut i32 = values;
// LOWERING-NEXT:     let _v26: *mut i32 = unsafe { _v25.add(0) };
// LOWERING-NEXT:     let _v27: i32 = unsafe { *_v26 };
// LOWERING-NEXT:     let _v28: *mut i32 = second;
// LOWERING-NEXT:     let _v29: i32 = unsafe { *_v28 };
// LOWERING-NEXT:     let _v30: i64 = 1;
// LOWERING-NEXT:     let _v31: *mut i32 = zeroed;
// LOWERING-NEXT:     let _v32: *mut i32 = unsafe { _v31.add(1) };
// LOWERING-NEXT:     let _v33: i32 = unsafe { *_v32 };
// LOWERING-NEXT:     let _v34: i32 = unsafe { printf(_v23 as *const i8, _v27, _v29, _v33) };
// LOWERING-NEXT:     let _v35: *mut i32 = zeroed;
// LOWERING-NEXT:     let _v36: *mut core::ffi::c_void = _v35 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v36 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v37: *mut i32 = values;
// LOWERING-NEXT:     let _v38: *mut core::ffi::c_void = _v37 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v38 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v39: i32 = 0;
// LOWERING-NEXT:     __retval = _v39;
// LOWERING-NEXT:     let _v40: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v40 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn calloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn allocate(arg3: u64, arg4: u64) -> *mut core::ffi::c_void {
// REWRITES-NEXT: let mut size: u64 = arg3;
// REWRITES-NEXT: let mut alignment: u64 = arg4;
// REWRITES-NEXT: let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v0: u64 = alignment;
// REWRITES-NEXT: __retval = unsafe { malloc(size as usize) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn allocate_array(arg1: u64, arg2: u64) -> *mut core::ffi::c_void {
// REWRITES-NEXT: let mut count: u64 = arg1;
// REWRITES-NEXT: let mut size: u64 = arg2;
// REWRITES-NEXT: let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: __retval = unsafe { calloc(count as usize, size as usize) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn offset_aligned(arg0: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// REWRITES-NEXT: let mut pointer: *mut core::ffi::c_void = arg0;
// REWRITES-NEXT: let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v1: *mut i8 = pointer as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 4;
// REWRITES-NEXT: let _v3: *mut i8 = unsafe { _v1.add(4) };
// REWRITES-NEXT: __retval = _v3 as *mut core::ffi::c_void;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut values: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut zeroed: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut second: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u64 = 3;
// REWRITES-NEXT: let _v2: u64 = 4;
// REWRITES-NEXT: let _v4: u64 = 4;
// REWRITES-NEXT: let _v5: *mut core::ffi::c_void = allocate(_v1 * _v2, _v4);
// REWRITES-NEXT: values = _v5 as *mut i32;
// REWRITES-NEXT: let _v7: u64 = 2;
// REWRITES-NEXT: let _v8: u64 = 4;
// REWRITES-NEXT: let _v9: *mut core::ffi::c_void = allocate_array(_v7, _v8);
// REWRITES-NEXT: zeroed = _v9 as *mut i32;
// REWRITES-NEXT: let _v11: i32 = 7;
// REWRITES-NEXT: let _v12: i64 = 0;
// REWRITES-NEXT: let _v13: *mut i32 = values;
// REWRITES-NEXT: let _v14: *mut i32 = unsafe { _v13.add(0) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v14 = _v11;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v15: i32 = 9;
// REWRITES-NEXT: let _v16: i64 = 1;
// REWRITES-NEXT: let _v17: *mut i32 = values;
// REWRITES-NEXT: let _v18: *mut i32 = unsafe { _v17.add(1) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v18 = _v15;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v21: *mut core::ffi::c_void = offset_aligned((values as *mut core::ffi::c_void) as *mut core::ffi::c_void);
// REWRITES-NEXT: second = _v21 as *mut i32;
// REWRITES-NEXT: let _v23: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v24: i64 = 0;
// REWRITES-NEXT: let _v25: *mut i32 = values;
// REWRITES-NEXT: let _v26: *mut i32 = unsafe { _v25.add(0) };
// REWRITES-NEXT: let _v27: i32 = unsafe { *_v26 };
// REWRITES-NEXT: let _v29: i32 = unsafe { *second };
// REWRITES-NEXT: let _v30: i64 = 1;
// REWRITES-NEXT: let _v31: *mut i32 = zeroed;
// REWRITES-NEXT: let _v32: *mut i32 = unsafe { _v31.add(1) };
// REWRITES-NEXT: let _v34: i32 = unsafe { printf(_v23 as *const i8, _v27, _v29, unsafe { *_v32 }) };
// REWRITES-NEXT: unsafe { free((zeroed as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: unsafe { free((values as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
