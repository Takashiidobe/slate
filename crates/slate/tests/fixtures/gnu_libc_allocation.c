#define _GNU_SOURCE
#define obstack_chunk_alloc malloc
#define obstack_chunk_free  free
#include <malloc.h>
#include <obstack.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int gnu_allocation_extensions(void) {
  int  *values    = reallocarray(NULL, 4, sizeof(*values));
  void *aligned   = memalign(64, 80);
  void *page      = valloc(1);
  void *rounded   = pvalloc(1);
  long  page_size = sysconf(_SC_PAGESIZE);
  int   total     = 0;

  for (int index = 0; index < 4; ++index) {
    values[index] = index + 1;
  }
  total += values[0] + values[1] + values[2] + values[3];
  total += malloc_usable_size(values) >= 4 * sizeof(*values);
  total += aligned != NULL && (uintptr_t)aligned % 64 == 0;
  total += page != NULL && (uintptr_t)page % (uintptr_t)page_size == 0;
  total += rounded != NULL && (uintptr_t)rounded % (uintptr_t)page_size == 0;
  total += malloc_usable_size(rounded) >= (size_t)page_size;
  total += mallopt(M_CHECK_ACTION, 1) != 0;

  free(values);
  free(aligned);
  free(page);
  free(rounded);
  return total;
}

static int gnu_obstack_extensions(void) {
  struct obstack storage;
  char          *first;
  char          *second;
  int            total = 0;

  obstack_init(&storage);
  first   = obstack_copy0(&storage, "gnu", 3);
  second  = obstack_copy0(&storage, "libc", 4);
  total  += strcmp(first, "gnu") == 0;
  total  += strcmp(second, "libc") == 0;
  total  += obstack_object_size(&storage) == 0;
  obstack_free(&storage, NULL);
  return total;
}

int main(void) {
  printf("%d %d\n", gnu_allocation_extensions(), gnu_obstack_extensions());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: struct __SlateAllocaFrame0(i32, *mut i8, *mut i8);
// LOWERING-EMPTY:
// LOWERING-NEXT: mod __slate_bitfields {
// LOWERING-NEXT:     #[bitfields::bitfield(u8, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// LOWERING-NEXT:     pub struct __SlateBitfield_obstack_10 {
// LOWERING-NEXT:         #[bits(1)]
// LOWERING-NEXT:         pub _reserved_0: u128,
// LOWERING-NEXT:         #[bits(1)]
// LOWERING-NEXT:         pub maybe_empty_object: u32,
// LOWERING-NEXT:         #[bits(6)]
// LOWERING-NEXT:         pub _reserved_1: u128,
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct _obstack_chunk {
// LOWERING-NEXT:     limit: *mut i8,
// LOWERING-NEXT:     prev: *mut _obstack_chunk,
// LOWERING-NEXT:     contents: [i8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union _unnamed_at__home_takashi_Projects_slate_libc_shim_include_obstack_h_28_3_ {
// LOWERING-NEXT:     tempint: i64,
// LOWERING-NEXT:     tempptr: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union anon_0 {
// LOWERING-NEXT:     tempint: i64,
// LOWERING-NEXT:     tempptr: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct obstack {
// LOWERING-NEXT:     __bitfield_0: i64,
// LOWERING-NEXT:     __bitfield_1: *mut _obstack_chunk,
// LOWERING-NEXT:     __bitfield_2: *mut i8,
// LOWERING-NEXT:     __bitfield_3: *mut i8,
// LOWERING-NEXT:     __bitfield_4: *mut i8,
// LOWERING-NEXT:     __bitfield_5: anon_0,
// LOWERING-NEXT:     __bitfield_6: i32,
// LOWERING-NEXT:     __bitfield_7: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i64) -> *mut _obstack_chunk>,
// LOWERING-NEXT:     __bitfield_8: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut _obstack_chunk)>,
// LOWERING-NEXT:     __bitfield_9: *mut core::ffi::c_void,
// LOWERING-NEXT:     __bitfield_10: __slate_bitfields::__SlateBitfield_obstack_10,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn reallocarray(_0: *mut core::ffi::c_void, _1: usize, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memalign(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn valloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn pvalloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn sysconf(_0: i32) -> i64;
// LOWERING-NEXT:     fn malloc_usable_size(_0: *mut core::ffi::c_void) -> usize;
// LOWERING-NEXT:     fn mallopt(_0: i32, _1: i32) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn _obstack_begin(_0: *mut obstack, _1: i32, _2: i32, _3: Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void>, _4: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>) -> i32;
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn _obstack_newchunk(_0: *mut obstack, _1: i32);
// LOWERING-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// LOWERING-NEXT:     fn obstack_free(_0: *mut obstack, _1: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_allocation_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut aligned: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut page: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut rounded: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut page_size: i64 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v1: u64 = 4;
// LOWERING-NEXT:     let _v2: u64 = 4;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = unsafe { reallocarray(_v0 as *mut core::ffi::c_void, _v1 as usize, _v2 as usize) };
// LOWERING-NEXT:     let _v4: *mut i32 = _v3 as *mut i32;
// LOWERING-NEXT:     values = _v4;
// LOWERING-NEXT:     let _v5: u64 = 64;
// LOWERING-NEXT:     let _v6: u64 = 80;
// LOWERING-NEXT:     let _v7: *mut core::ffi::c_void = unsafe { memalign(_v5 as usize, _v6 as usize) };
// LOWERING-NEXT:     aligned = _v7;
// LOWERING-NEXT:     let _v8: u64 = 1;
// LOWERING-NEXT:     let _v9: *mut core::ffi::c_void = unsafe { valloc(_v8 as usize) };
// LOWERING-NEXT:     page = _v9;
// LOWERING-NEXT:     let _v10: u64 = 1;
// LOWERING-NEXT:     let _v11: *mut core::ffi::c_void = unsafe { pvalloc(_v10 as usize) };
// LOWERING-NEXT:     rounded = _v11;
// LOWERING-NEXT:     let _v12: i32 = 30;
// LOWERING-NEXT:     let _v13: i64 = unsafe { sysconf(_v12 as i32) };
// LOWERING-NEXT:     page_size = _v13;
// LOWERING-NEXT:     let _v14: i32 = 0;
// LOWERING-NEXT:     total = _v14;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: i32 = 0;
// LOWERING-NEXT:         let _v15: i32 = 0;
// LOWERING-NEXT:         index = _v15;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v16: i32 = index;
// LOWERING-NEXT:             let _v17: i32 = 4;
// LOWERING-NEXT:             let _v18: bool = _v16 < _v17;
// LOWERING-NEXT:             if !_v18 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v19: i32 = index;
// LOWERING-NEXT:                 let _v20: i32 = 1;
// LOWERING-NEXT:                 let _v21: i32 = _v19 + _v20;
// LOWERING-NEXT:                 let _v22: i32 = index;
// LOWERING-NEXT:                 let _v23: i64 = _v22 as i64;
// LOWERING-NEXT:                 let _v24: *mut i32 = values;
// LOWERING-NEXT:                 let _v25: *mut i32 = unsafe { _v24.offset(_v23 as isize) };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *_v25 = _v21;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v26: i32 = index;
// LOWERING-NEXT:             let _v27: i32 = _v26 + 1;
// LOWERING-NEXT:             index = _v27;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v28: i64 = 0;
// LOWERING-NEXT:     let _v29: *mut i32 = values;
// LOWERING-NEXT:     let _v30: *mut i32 = unsafe { _v29.add(0) };
// LOWERING-NEXT:     let _v31: i32 = unsafe { *_v30 };
// LOWERING-NEXT:     let _v32: i64 = 1;
// LOWERING-NEXT:     let _v33: *mut i32 = values;
// LOWERING-NEXT:     let _v34: *mut i32 = unsafe { _v33.add(1) };
// LOWERING-NEXT:     let _v35: i32 = unsafe { *_v34 };
// LOWERING-NEXT:     let _v36: i32 = _v31 + _v35;
// LOWERING-NEXT:     let _v37: i64 = 2;
// LOWERING-NEXT:     let _v38: *mut i32 = values;
// LOWERING-NEXT:     let _v39: *mut i32 = unsafe { _v38.add(2) };
// LOWERING-NEXT:     let _v40: i32 = unsafe { *_v39 };
// LOWERING-NEXT:     let _v41: i32 = _v36 + _v40;
// LOWERING-NEXT:     let _v42: i64 = 3;
// LOWERING-NEXT:     let _v43: *mut i32 = values;
// LOWERING-NEXT:     let _v44: *mut i32 = unsafe { _v43.add(3) };
// LOWERING-NEXT:     let _v45: i32 = unsafe { *_v44 };
// LOWERING-NEXT:     let _v46: i32 = _v41 + _v45;
// LOWERING-NEXT:     let _v47: i32 = total;
// LOWERING-NEXT:     let _v48: i32 = _v47 + _v46;
// LOWERING-NEXT:     total = _v48;
// LOWERING-NEXT:     let _v49: *mut i32 = values;
// LOWERING-NEXT:     let _v50: *mut core::ffi::c_void = _v49 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v51: u64 = (unsafe { malloc_usable_size(_v50 as *mut core::ffi::c_void) }) as u64;
// LOWERING-NEXT:     let _v52: u64 = 4;
// LOWERING-NEXT:     let _v53: u64 = 4;
// LOWERING-NEXT:     let _v54: u64 = _v52 * _v53;
// LOWERING-NEXT:     let _v55: bool = _v51 >= _v54;
// LOWERING-NEXT:     let _v56: i32 = _v55 as i32;
// LOWERING-NEXT:     let _v57: i32 = total;
// LOWERING-NEXT:     let _v58: i32 = _v57 + _v56;
// LOWERING-NEXT:     total = _v58;
// LOWERING-NEXT:     let _v59: *mut core::ffi::c_void = aligned;
// LOWERING-NEXT:     let _v60: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v61: bool = _v59 != _v60;
// LOWERING-NEXT:     let _v62: bool = if _v61 {
// LOWERING-NEXT:         let _v63: *mut core::ffi::c_void = aligned;
// LOWERING-NEXT:         let _v64: u64 = _v63 as u64;
// LOWERING-NEXT:         let _v65: u64 = 64;
// LOWERING-NEXT:         let _v66: u64 = _v64 % _v65;
// LOWERING-NEXT:         let _v67: u64 = 0;
// LOWERING-NEXT:         let _v68: bool = _v66 == _v67;
// LOWERING-NEXT:         _v68
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v69: bool = false;
// LOWERING-NEXT:         _v69
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v70: i32 = _v62 as i32;
// LOWERING-NEXT:     let _v71: i32 = total;
// LOWERING-NEXT:     let _v72: i32 = _v71 + _v70;
// LOWERING-NEXT:     total = _v72;
// LOWERING-NEXT:     let _v73: *mut core::ffi::c_void = page;
// LOWERING-NEXT:     let _v74: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v75: bool = _v73 != _v74;
// LOWERING-NEXT:     let _v76: bool = if _v75 {
// LOWERING-NEXT:         let _v77: *mut core::ffi::c_void = page;
// LOWERING-NEXT:         let _v78: u64 = _v77 as u64;
// LOWERING-NEXT:         let _v79: i64 = page_size;
// LOWERING-NEXT:         let _v80: u64 = _v79 as u64;
// LOWERING-NEXT:         let _v81: u64 = _v78 % _v80;
// LOWERING-NEXT:         let _v82: u64 = 0;
// LOWERING-NEXT:         let _v83: bool = _v81 == _v82;
// LOWERING-NEXT:         _v83
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v84: bool = false;
// LOWERING-NEXT:         _v84
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v85: i32 = _v76 as i32;
// LOWERING-NEXT:     let _v86: i32 = total;
// LOWERING-NEXT:     let _v87: i32 = _v86 + _v85;
// LOWERING-NEXT:     total = _v87;
// LOWERING-NEXT:     let _v88: *mut core::ffi::c_void = rounded;
// LOWERING-NEXT:     let _v89: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v90: bool = _v88 != _v89;
// LOWERING-NEXT:     let _v91: bool = if _v90 {
// LOWERING-NEXT:         let _v92: *mut core::ffi::c_void = rounded;
// LOWERING-NEXT:         let _v93: u64 = _v92 as u64;
// LOWERING-NEXT:         let _v94: i64 = page_size;
// LOWERING-NEXT:         let _v95: u64 = _v94 as u64;
// LOWERING-NEXT:         let _v96: u64 = _v93 % _v95;
// LOWERING-NEXT:         let _v97: u64 = 0;
// LOWERING-NEXT:         let _v98: bool = _v96 == _v97;
// LOWERING-NEXT:         _v98
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v99: bool = false;
// LOWERING-NEXT:         _v99
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v100: i32 = _v91 as i32;
// LOWERING-NEXT:     let _v101: i32 = total;
// LOWERING-NEXT:     let _v102: i32 = _v101 + _v100;
// LOWERING-NEXT:     total = _v102;
// LOWERING-NEXT:     let _v103: *mut core::ffi::c_void = rounded;
// LOWERING-NEXT:     let _v104: u64 = (unsafe { malloc_usable_size(_v103 as *mut core::ffi::c_void) }) as u64;
// LOWERING-NEXT:     let _v105: i64 = page_size;
// LOWERING-NEXT:     let _v106: u64 = _v105 as u64;
// LOWERING-NEXT:     let _v107: bool = _v104 >= _v106;
// LOWERING-NEXT:     let _v108: i32 = _v107 as i32;
// LOWERING-NEXT:     let _v109: i32 = total;
// LOWERING-NEXT:     let _v110: i32 = _v109 + _v108;
// LOWERING-NEXT:     total = _v110;
// LOWERING-NEXT:     let _v111: i32 = -5;
// LOWERING-NEXT:     let _v112: i32 = 1;
// LOWERING-NEXT:     let _v113: i32 = unsafe { mallopt(_v111 as i32, _v112 as i32) };
// LOWERING-NEXT:     let _v114: i32 = 0;
// LOWERING-NEXT:     let _v115: bool = _v113 != _v114;
// LOWERING-NEXT:     let _v116: i32 = _v115 as i32;
// LOWERING-NEXT:     let _v117: i32 = total;
// LOWERING-NEXT:     let _v118: i32 = _v117 + _v116;
// LOWERING-NEXT:     total = _v118;
// LOWERING-NEXT:     let _v119: *mut i32 = values;
// LOWERING-NEXT:     let _v120: *mut core::ffi::c_void = _v119 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v120 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v121: *mut core::ffi::c_void = aligned;
// LOWERING-NEXT:     unsafe { free(_v121 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v122: *mut core::ffi::c_void = page;
// LOWERING-NEXT:     unsafe { free(_v122 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v123: *mut core::ffi::c_void = rounded;
// LOWERING-NEXT:     unsafe { free(_v123 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v124: i32 = total;
// LOWERING-NEXT:     __retval = _v124;
// LOWERING-NEXT:     let _v125: i32 = __retval;
// LOWERING-NEXT:     return _v125;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_obstack_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut storage: obstack = obstack { __bitfield_0: 0, __bitfield_1: std::ptr::null_mut(), __bitfield_2: std::ptr::null_mut(), __bitfield_3: std::ptr::null_mut(), __bitfield_4: std::ptr::null_mut(), __bitfield_5: anon_0 { tempint: 0 }, __bitfield_6: 0, __bitfield_7: None, __bitfield_8: None, __bitfield_9: std::ptr::null_mut(), __bitfield_10: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_obstack_10>(0) } };
// LOWERING-NEXT:     let mut __slate_alloca_frame0: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, std::ptr::null_mut(), std::ptr::null_mut());
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __slate_alloca_frame0.0 = _v0;
// LOWERING-NEXT:     let _v1: i32 = 0;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     let _v3: Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void> = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void>>(malloc as *const ()) };
// LOWERING-NEXT:     let _v4: i32 = unsafe { _obstack_begin(std::ptr::addr_of_mut!(storage) as *mut obstack, _v1 as i32, _v2 as i32, _v3, Some(free)) };
// LOWERING-NEXT:     let _v5: i64 = 3;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempint = _v5;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v6: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v7: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let _v8: *mut i8 = unsafe { _v6.offset(_v7 as isize) };
// LOWERING-NEXT:     let _v9: i32 = 1;
// LOWERING-NEXT:     let _v10: *mut i8 = unsafe { _v8.add(1) };
// LOWERING-NEXT:     let _v11: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:     let _v12: bool = _v10 > _v11;
// LOWERING-NEXT:     let _v13: i32 = if _v12 {
// LOWERING-NEXT:         let _v14: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:         let _v15: i64 = 1;
// LOWERING-NEXT:         let _v16: i64 = _v14 + _v15;
// LOWERING-NEXT:         let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:         unsafe { _obstack_newchunk(std::ptr::addr_of_mut!(storage) as *mut obstack, _v17 as i32) };
// LOWERING-NEXT:         let _v18: i32 = 0;
// LOWERING-NEXT:         _v18
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v19: i32 = 0;
// LOWERING-NEXT:         _v19
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v20: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v21: *mut core::ffi::c_void = _v20 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v22: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v23: *mut core::ffi::c_void = _v22 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v24: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let _v25: u64 = _v24 as u64;
// LOWERING-NEXT:     let _v26: *mut core::ffi::c_void = unsafe { memcpy(_v21 as *mut core::ffi::c_void, _v23 as *const core::ffi::c_void, _v25 as usize) };
// LOWERING-NEXT:     let _v27: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let _v28: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v29: *mut i8 = unsafe { _v28.offset(_v27 as isize) };
// LOWERING-NEXT:     storage.__bitfield_3 = _v29;
// LOWERING-NEXT:     let _v30: i8 = 0;
// LOWERING-NEXT:     let _v31: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v32: i32 = 1;
// LOWERING-NEXT:     let _v33: *mut i8 = unsafe { _v31.add(1) };
// LOWERING-NEXT:     storage.__bitfield_3 = _v33;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v31 = _v30;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v34: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v35: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let _v36: bool = _v34 == _v35;
// LOWERING-NEXT:     let _v37: i32 = if _v36 {
// LOWERING-NEXT:         let _v38: u32 = 1;
// LOWERING-NEXT:         let _v39: u32 = (_v38 as u32) << 31 >> 31;
// LOWERING-NEXT:         storage.__bitfield_10.set_maybe_empty_object((_v38 as u32) << 31 >> 31);
// LOWERING-NEXT:         let _v40: i32 = 0;
// LOWERING-NEXT:         _v40
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v41: i32 = 0;
// LOWERING-NEXT:         _v41
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v42: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let _v43: *mut core::ffi::c_void = _v42 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempptr = _v43;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v44: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v45: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v46: i64 = unsafe { _v44.offset_from(_v45) as i64 };
// LOWERING-NEXT:     let _v47: i32 = storage.__bitfield_6;
// LOWERING-NEXT:     let _v48: i64 = _v47 as i64;
// LOWERING-NEXT:     let _v49: i64 = _v46 + _v48;
// LOWERING-NEXT:     let _v50: i32 = storage.__bitfield_6;
// LOWERING-NEXT:     let _v51: i32 = !_v50;
// LOWERING-NEXT:     let _v52: i64 = _v51 as i64;
// LOWERING-NEXT:     let _v53: i64 = _v49 & _v52;
// LOWERING-NEXT:     let _v54: *mut i8 = _v53 as *mut i8;
// LOWERING-NEXT:     storage.__bitfield_3 = _v54;
// LOWERING-NEXT:     let _v55: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v56: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let _v57: *mut i8 = _v56 as *mut i8;
// LOWERING-NEXT:     let _v58: i64 = unsafe { _v55.offset_from(_v57) as i64 };
// LOWERING-NEXT:     let _v59: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:     let _v60: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let _v61: *mut i8 = _v60 as *mut i8;
// LOWERING-NEXT:     let _v62: i64 = unsafe { _v59.offset_from(_v61) as i64 };
// LOWERING-NEXT:     let _v63: bool = _v58 > _v62;
// LOWERING-NEXT:     let _v64: *mut i8 = if _v63 {
// LOWERING-NEXT:         let _v65: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:         storage.__bitfield_3 = _v65;
// LOWERING-NEXT:         _v65
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v66: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         _v66
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v67: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     storage.__bitfield_2 = _v67;
// LOWERING-NEXT:     let _v68: *mut core::ffi::c_void = unsafe { storage.__bitfield_5.tempptr };
// LOWERING-NEXT:     let _v69: *mut i8 = _v68 as *mut i8;
// LOWERING-NEXT:     __slate_alloca_frame0.2 = _v69;
// LOWERING-NEXT:     let _v70: i64 = 4;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempint = _v70;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v71: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v72: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let _v73: *mut i8 = unsafe { _v71.offset(_v72 as isize) };
// LOWERING-NEXT:     let _v74: i32 = 1;
// LOWERING-NEXT:     let _v75: *mut i8 = unsafe { _v73.add(1) };
// LOWERING-NEXT:     let _v76: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:     let _v77: bool = _v75 > _v76;
// LOWERING-NEXT:     let _v78: i32 = if _v77 {
// LOWERING-NEXT:         let _v79: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:         let _v80: i64 = 1;
// LOWERING-NEXT:         let _v81: i64 = _v79 + _v80;
// LOWERING-NEXT:         let _v82: i32 = _v81 as i32;
// LOWERING-NEXT:         unsafe { _obstack_newchunk(std::ptr::addr_of_mut!(storage) as *mut obstack, _v82 as i32) };
// LOWERING-NEXT:         let _v83: i32 = 0;
// LOWERING-NEXT:         _v83
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v84: i32 = 0;
// LOWERING-NEXT:         _v84
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v85: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v86: *mut core::ffi::c_void = _v85 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v87: *mut i8 = b"libc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v88: *mut core::ffi::c_void = _v87 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v89: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let _v90: u64 = _v89 as u64;
// LOWERING-NEXT:     let _v91: *mut core::ffi::c_void = unsafe { memcpy(_v86 as *mut core::ffi::c_void, _v88 as *const core::ffi::c_void, _v90 as usize) };
// LOWERING-NEXT:     let _v92: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let _v93: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v94: *mut i8 = unsafe { _v93.offset(_v92 as isize) };
// LOWERING-NEXT:     storage.__bitfield_3 = _v94;
// LOWERING-NEXT:     let _v95: i8 = 0;
// LOWERING-NEXT:     let _v96: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v97: i32 = 1;
// LOWERING-NEXT:     let _v98: *mut i8 = unsafe { _v96.add(1) };
// LOWERING-NEXT:     storage.__bitfield_3 = _v98;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v96 = _v95;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v99: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v100: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let _v101: bool = _v99 == _v100;
// LOWERING-NEXT:     let _v102: i32 = if _v101 {
// LOWERING-NEXT:         let _v103: u32 = 1;
// LOWERING-NEXT:         let _v104: u32 = (_v103 as u32) << 31 >> 31;
// LOWERING-NEXT:         storage.__bitfield_10.set_maybe_empty_object((_v103 as u32) << 31 >> 31);
// LOWERING-NEXT:         let _v105: i32 = 0;
// LOWERING-NEXT:         _v105
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v106: i32 = 0;
// LOWERING-NEXT:         _v106
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v107: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let _v108: *mut core::ffi::c_void = _v107 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempptr = _v108;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v109: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v110: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v111: i64 = unsafe { _v109.offset_from(_v110) as i64 };
// LOWERING-NEXT:     let _v112: i32 = storage.__bitfield_6;
// LOWERING-NEXT:     let _v113: i64 = _v112 as i64;
// LOWERING-NEXT:     let _v114: i64 = _v111 + _v113;
// LOWERING-NEXT:     let _v115: i32 = storage.__bitfield_6;
// LOWERING-NEXT:     let _v116: i32 = !_v115;
// LOWERING-NEXT:     let _v117: i64 = _v116 as i64;
// LOWERING-NEXT:     let _v118: i64 = _v114 & _v117;
// LOWERING-NEXT:     let _v119: *mut i8 = _v118 as *mut i8;
// LOWERING-NEXT:     storage.__bitfield_3 = _v119;
// LOWERING-NEXT:     let _v120: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v121: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let _v122: *mut i8 = _v121 as *mut i8;
// LOWERING-NEXT:     let _v123: i64 = unsafe { _v120.offset_from(_v122) as i64 };
// LOWERING-NEXT:     let _v124: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:     let _v125: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let _v126: *mut i8 = _v125 as *mut i8;
// LOWERING-NEXT:     let _v127: i64 = unsafe { _v124.offset_from(_v126) as i64 };
// LOWERING-NEXT:     let _v128: bool = _v123 > _v127;
// LOWERING-NEXT:     let _v129: *mut i8 = if _v128 {
// LOWERING-NEXT:         let _v130: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:         storage.__bitfield_3 = _v130;
// LOWERING-NEXT:         _v130
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v131: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         _v131
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v132: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     storage.__bitfield_2 = _v132;
// LOWERING-NEXT:     let _v133: *mut core::ffi::c_void = unsafe { storage.__bitfield_5.tempptr };
// LOWERING-NEXT:     let _v134: *mut i8 = _v133 as *mut i8;
// LOWERING-NEXT:     __slate_alloca_frame0.1 = _v134;
// LOWERING-NEXT:     let _v135: *mut i8 = __slate_alloca_frame0.2;
// LOWERING-NEXT:     let _v136: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v137: i32 = unsafe { strcmp(_v135 as *const i8, _v136 as *const i8) };
// LOWERING-NEXT:     let _v138: i32 = 0;
// LOWERING-NEXT:     let _v139: bool = _v137 == _v138;
// LOWERING-NEXT:     let _v140: i32 = _v139 as i32;
// LOWERING-NEXT:     let _v141: i32 = __slate_alloca_frame0.0;
// LOWERING-NEXT:     let _v142: i32 = _v141 + _v140;
// LOWERING-NEXT:     __slate_alloca_frame0.0 = _v142;
// LOWERING-NEXT:     let _v143: *mut i8 = __slate_alloca_frame0.1;
// LOWERING-NEXT:     let _v144: *mut i8 = b"libc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v145: i32 = unsafe { strcmp(_v143 as *const i8, _v144 as *const i8) };
// LOWERING-NEXT:     let _v146: i32 = 0;
// LOWERING-NEXT:     let _v147: bool = _v145 == _v146;
// LOWERING-NEXT:     let _v148: i32 = _v147 as i32;
// LOWERING-NEXT:     let _v149: i32 = __slate_alloca_frame0.0;
// LOWERING-NEXT:     let _v150: i32 = _v149 + _v148;
// LOWERING-NEXT:     __slate_alloca_frame0.0 = _v150;
// LOWERING-NEXT:     let _v151: *mut i8 = storage.__bitfield_3;
// LOWERING-NEXT:     let _v152: *mut i8 = storage.__bitfield_2;
// LOWERING-NEXT:     let _v153: i64 = unsafe { _v151.offset_from(_v152) as i64 };
// LOWERING-NEXT:     let _v154: u32 = _v153 as u32;
// LOWERING-NEXT:     let _v155: u32 = 0;
// LOWERING-NEXT:     let _v156: bool = _v154 == _v155;
// LOWERING-NEXT:     let _v157: i32 = _v156 as i32;
// LOWERING-NEXT:     let _v158: i32 = __slate_alloca_frame0.0;
// LOWERING-NEXT:     let _v159: i32 = _v158 + _v157;
// LOWERING-NEXT:     __slate_alloca_frame0.0 = _v159;
// LOWERING-NEXT:     let _v160: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v161: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:     let _v162: *mut i8 = _v161 as *mut i8;
// LOWERING-NEXT:     let _v163: i64 = unsafe { _v160.offset_from(_v162) as i64 };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         storage.__bitfield_5.tempint = _v163;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v164: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:     let _v165: i64 = 0;
// LOWERING-NEXT:     let _v166: bool = _v164 > _v165;
// LOWERING-NEXT:     let _v167: bool = if _v166 {
// LOWERING-NEXT:         let _v168: i64 = unsafe { storage.__bitfield_5.tempint };
// LOWERING-NEXT:         let _v169: *mut i8 = storage.__bitfield_4;
// LOWERING-NEXT:         let _v170: *mut _obstack_chunk = storage.__bitfield_1;
// LOWERING-NEXT:         let _v171: *mut i8 = _v170 as *mut i8;
// LOWERING-NEXT:         let _v172: i64 = unsafe { _v169.offset_from(_v171) as i64 };
// LOWERING-NEXT:         let _v173: bool = _v168 < _v172;
// LOWERING-NEXT:         _v173
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v174: bool = false;
// LOWERING-NEXT:         _v174
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v175: i32 = __slate_alloca_frame0.0;
// LOWERING-NEXT:     __retval = _v175;
// LOWERING-NEXT:     let _v176: i32 = __retval;
// LOWERING-NEXT:     return _v176;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = gnu_allocation_extensions();
// LOWERING-NEXT:     let _v3: i32 = gnu_obstack_extensions();
// LOWERING-NEXT:     let _v4: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3) };
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v6 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: struct __SlateAllocaFrame0(i32, *mut i8, *mut i8);
// REWRITES-EMPTY:
// REWRITES-NEXT: mod __slate_bitfields {
// REWRITES-NEXT:     #[bitfields::bitfield(u8, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// REWRITES-NEXT:     pub struct __SlateBitfield_obstack_10 {
// REWRITES-NEXT:         #[bits(1)]
// REWRITES-NEXT:         pub _reserved_0: u128,
// REWRITES-NEXT:         #[bits(1)]
// REWRITES-NEXT:         pub maybe_empty_object: u32,
// REWRITES-NEXT:         #[bits(6)]
// REWRITES-NEXT:         pub _reserved_1: u128,
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct _obstack_chunk {
// REWRITES-NEXT:     limit: *mut i8,
// REWRITES-NEXT:     prev: *mut _obstack_chunk,
// REWRITES-NEXT:     contents: [i8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union _unnamed_at__home_takashi_Projects_slate_libc_shim_include_obstack_h_28_3_ {
// REWRITES-NEXT:     tempint: i64,
// REWRITES-NEXT:     tempptr: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union anon_0 {
// REWRITES-NEXT:     tempint: i64,
// REWRITES-NEXT:     tempptr: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct obstack {
// REWRITES-NEXT:     __bitfield_0: i64,
// REWRITES-NEXT:     __bitfield_1: *mut _obstack_chunk,
// REWRITES-NEXT:     __bitfield_2: *mut i8,
// REWRITES-NEXT:     __bitfield_3: *mut i8,
// REWRITES-NEXT:     __bitfield_4: *mut i8,
// REWRITES-NEXT:     __bitfield_5: anon_0,
// REWRITES-NEXT:     __bitfield_6: i32,
// REWRITES-NEXT:     __bitfield_7: Option<unsafe extern "C" fn(*mut core::ffi::c_void, i64) -> *mut _obstack_chunk>,
// REWRITES-NEXT:     __bitfield_8: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut _obstack_chunk)>,
// REWRITES-NEXT:     __bitfield_9: *mut core::ffi::c_void,
// REWRITES-NEXT:     __bitfield_10: __slate_bitfields::__SlateBitfield_obstack_10,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn reallocarray(_0: *mut core::ffi::c_void, _1: usize, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memalign(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn valloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn pvalloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn sysconf(_0: i32) -> i64;
// REWRITES-NEXT:     fn malloc_usable_size(_0: *mut core::ffi::c_void) -> usize;
// REWRITES-NEXT:     fn mallopt(_0: i32, _1: i32) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn _obstack_begin(_0: *mut obstack, _1: i32, _2: i32, _3: Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void>, _4: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>) -> i32;
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn _obstack_newchunk(_0: *mut obstack, _1: i32);
// REWRITES-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// REWRITES-NEXT:     fn obstack_free(_0: *mut obstack, _1: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_allocation_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut values: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut aligned: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut page: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut rounded: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut page_size: i64 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: let _v0: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v1: u64 = 4;
// REWRITES-NEXT: let _v2: u64 = 4;
// REWRITES-NEXT: let _v3: *mut core::ffi::c_void = unsafe { reallocarray(_v0 as *mut core::ffi::c_void, _v1 as usize, _v2 as usize) };
// REWRITES-NEXT: values = _v3 as *mut i32;
// REWRITES-NEXT: let _v5: u64 = 64;
// REWRITES-NEXT: let _v6: u64 = 80;
// REWRITES-NEXT: aligned = unsafe { memalign(_v5 as usize, _v6 as usize) };
// REWRITES-NEXT: let _v8: u64 = 1;
// REWRITES-NEXT: page = unsafe { valloc(_v8 as usize) };
// REWRITES-NEXT: let _v10: u64 = 1;
// REWRITES-NEXT: rounded = unsafe { pvalloc(_v10 as usize) };
// REWRITES-NEXT: let _v12: i32 = 30;
// REWRITES-NEXT: page_size = unsafe { sysconf(_v12 as i32) };
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut index: i32 = 0;
// REWRITES-NEXT:         index = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v17: i32 = 4;
// REWRITES-NEXT:                     if !(index < _v17) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v20: i32 = 1;
// REWRITES-NEXT:                                     let _v21: i32 = index + _v20;
// REWRITES-NEXT:                                     let _v24: *mut i32 = values;
// REWRITES-NEXT:                                     let _v25: *mut i32 = unsafe { _v24.offset((index as i64) as isize) };
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         *_v25 = _v21;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     index = index + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v28: i64 = 0;
// REWRITES-NEXT: let _v29: *mut i32 = values;
// REWRITES-NEXT: let _v30: *mut i32 = unsafe { _v29.add(0) };
// REWRITES-NEXT: let _v31: i32 = unsafe { *_v30 };
// REWRITES-NEXT: let _v32: i64 = 1;
// REWRITES-NEXT: let _v33: *mut i32 = values;
// REWRITES-NEXT: let _v34: *mut i32 = unsafe { _v33.add(1) };
// REWRITES-NEXT: let _v36: i32 = _v31 + unsafe { *_v34 };
// REWRITES-NEXT: let _v37: i64 = 2;
// REWRITES-NEXT: let _v38: *mut i32 = values;
// REWRITES-NEXT: let _v39: *mut i32 = unsafe { _v38.add(2) };
// REWRITES-NEXT: let _v41: i32 = _v36 + unsafe { *_v39 };
// REWRITES-NEXT: let _v42: i64 = 3;
// REWRITES-NEXT: let _v43: *mut i32 = values;
// REWRITES-NEXT: let _v44: *mut i32 = unsafe { _v43.add(3) };
// REWRITES-NEXT: total = total + (_v41 + unsafe { *_v44 });
// REWRITES-NEXT: let _v51: u64 = (unsafe { malloc_usable_size((values as *mut core::ffi::c_void) as *mut core::ffi::c_void) }) as u64;
// REWRITES-NEXT: let _v52: u64 = 4;
// REWRITES-NEXT: let _v53: u64 = 4;
// REWRITES-NEXT: total = total + ((_v51 >= _v52 * _v53) as i32);
// REWRITES-NEXT: let _v60: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v62: bool = if aligned != _v60 {
// REWRITES-NEXT:         let _v65: u64 = 64;
// REWRITES-NEXT:         let _v67: u64 = 0;
// REWRITES-NEXT:         let _v68: bool = (aligned as u64) % _v65 == _v67;
// REWRITES-NEXT:     _v68
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v69: bool = false;
// REWRITES-NEXT:     _v69
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v62 as i32);
// REWRITES-NEXT: let _v74: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v76: bool = if page != _v74 {
// REWRITES-NEXT:         let _v82: u64 = 0;
// REWRITES-NEXT:         let _v83: bool = (page as u64) % (page_size as u64) == _v82;
// REWRITES-NEXT:     _v83
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v84: bool = false;
// REWRITES-NEXT:     _v84
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v76 as i32);
// REWRITES-NEXT: let _v89: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v91: bool = if rounded != _v89 {
// REWRITES-NEXT:         let _v97: u64 = 0;
// REWRITES-NEXT:         let _v98: bool = (rounded as u64) % (page_size as u64) == _v97;
// REWRITES-NEXT:     _v98
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v99: bool = false;
// REWRITES-NEXT:     _v99
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v91 as i32);
// REWRITES-NEXT: let _v104: u64 = (unsafe { malloc_usable_size(rounded as *mut core::ffi::c_void) }) as u64;
// REWRITES-NEXT: total = total + ((_v104 >= (page_size as u64)) as i32);
// REWRITES-NEXT: let _v111: i32 = -5;
// REWRITES-NEXT: let _v112: i32 = 1;
// REWRITES-NEXT: let _v113: i32 = unsafe { mallopt(_v111 as i32, _v112 as i32) };
// REWRITES-NEXT: let _v114: i32 = 0;
// REWRITES-NEXT: total = total + ((_v113 != _v114) as i32);
// REWRITES-NEXT: unsafe { free((values as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: unsafe { free(aligned as *mut core::ffi::c_void) };
// REWRITES-NEXT: unsafe { free(page as *mut core::ffi::c_void) };
// REWRITES-NEXT: unsafe { free(rounded as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_obstack_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut storage: obstack = obstack { __bitfield_0: 0, __bitfield_1: std::ptr::null_mut(), __bitfield_2: std::ptr::null_mut(), __bitfield_3: std::ptr::null_mut(), __bitfield_4: std::ptr::null_mut(), __bitfield_5: anon_0 { tempint: 0 }, __bitfield_6: 0, __bitfield_7: None, __bitfield_8: None, __bitfield_9: std::ptr::null_mut(), __bitfield_10: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_obstack_10>(0) } };
// REWRITES-NEXT: let mut __slate_alloca_frame0: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, std::ptr::null_mut(), std::ptr::null_mut());
// REWRITES-NEXT: __slate_alloca_frame0.0 = 0;
// REWRITES-NEXT: let _v1: i32 = 0;
// REWRITES-NEXT: let _v2: i32 = 0;
// REWRITES-NEXT: let _v3: Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void> = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i64) -> *mut core::ffi::c_void>>(malloc as *const ()) };
// REWRITES-NEXT: let _v4: i32 = unsafe { _obstack_begin(std::ptr::addr_of_mut!(storage) as *mut obstack, _v1 as i32, _v2 as i32, _v3, Some(free)) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempint = 3;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v6: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v7: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT: let _v8: *mut i8 = unsafe { _v6.offset(_v7 as isize) };
// REWRITES-NEXT: let _v9: i32 = 1;
// REWRITES-NEXT: let _v10: *mut i8 = unsafe { _v8.add(1) };
// REWRITES-NEXT: let _v12: bool = _v10 > storage.__bitfield_4;
// REWRITES-NEXT: let _v13: i32 = if _v12 {
// REWRITES-NEXT:         let _v15: i64 = 1;
// REWRITES-NEXT:         unsafe { _obstack_newchunk(std::ptr::addr_of_mut!(storage) as *mut obstack, (((unsafe { storage.__bitfield_5.tempint }) + _v15) as i32) as i32) };
// REWRITES-NEXT:         let _v18: i32 = 0;
// REWRITES-NEXT:     _v18
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v19: i32 = 0;
// REWRITES-NEXT:     _v19
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v21: *mut core::ffi::c_void = storage.__bitfield_3 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v22: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { std::ptr::copy_nonoverlapping(((_v22 as *mut core::ffi::c_void) as *const core::ffi::c_void) as *const u8, (_v21 as *mut core::ffi::c_void) as *mut u8, (((unsafe { storage.__bitfield_5.tempint }) as u64) as usize) as usize) };
// REWRITES-NEXT: let _v27: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT: let _v28: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v29: *mut i8 = unsafe { _v28.offset(_v27 as isize) };
// REWRITES-NEXT: storage.__bitfield_3 = _v29;
// REWRITES-NEXT: let _v30: i8 = 0;
// REWRITES-NEXT: let _v31: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v32: i32 = 1;
// REWRITES-NEXT: let _v33: *mut i8 = unsafe { _v31.add(1) };
// REWRITES-NEXT: storage.__bitfield_3 = _v33;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v31 = _v30;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v36: bool = storage.__bitfield_3 == storage.__bitfield_2;
// REWRITES-NEXT: let _v37: i32 = if _v36 {
// REWRITES-NEXT:         let _v38: u32 = 1;
// REWRITES-NEXT:         let _v39: u32 = (_v38 as u32) << 31 >> 31;
// REWRITES-NEXT:         storage.__bitfield_10.set_maybe_empty_object((_v38 as u32) << 31 >> 31);
// REWRITES-NEXT:         let _v40: i32 = 0;
// REWRITES-NEXT:     _v40
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v41: i32 = 0;
// REWRITES-NEXT:     _v41
// REWRITES-NEXT: };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempptr = storage.__bitfield_2 as *mut core::ffi::c_void;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v44: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v45: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let _v46: i64 = unsafe { _v44.offset_from(_v45) as i64 };
// REWRITES-NEXT: let _v49: i64 = _v46 + (storage.__bitfield_6 as i64);
// REWRITES-NEXT: let _v51: i32 = !storage.__bitfield_6;
// REWRITES-NEXT: storage.__bitfield_3 = (_v49 & (_v51 as i64)) as *mut i8;
// REWRITES-NEXT: let _v55: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v58: i64 = unsafe { _v55.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT: let _v59: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT: let _v62: i64 = unsafe { _v59.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT: let _v64: *mut i8 = if _v58 > _v62 {
// REWRITES-NEXT:         let _v65: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT:         storage.__bitfield_3 = _v65;
// REWRITES-NEXT:     _v65
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v66: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:     _v66
// REWRITES-NEXT: };
// REWRITES-NEXT: storage.__bitfield_2 = storage.__bitfield_3;
// REWRITES-NEXT: __slate_alloca_frame0.2 = (unsafe { storage.__bitfield_5.tempptr }) as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempint = 4;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v71: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v72: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT: let _v73: *mut i8 = unsafe { _v71.offset(_v72 as isize) };
// REWRITES-NEXT: let _v74: i32 = 1;
// REWRITES-NEXT: let _v75: *mut i8 = unsafe { _v73.add(1) };
// REWRITES-NEXT: let _v77: bool = _v75 > storage.__bitfield_4;
// REWRITES-NEXT: let _v78: i32 = if _v77 {
// REWRITES-NEXT:         let _v80: i64 = 1;
// REWRITES-NEXT:         unsafe { _obstack_newchunk(std::ptr::addr_of_mut!(storage) as *mut obstack, (((unsafe { storage.__bitfield_5.tempint }) + _v80) as i32) as i32) };
// REWRITES-NEXT:         let _v83: i32 = 0;
// REWRITES-NEXT:     _v83
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v84: i32 = 0;
// REWRITES-NEXT:     _v84
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v86: *mut core::ffi::c_void = storage.__bitfield_3 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v87: *mut i8 = b"libc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { std::ptr::copy_nonoverlapping(((_v87 as *mut core::ffi::c_void) as *const core::ffi::c_void) as *const u8, (_v86 as *mut core::ffi::c_void) as *mut u8, (((unsafe { storage.__bitfield_5.tempint }) as u64) as usize) as usize) };
// REWRITES-NEXT: let _v92: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT: let _v93: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v94: *mut i8 = unsafe { _v93.offset(_v92 as isize) };
// REWRITES-NEXT: storage.__bitfield_3 = _v94;
// REWRITES-NEXT: let _v95: i8 = 0;
// REWRITES-NEXT: let _v96: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v97: i32 = 1;
// REWRITES-NEXT: let _v98: *mut i8 = unsafe { _v96.add(1) };
// REWRITES-NEXT: storage.__bitfield_3 = _v98;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v96 = _v95;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v101: bool = storage.__bitfield_3 == storage.__bitfield_2;
// REWRITES-NEXT: let _v102: i32 = if _v101 {
// REWRITES-NEXT:         let _v103: u32 = 1;
// REWRITES-NEXT:         let _v104: u32 = (_v103 as u32) << 31 >> 31;
// REWRITES-NEXT:         storage.__bitfield_10.set_maybe_empty_object((_v103 as u32) << 31 >> 31);
// REWRITES-NEXT:         let _v105: i32 = 0;
// REWRITES-NEXT:     _v105
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v106: i32 = 0;
// REWRITES-NEXT:     _v106
// REWRITES-NEXT: };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempptr = storage.__bitfield_2 as *mut core::ffi::c_void;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v109: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v110: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let _v111: i64 = unsafe { _v109.offset_from(_v110) as i64 };
// REWRITES-NEXT: let _v114: i64 = _v111 + (storage.__bitfield_6 as i64);
// REWRITES-NEXT: let _v116: i32 = !storage.__bitfield_6;
// REWRITES-NEXT: storage.__bitfield_3 = (_v114 & (_v116 as i64)) as *mut i8;
// REWRITES-NEXT: let _v120: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v123: i64 = unsafe { _v120.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT: let _v124: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT: let _v127: i64 = unsafe { _v124.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT: let _v129: *mut i8 = if _v123 > _v127 {
// REWRITES-NEXT:         let _v130: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT:         storage.__bitfield_3 = _v130;
// REWRITES-NEXT:     _v130
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v131: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:     _v131
// REWRITES-NEXT: };
// REWRITES-NEXT: storage.__bitfield_2 = storage.__bitfield_3;
// REWRITES-NEXT: __slate_alloca_frame0.1 = (unsafe { storage.__bitfield_5.tempptr }) as *mut i8;
// REWRITES-NEXT: let _v135: *mut i8 = __slate_alloca_frame0.2;
// REWRITES-NEXT: let _v136: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v137: i32 = unsafe { strcmp(_v135 as *const i8, _v136 as *const i8) };
// REWRITES-NEXT: let _v138: i32 = 0;
// REWRITES-NEXT: __slate_alloca_frame0.0 = __slate_alloca_frame0.0 + ((_v137 == _v138) as i32);
// REWRITES-NEXT: let _v143: *mut i8 = __slate_alloca_frame0.1;
// REWRITES-NEXT: let _v144: *mut i8 = b"libc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v145: i32 = unsafe { strcmp(_v143 as *const i8, _v144 as *const i8) };
// REWRITES-NEXT: let _v146: i32 = 0;
// REWRITES-NEXT: __slate_alloca_frame0.0 = __slate_alloca_frame0.0 + ((_v145 == _v146) as i32);
// REWRITES-NEXT: let _v151: *mut i8 = storage.__bitfield_3;
// REWRITES-NEXT: let _v152: *mut i8 = storage.__bitfield_2;
// REWRITES-NEXT: let _v153: i64 = unsafe { _v151.offset_from(_v152) as i64 };
// REWRITES-NEXT: let _v155: u32 = 0;
// REWRITES-NEXT: __slate_alloca_frame0.0 = __slate_alloca_frame0.0 + (((_v153 as u32) == _v155) as i32);
// REWRITES-NEXT: let _v160: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let _v163: i64 = unsafe { _v160.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         storage.__bitfield_5.tempint = _v163;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v165: i64 = 0;
// REWRITES-NEXT: let _v166: bool = (unsafe { storage.__bitfield_5.tempint }) > _v165;
// REWRITES-NEXT: let _v167: bool = if _v166 {
// REWRITES-NEXT:         let _v168: i64 = unsafe { storage.__bitfield_5.tempint };
// REWRITES-NEXT:         let _v169: *mut i8 = storage.__bitfield_4;
// REWRITES-NEXT:         let _v172: i64 = unsafe { _v169.offset_from(storage.__bitfield_1 as *mut i8) as i64 };
// REWRITES-NEXT:         let _v173: bool = _v168 < _v172;
// REWRITES-NEXT:     _v173
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v174: bool = false;
// REWRITES-NEXT:     _v174
// REWRITES-NEXT: };
// REWRITES-NEXT: __retval = __slate_alloca_frame0.0;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = gnu_allocation_extensions();
// REWRITES-NEXT: let _v3: i32 = gnu_obstack_extensions();
// REWRITES-NEXT: let _v4: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
