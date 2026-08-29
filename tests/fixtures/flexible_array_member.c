#include <stddef.h>
#include <stdlib.h>

struct FlexibleArray {
  size_t count;
  int    values[];
};

int main(void) {
  if (sizeof(struct FlexibleArray) != sizeof(size_t)) {
    return 1;
  }

  struct FlexibleArray *flexible =
      malloc(sizeof(*flexible) + 3 * sizeof(flexible->values[0]));
  if (flexible == NULL) {
    return 2;
  }

  flexible->count = 3;
  for (size_t index = 0; index < flexible->count; ++index) {
    flexible->values[index] = (int)index + 1;
  }

  int total = 0;
  for (size_t index = 0; index < flexible->count; ++index) {
    total += flexible->values[index];
  }

  free(flexible);
  return total == 6 ? 0 : 3;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct FlexibleArray {
// LOWERING-NEXT:     count: u64,
// LOWERING-NEXT:     values: [i32; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut flexible: *mut FlexibleArray = std::ptr::null_mut();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v1: u64 = std::mem::size_of::<FlexibleArray>() as u64;
// LOWERING-NEXT:         let _v2: u64 = 8;
// LOWERING-NEXT:         let _v3: bool = _v1 != _v2;
// LOWERING-NEXT:         if _v3 {
// LOWERING-NEXT:             let _v4: i32 = 1;
// LOWERING-NEXT:             __retval = _v4;
// LOWERING-NEXT:             let _v5: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v5 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v6: u64 = 8;
// LOWERING-NEXT:     let _v7: u64 = 3;
// LOWERING-NEXT:     let _v8: u64 = 4;
// LOWERING-NEXT:     let _v9: u64 = _v7 * _v8;
// LOWERING-NEXT:     let _v10: u64 = _v6 + _v9;
// LOWERING-NEXT:     let _v11: *mut core::ffi::c_void = unsafe { malloc(_v10 as usize) };
// LOWERING-NEXT:     let _v12: *mut FlexibleArray = _v11 as *mut FlexibleArray;
// LOWERING-NEXT:     flexible = _v12;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v13: *mut FlexibleArray = flexible;
// LOWERING-NEXT:         let _v14: *mut FlexibleArray = std::ptr::null_mut();
// LOWERING-NEXT:         let _v15: bool = _v13 == _v14;
// LOWERING-NEXT:         if _v15 {
// LOWERING-NEXT:             let _v16: i32 = 2;
// LOWERING-NEXT:             __retval = _v16;
// LOWERING-NEXT:             let _v17: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v17 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v18: u64 = 3;
// LOWERING-NEXT:     let _v19: *mut FlexibleArray = flexible;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v19).count = _v18;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: u64 = 0;
// LOWERING-NEXT:         let _v20: u64 = 0;
// LOWERING-NEXT:         index = _v20;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v21: u64 = index;
// LOWERING-NEXT:             let _v22: *mut FlexibleArray = flexible;
// LOWERING-NEXT:             let _v23: u64 = unsafe { (*_v22).count };
// LOWERING-NEXT:             let _v24: bool = _v21 < _v23;
// LOWERING-NEXT:             if !_v24 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v25: u64 = index;
// LOWERING-NEXT:                 let _v26: i32 = _v25 as i32;
// LOWERING-NEXT:                 let _v27: i32 = 1;
// LOWERING-NEXT:                 let _v28: i32 = _v26 + _v27;
// LOWERING-NEXT:                 let _v29: u64 = index;
// LOWERING-NEXT:                 let _v30: *mut FlexibleArray = flexible;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *(*_v30).values.as_mut_ptr().add(_v29 as usize) = _v28;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v31: u64 = index;
// LOWERING-NEXT:             let _v32: u64 = _v31 + 1;
// LOWERING-NEXT:             index = _v32;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v33: i32 = 0;
// LOWERING-NEXT:     total = _v33;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index2: u64 = 0;
// LOWERING-NEXT:         let _v34: u64 = 0;
// LOWERING-NEXT:         index2 = _v34;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v35: u64 = index2;
// LOWERING-NEXT:             let _v36: *mut FlexibleArray = flexible;
// LOWERING-NEXT:             let _v37: u64 = unsafe { (*_v36).count };
// LOWERING-NEXT:             let _v38: bool = _v35 < _v37;
// LOWERING-NEXT:             if !_v38 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v39: u64 = index2;
// LOWERING-NEXT:                 let _v40: *mut FlexibleArray = flexible;
// LOWERING-NEXT:                 let _v41: i32 = unsafe { *(*_v40).values.as_mut_ptr().add(_v39 as usize) };
// LOWERING-NEXT:                 let _v42: i32 = total;
// LOWERING-NEXT:                 let _v43: i32 = _v42 + _v41;
// LOWERING-NEXT:                 total = _v43;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v44: u64 = index2;
// LOWERING-NEXT:             let _v45: u64 = _v44 + 1;
// LOWERING-NEXT:             index2 = _v45;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v46: *mut FlexibleArray = flexible;
// LOWERING-NEXT:     let _v47: *mut core::ffi::c_void = _v46 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v47 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v48: i32 = total;
// LOWERING-NEXT:     let _v49: i32 = 6;
// LOWERING-NEXT:     let _v50: bool = _v48 == _v49;
// LOWERING-NEXT:     let _v51: i32 = 0;
// LOWERING-NEXT:     let _v52: i32 = 3;
// LOWERING-NEXT:     let _v53: i32 = if _v50 { _v51 } else { _v52 };
// LOWERING-NEXT:     __retval = _v53;
// LOWERING-NEXT:     let _v54: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v54 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct FlexibleArray {
// REWRITES-NEXT:     count: u64,
// REWRITES-NEXT:     values: [i32; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut flexible: *mut FlexibleArray = std::ptr::null_mut();
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v1: u64 = std::mem::size_of::<FlexibleArray>() as u64;
// REWRITES-NEXT:         let _v2: u64 = 8;
// REWRITES-NEXT:         let _v3: bool = _v1 != _v2;
// REWRITES-NEXT:         if _v3 {
// REWRITES-NEXT:                     __retval = 1;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v6: u64 = 8;
// REWRITES-NEXT: let _v7: u64 = 3;
// REWRITES-NEXT: let _v8: u64 = 4;
// REWRITES-NEXT: let _v11: *mut core::ffi::c_void = unsafe { malloc((_v6 + _v7 * _v8) as usize) };
// REWRITES-NEXT: flexible = _v11 as *mut FlexibleArray;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v14: *mut FlexibleArray = std::ptr::null_mut();
// REWRITES-NEXT:         let _v15: bool = flexible == _v14;
// REWRITES-NEXT:         if _v15 {
// REWRITES-NEXT:                     __retval = 2;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*flexible).count = 3;
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut index: u64 = 0;
// REWRITES-NEXT:         index = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(index < unsafe { (*flexible).count }) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v27: i32 = 1;
// REWRITES-NEXT:                                     let _v28: i32 = (index as i32) + _v27;
// REWRITES-NEXT:                                     let _v29: u64 = index;
// REWRITES-NEXT:                                     let _v30: *mut FlexibleArray = flexible;
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         *(*_v30).values.as_mut_ptr().add(_v29 as usize) = _v28;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     index = index + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut index2: u64 = 0;
// REWRITES-NEXT:         index2 = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(index2 < unsafe { (*flexible).count }) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v39: u64 = index2;
// REWRITES-NEXT:                                     let _v40: *mut FlexibleArray = flexible;
// REWRITES-NEXT:                                     let _v41: i32 = unsafe { *(*_v40).values.as_mut_ptr().add(_v39 as usize) };
// REWRITES-NEXT:                                     total = total + _v41;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     index2 = index2 + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe { free((flexible as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v49: i32 = 6;
// REWRITES-NEXT: let _v51: i32 = 0;
// REWRITES-NEXT: let _v52: i32 = 3;
// REWRITES-NEXT: __retval = if total == _v49 { _v51 } else { _v52 };
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
