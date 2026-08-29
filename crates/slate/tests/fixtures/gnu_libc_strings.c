#define _GNU_SOURCE
#include <argz.h>
#include <envz.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int gnu_string_extensions(void) {
  char       destination[16] = {};
  const char repeated[]      = "abca";
  char       obscured[]      = "gnu";
  char       tokens[]        = "a::bc";
  char      *cursor          = tokens;
  char      *token;
  char      *end         = mempcpy(destination, "slate", 6);
  int        token_score = 0;

  while ((token = strsep(&cursor, ":")) != NULL) {
    token_score = token_score * 10 + (int)strlen(token);
  }

  memfrob(obscured, 3);
  memfrob(obscured, 3);

  return (int)(end - destination) + (strcmp(destination, "slate") == 0) +
         ((const char *)memrchr(repeated, 'a', 4) - repeated) +
         ((const char *)rawmemchr(repeated, 'c') - repeated) +
         (strchrnul("abc", 'z') - "abc") +
         (strcasestr("GNU Library", "library") != NULL) +
         (strverscmp("release-2", "release-10") < 0) +
         (strcmp(obscured, "gnu") == 0) + token_score +
         (strcmp(strerrorname_np(EINVAL), "EINVAL") == 0) +
         (strerrordesc_np(EINVAL) != NULL);
}

static int gnu_argz_extensions(void) {
  char        *argz   = NULL;
  size_t       length = 0;
  char        *arguments[6];
  unsigned int replacements = 0;
  int          total        = 0;

  total += argz_create_sep("one:two:three", ':', &argz, &length) == 0;
  total += argz_count(argz, length) == 3;
  argz_extract(argz, length, arguments);
  total += strcmp(arguments[1], "two") == 0;
  total += argz_add(&argz, &length, "four") == 0;
  total += argz_replace(&argz, &length, "three", "THREE", &replacements) == 0;
  total += replacements == 1;
  total += argz_count(argz, length) == 4;
  argz_stringify(argz, length, ',');
  total += strcmp(argz, "one,two,THREE,four") == 0;
  free(argz);
  return total;
}

static int gnu_envz_extensions(void) {
  char  *envz   = NULL;
  size_t length = 0;
  int    total  = 0;

  total += envz_add(&envz, &length, "ALPHA", "one") == 0;
  total += envz_add(&envz, &length, "BETA", NULL) == 0;
  total += strcmp(envz_get(envz, length, "ALPHA"), "one") == 0;
  total += envz_entry(envz, length, "BETA") != NULL;
  envz_remove(&envz, &length, "ALPHA");
  total += envz_get(envz, length, "ALPHA") == NULL;
  envz_strip(&envz, &length);
  total += length == 0;
  free(envz);
  return total;
}

int main(void) {
  printf("%d %d %d\n", gnu_string_extensions(), gnu_argz_extensions(),
         gnu_envz_extensions());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: struct __SlateAllocaFrame0(i32, *mut i8, *mut i8, *mut i8, [i8; 6], [i8; 4], [i8; 5], aligned::Aligned<aligned::A16, [i8; 16]>, i32);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn mempcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strsep(_0: *mut *mut i8, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn strlen(_0: *const i8) -> usize;
// LOWERING-NEXT:     fn memfrob(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// LOWERING-NEXT:     fn memrchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn rawmemchr(_0: *const core::ffi::c_void, _1: i32) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strchrnul(_0: *const i8, _1: i32) -> *mut i8;
// LOWERING-NEXT:     fn strcasestr(_0: *const i8, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn strverscmp(_0: *const i8, _1: *const i8) -> i32;
// LOWERING-NEXT:     fn strerrorname_np(_0: i32) -> *const i8;
// LOWERING-NEXT:     fn strerrordesc_np(_0: i32) -> *const i8;
// LOWERING-NEXT:     fn argz_create_sep(_0: *const i8, _1: i32, _2: *mut *mut i8, _3: *mut usize) -> i32;
// LOWERING-NEXT:     fn argz_count(_0: *const i8, _1: usize) -> usize;
// LOWERING-NEXT:     fn argz_extract(_0: *const i8, _1: usize, _2: *mut *mut i8);
// LOWERING-NEXT:     fn argz_add(_0: *mut *mut i8, _1: *mut usize, _2: *const i8) -> i32;
// LOWERING-NEXT:     fn argz_replace(_0: *mut *mut i8, _1: *mut usize, _2: *const i8, _3: *const i8, _4: *mut u32) -> i32;
// LOWERING-NEXT:     fn argz_stringify(_0: *mut i8, _1: usize, _2: i32);
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn envz_add(_0: *mut *mut i8, _1: *mut usize, _2: *const i8, _3: *const i8) -> i32;
// LOWERING-NEXT:     fn envz_get(_0: *const i8, _1: usize, _2: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn envz_entry(_0: *const i8, _1: usize, _2: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn envz_remove(_0: *mut *mut i8, _1: *mut usize, _2: *const i8);
// LOWERING-NEXT:     fn envz_strip(_0: *mut *mut i8, _1: *mut usize);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_string_extensions() -> i32 {
// LOWERING-NEXT:     let mut __slate_alloca_frame0: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, std::ptr::null_mut(), std::ptr::null_mut(), std::ptr::null_mut(), [0; 6], [0; 4], [0; 5], aligned::Aligned([0; 16]), 0);
// LOWERING-NEXT:     *__slate_alloca_frame0.7 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     __slate_alloca_frame0.6 = [97, 98, 99, 97, 0];
// LOWERING-NEXT:     __slate_alloca_frame0.5 = [103, 110, 117, 0];
// LOWERING-NEXT:     __slate_alloca_frame0.4 = [97, 58, 58, 98, 99, 0];
// LOWERING-NEXT:     let _v0: *mut i8 = __slate_alloca_frame0.4.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     __slate_alloca_frame0.3 = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = __slate_alloca_frame0.7.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v3: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = _v3 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v5: u64 = 6;
// LOWERING-NEXT:     let _v6: *mut core::ffi::c_void = unsafe { mempcpy(_v2 as *mut core::ffi::c_void, _v4 as *const core::ffi::c_void, _v5 as usize) };
// LOWERING-NEXT:     let _v7: *mut i8 = _v6 as *mut i8;
// LOWERING-NEXT:     __slate_alloca_frame0.1 = _v7;
// LOWERING-NEXT:     let _v8: i32 = 0;
// LOWERING-NEXT:     __slate_alloca_frame0.0 = _v8;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v9: *mut i8 = b":\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v10: *mut i8 = unsafe { strsep(std::ptr::addr_of_mut!(__slate_alloca_frame0.3) as *mut *mut i8, _v9 as *const i8) };
// LOWERING-NEXT:             __slate_alloca_frame0.2 = _v10;
// LOWERING-NEXT:             let _v11: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:             let _v12: bool = _v10 != _v11;
// LOWERING-NEXT:             if !_v12 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v13: i32 = __slate_alloca_frame0.0;
// LOWERING-NEXT:                 let _v14: i32 = 10;
// LOWERING-NEXT:                 let _v15: i32 = _v13 * _v14;
// LOWERING-NEXT:                 let _v16: *mut i8 = __slate_alloca_frame0.2;
// LOWERING-NEXT:                 let _v17: u64 = (unsafe { strlen(_v16 as *const i8) }) as u64;
// LOWERING-NEXT:                 let _v18: i32 = _v17 as i32;
// LOWERING-NEXT:                 let _v19: i32 = _v15 + _v18;
// LOWERING-NEXT:                 __slate_alloca_frame0.0 = _v19;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v20: *mut i8 = __slate_alloca_frame0.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v21: *mut core::ffi::c_void = _v20 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v22: u64 = 3;
// LOWERING-NEXT:     let _v23: *mut core::ffi::c_void = unsafe { memfrob(_v21 as *mut core::ffi::c_void, _v22 as usize) };
// LOWERING-NEXT:     let _v24: *mut i8 = __slate_alloca_frame0.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: *mut core::ffi::c_void = _v24 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v26: u64 = 3;
// LOWERING-NEXT:     let _v27: *mut core::ffi::c_void = unsafe { memfrob(_v25 as *mut core::ffi::c_void, _v26 as usize) };
// LOWERING-NEXT:     let _v28: *mut i8 = __slate_alloca_frame0.1;
// LOWERING-NEXT:     let _v29: *mut i8 = __slate_alloca_frame0.7.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v30: i64 = unsafe { _v28.offset_from(_v29) as i64 };
// LOWERING-NEXT:     let _v31: i32 = _v30 as i32;
// LOWERING-NEXT:     let _v32: *mut i8 = __slate_alloca_frame0.7.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v33: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v34: i32 = unsafe { strcmp(_v32 as *const i8, _v33 as *const i8) };
// LOWERING-NEXT:     let _v35: i32 = 0;
// LOWERING-NEXT:     let _v36: bool = _v34 == _v35;
// LOWERING-NEXT:     let _v37: i32 = _v36 as i32;
// LOWERING-NEXT:     let _v38: i32 = _v31 + _v37;
// LOWERING-NEXT:     let _v39: i64 = _v38 as i64;
// LOWERING-NEXT:     let _v40: *mut i8 = __slate_alloca_frame0.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v41: *mut core::ffi::c_void = _v40 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v42: i32 = 97;
// LOWERING-NEXT:     let _v43: u64 = 4;
// LOWERING-NEXT:     let _v44: *mut core::ffi::c_void = unsafe { memrchr(_v41 as *const core::ffi::c_void, _v42 as i32, _v43 as usize) };
// LOWERING-NEXT:     let _v45: *mut i8 = _v44 as *mut i8;
// LOWERING-NEXT:     let _v46: *mut i8 = __slate_alloca_frame0.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v47: i64 = unsafe { _v45.offset_from(_v46) as i64 };
// LOWERING-NEXT:     let _v48: i64 = _v39 + _v47;
// LOWERING-NEXT:     let _v49: *mut i8 = __slate_alloca_frame0.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v50: *mut core::ffi::c_void = _v49 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v51: i32 = 99;
// LOWERING-NEXT:     let _v52: *mut core::ffi::c_void = unsafe { rawmemchr(_v50 as *const core::ffi::c_void, _v51 as i32) };
// LOWERING-NEXT:     let _v53: *mut i8 = _v52 as *mut i8;
// LOWERING-NEXT:     let _v54: *mut i8 = __slate_alloca_frame0.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v55: i64 = unsafe { _v53.offset_from(_v54) as i64 };
// LOWERING-NEXT:     let _v56: i64 = _v48 + _v55;
// LOWERING-NEXT:     let _v57: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v58: i32 = 122;
// LOWERING-NEXT:     let _v59: *mut i8 = unsafe { strchrnul(_v57 as *const i8, _v58 as i32) };
// LOWERING-NEXT:     let _v60: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v61: i64 = unsafe { _v59.offset_from(_v60) as i64 };
// LOWERING-NEXT:     let _v62: i64 = _v56 + _v61;
// LOWERING-NEXT:     let _v63: *mut i8 = b"GNU Library\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v64: *mut i8 = b"library\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v65: *mut i8 = unsafe { strcasestr(_v63 as *const i8, _v64 as *const i8) };
// LOWERING-NEXT:     let _v66: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v67: bool = _v65 != _v66;
// LOWERING-NEXT:     let _v68: i32 = _v67 as i32;
// LOWERING-NEXT:     let _v69: i64 = _v68 as i64;
// LOWERING-NEXT:     let _v70: i64 = _v62 + _v69;
// LOWERING-NEXT:     let _v71: *mut i8 = b"release-2\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v72: *mut i8 = b"release-10\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v73: i32 = unsafe { strverscmp(_v71 as *const i8, _v72 as *const i8) };
// LOWERING-NEXT:     let _v74: i32 = 0;
// LOWERING-NEXT:     let _v75: bool = _v73 < _v74;
// LOWERING-NEXT:     let _v76: i32 = _v75 as i32;
// LOWERING-NEXT:     let _v77: i64 = _v76 as i64;
// LOWERING-NEXT:     let _v78: i64 = _v70 + _v77;
// LOWERING-NEXT:     let _v79: *mut i8 = __slate_alloca_frame0.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v80: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v81: i32 = unsafe { strcmp(_v79 as *const i8, _v80 as *const i8) };
// LOWERING-NEXT:     let _v82: i32 = 0;
// LOWERING-NEXT:     let _v83: bool = _v81 == _v82;
// LOWERING-NEXT:     let _v84: i32 = _v83 as i32;
// LOWERING-NEXT:     let _v85: i64 = _v84 as i64;
// LOWERING-NEXT:     let _v86: i64 = _v78 + _v85;
// LOWERING-NEXT:     let _v87: i32 = __slate_alloca_frame0.0;
// LOWERING-NEXT:     let _v88: i64 = _v87 as i64;
// LOWERING-NEXT:     let _v89: i64 = _v86 + _v88;
// LOWERING-NEXT:     let _v90: i32 = 22;
// LOWERING-NEXT:     let _v91: *mut i8 = (unsafe { strerrorname_np(_v90 as i32) }) as *mut i8;
// LOWERING-NEXT:     let _v92: *mut i8 = b"EINVAL\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v93: i32 = unsafe { strcmp(_v91 as *const i8, _v92 as *const i8) };
// LOWERING-NEXT:     let _v94: i32 = 0;
// LOWERING-NEXT:     let _v95: bool = _v93 == _v94;
// LOWERING-NEXT:     let _v96: i32 = _v95 as i32;
// LOWERING-NEXT:     let _v97: i64 = _v96 as i64;
// LOWERING-NEXT:     let _v98: i64 = _v89 + _v97;
// LOWERING-NEXT:     let _v99: i32 = 22;
// LOWERING-NEXT:     let _v100: *mut i8 = (unsafe { strerrordesc_np(_v99 as i32) }) as *mut i8;
// LOWERING-NEXT:     let _v101: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v102: bool = _v100 != _v101;
// LOWERING-NEXT:     let _v103: i32 = _v102 as i32;
// LOWERING-NEXT:     let _v104: i64 = _v103 as i64;
// LOWERING-NEXT:     let _v105: i64 = _v98 + _v104;
// LOWERING-NEXT:     let _v106: i32 = _v105 as i32;
// LOWERING-NEXT:     __slate_alloca_frame0.8 = _v106;
// LOWERING-NEXT:     let _v107: i32 = __slate_alloca_frame0.8;
// LOWERING-NEXT:     return _v107;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_argz_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut argz: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut length: u64 = 0;
// LOWERING-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 6]> = aligned::Aligned([std::ptr::null_mut(); 6]);
// LOWERING-NEXT:     let mut replacements: u32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let _v0: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     argz = _v0;
// LOWERING-NEXT:     let _v1: u64 = 0;
// LOWERING-NEXT:     length = _v1;
// LOWERING-NEXT:     let _v2: u32 = 0;
// LOWERING-NEXT:     replacements = _v2;
// LOWERING-NEXT:     let _v3: i32 = 0;
// LOWERING-NEXT:     total = _v3;
// LOWERING-NEXT:     let _v4: *mut i8 = b"one:two:three\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i32 = 58;
// LOWERING-NEXT:     let _v6: i32 = unsafe { argz_create_sep(_v4 as *const i8, _v5 as i32, std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize) };
// LOWERING-NEXT:     let _v7: i32 = 0;
// LOWERING-NEXT:     let _v8: bool = _v6 == _v7;
// LOWERING-NEXT:     let _v9: i32 = _v8 as i32;
// LOWERING-NEXT:     let _v10: i32 = total;
// LOWERING-NEXT:     let _v11: i32 = _v10 + _v9;
// LOWERING-NEXT:     total = _v11;
// LOWERING-NEXT:     let _v12: *mut i8 = argz;
// LOWERING-NEXT:     let _v13: u64 = length;
// LOWERING-NEXT:     let _v14: u64 = (unsafe { argz_count(_v12 as *const i8, _v13 as usize) }) as u64;
// LOWERING-NEXT:     let _v15: u64 = 3;
// LOWERING-NEXT:     let _v16: bool = _v14 == _v15;
// LOWERING-NEXT:     let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:     let _v18: i32 = total;
// LOWERING-NEXT:     let _v19: i32 = _v18 + _v17;
// LOWERING-NEXT:     total = _v19;
// LOWERING-NEXT:     let _v20: *mut i8 = argz;
// LOWERING-NEXT:     let _v21: u64 = length;
// LOWERING-NEXT:     let _v22: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:     unsafe { argz_extract(_v20 as *const i8, _v21 as usize, _v22 as *mut *mut i8) };
// LOWERING-NEXT:     let _v23: i64 = 1;
// LOWERING-NEXT:     let _v24: *mut i8 = arguments[(_v23 as usize)];
// LOWERING-NEXT:     let _v25: *mut i8 = b"two\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v26: i32 = unsafe { strcmp(_v24 as *const i8, _v25 as *const i8) };
// LOWERING-NEXT:     let _v27: i32 = 0;
// LOWERING-NEXT:     let _v28: bool = _v26 == _v27;
// LOWERING-NEXT:     let _v29: i32 = _v28 as i32;
// LOWERING-NEXT:     let _v30: i32 = total;
// LOWERING-NEXT:     let _v31: i32 = _v30 + _v29;
// LOWERING-NEXT:     total = _v31;
// LOWERING-NEXT:     let _v32: *mut i8 = b"four\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v33: i32 = unsafe { argz_add(std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v32 as *const i8) };
// LOWERING-NEXT:     let _v34: i32 = 0;
// LOWERING-NEXT:     let _v35: bool = _v33 == _v34;
// LOWERING-NEXT:     let _v36: i32 = _v35 as i32;
// LOWERING-NEXT:     let _v37: i32 = total;
// LOWERING-NEXT:     let _v38: i32 = _v37 + _v36;
// LOWERING-NEXT:     total = _v38;
// LOWERING-NEXT:     let _v39: *mut i8 = b"three\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v40: *mut i8 = b"THREE\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v41: i32 = unsafe { argz_replace(std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v39 as *const i8, _v40 as *const i8, std::ptr::addr_of_mut!(replacements) as *mut u32) };
// LOWERING-NEXT:     let _v42: i32 = 0;
// LOWERING-NEXT:     let _v43: bool = _v41 == _v42;
// LOWERING-NEXT:     let _v44: i32 = _v43 as i32;
// LOWERING-NEXT:     let _v45: i32 = total;
// LOWERING-NEXT:     let _v46: i32 = _v45 + _v44;
// LOWERING-NEXT:     total = _v46;
// LOWERING-NEXT:     let _v47: u32 = replacements;
// LOWERING-NEXT:     let _v48: u32 = 1;
// LOWERING-NEXT:     let _v49: bool = _v47 == _v48;
// LOWERING-NEXT:     let _v50: i32 = _v49 as i32;
// LOWERING-NEXT:     let _v51: i32 = total;
// LOWERING-NEXT:     let _v52: i32 = _v51 + _v50;
// LOWERING-NEXT:     total = _v52;
// LOWERING-NEXT:     let _v53: *mut i8 = argz;
// LOWERING-NEXT:     let _v54: u64 = length;
// LOWERING-NEXT:     let _v55: u64 = (unsafe { argz_count(_v53 as *const i8, _v54 as usize) }) as u64;
// LOWERING-NEXT:     let _v56: u64 = 4;
// LOWERING-NEXT:     let _v57: bool = _v55 == _v56;
// LOWERING-NEXT:     let _v58: i32 = _v57 as i32;
// LOWERING-NEXT:     let _v59: i32 = total;
// LOWERING-NEXT:     let _v60: i32 = _v59 + _v58;
// LOWERING-NEXT:     total = _v60;
// LOWERING-NEXT:     let _v61: *mut i8 = argz;
// LOWERING-NEXT:     let _v62: u64 = length;
// LOWERING-NEXT:     let _v63: i32 = 44;
// LOWERING-NEXT:     unsafe { argz_stringify(_v61 as *mut i8, _v62 as usize, _v63 as i32) };
// LOWERING-NEXT:     let _v64: *mut i8 = argz;
// LOWERING-NEXT:     let _v65: *mut i8 = b"one,two,THREE,four\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v66: i32 = unsafe { strcmp(_v64 as *const i8, _v65 as *const i8) };
// LOWERING-NEXT:     let _v67: i32 = 0;
// LOWERING-NEXT:     let _v68: bool = _v66 == _v67;
// LOWERING-NEXT:     let _v69: i32 = _v68 as i32;
// LOWERING-NEXT:     let _v70: i32 = total;
// LOWERING-NEXT:     let _v71: i32 = _v70 + _v69;
// LOWERING-NEXT:     total = _v71;
// LOWERING-NEXT:     let _v72: *mut i8 = argz;
// LOWERING-NEXT:     let _v73: *mut core::ffi::c_void = _v72 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v73 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v74: i32 = total;
// LOWERING-NEXT:     __retval = _v74;
// LOWERING-NEXT:     let _v75: i32 = __retval;
// LOWERING-NEXT:     return _v75;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_envz_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut envz: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut length: u64 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let _v0: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     envz = _v0;
// LOWERING-NEXT:     let _v1: u64 = 0;
// LOWERING-NEXT:     length = _v1;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     total = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: *mut i8 = b"one\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i32 = unsafe { envz_add(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v3 as *const i8, _v4 as *const i8) };
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     let _v7: bool = _v5 == _v6;
// LOWERING-NEXT:     let _v8: i32 = _v7 as i32;
// LOWERING-NEXT:     let _v9: i32 = total;
// LOWERING-NEXT:     let _v10: i32 = _v9 + _v8;
// LOWERING-NEXT:     total = _v10;
// LOWERING-NEXT:     let _v11: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v13: i32 = unsafe { envz_add(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v11 as *const i8, _v12 as *const i8) };
// LOWERING-NEXT:     let _v14: i32 = 0;
// LOWERING-NEXT:     let _v15: bool = _v13 == _v14;
// LOWERING-NEXT:     let _v16: i32 = _v15 as i32;
// LOWERING-NEXT:     let _v17: i32 = total;
// LOWERING-NEXT:     let _v18: i32 = _v17 + _v16;
// LOWERING-NEXT:     total = _v18;
// LOWERING-NEXT:     let _v19: *mut i8 = envz;
// LOWERING-NEXT:     let _v20: u64 = length;
// LOWERING-NEXT:     let _v21: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v22: *mut i8 = unsafe { envz_get(_v19 as *const i8, _v20 as usize, _v21 as *const i8) };
// LOWERING-NEXT:     let _v23: *mut i8 = b"one\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v24: i32 = unsafe { strcmp(_v22 as *const i8, _v23 as *const i8) };
// LOWERING-NEXT:     let _v25: i32 = 0;
// LOWERING-NEXT:     let _v26: bool = _v24 == _v25;
// LOWERING-NEXT:     let _v27: i32 = _v26 as i32;
// LOWERING-NEXT:     let _v28: i32 = total;
// LOWERING-NEXT:     let _v29: i32 = _v28 + _v27;
// LOWERING-NEXT:     total = _v29;
// LOWERING-NEXT:     let _v30: *mut i8 = envz;
// LOWERING-NEXT:     let _v31: u64 = length;
// LOWERING-NEXT:     let _v32: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v33: *mut i8 = unsafe { envz_entry(_v30 as *const i8, _v31 as usize, _v32 as *const i8) };
// LOWERING-NEXT:     let _v34: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v35: bool = _v33 != _v34;
// LOWERING-NEXT:     let _v36: i32 = _v35 as i32;
// LOWERING-NEXT:     let _v37: i32 = total;
// LOWERING-NEXT:     let _v38: i32 = _v37 + _v36;
// LOWERING-NEXT:     total = _v38;
// LOWERING-NEXT:     let _v39: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { envz_remove(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v39 as *const i8) };
// LOWERING-NEXT:     let _v40: *mut i8 = envz;
// LOWERING-NEXT:     let _v41: u64 = length;
// LOWERING-NEXT:     let _v42: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v43: *mut i8 = unsafe { envz_get(_v40 as *const i8, _v41 as usize, _v42 as *const i8) };
// LOWERING-NEXT:     let _v44: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v45: bool = _v43 == _v44;
// LOWERING-NEXT:     let _v46: i32 = _v45 as i32;
// LOWERING-NEXT:     let _v47: i32 = total;
// LOWERING-NEXT:     let _v48: i32 = _v47 + _v46;
// LOWERING-NEXT:     total = _v48;
// LOWERING-NEXT:     unsafe { envz_strip(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize) };
// LOWERING-NEXT:     let _v49: u64 = length;
// LOWERING-NEXT:     let _v50: u64 = 0;
// LOWERING-NEXT:     let _v51: bool = _v49 == _v50;
// LOWERING-NEXT:     let _v52: i32 = _v51 as i32;
// LOWERING-NEXT:     let _v53: i32 = total;
// LOWERING-NEXT:     let _v54: i32 = _v53 + _v52;
// LOWERING-NEXT:     total = _v54;
// LOWERING-NEXT:     let _v55: *mut i8 = envz;
// LOWERING-NEXT:     let _v56: *mut core::ffi::c_void = _v55 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v56 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v57: i32 = total;
// LOWERING-NEXT:     __retval = _v57;
// LOWERING-NEXT:     let _v58: i32 = __retval;
// LOWERING-NEXT:     return _v58;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = gnu_string_extensions();
// LOWERING-NEXT:     let _v3: i32 = gnu_argz_extensions();
// LOWERING-NEXT:     let _v4: i32 = gnu_envz_extensions();
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4) };
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     __retval = _v6;
// LOWERING-NEXT:     let _v7: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v7 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: struct __SlateAllocaFrame0(i32, *mut i8, *mut i8, *mut i8, [i8; 6], [i8; 4], [i8; 5], aligned::Aligned<aligned::A16, [i8; 16]>, i32);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn mempcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strsep(_0: *mut *mut i8, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn strlen(_0: *const i8) -> usize;
// REWRITES-NEXT:     fn memfrob(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// REWRITES-NEXT:     fn memrchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn rawmemchr(_0: *const core::ffi::c_void, _1: i32) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strchrnul(_0: *const i8, _1: i32) -> *mut i8;
// REWRITES-NEXT:     fn strcasestr(_0: *const i8, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn strverscmp(_0: *const i8, _1: *const i8) -> i32;
// REWRITES-NEXT:     fn strerrorname_np(_0: i32) -> *const i8;
// REWRITES-NEXT:     fn strerrordesc_np(_0: i32) -> *const i8;
// REWRITES-NEXT:     fn argz_create_sep(_0: *const i8, _1: i32, _2: *mut *mut i8, _3: *mut usize) -> i32;
// REWRITES-NEXT:     fn argz_count(_0: *const i8, _1: usize) -> usize;
// REWRITES-NEXT:     fn argz_extract(_0: *const i8, _1: usize, _2: *mut *mut i8);
// REWRITES-NEXT:     fn argz_add(_0: *mut *mut i8, _1: *mut usize, _2: *const i8) -> i32;
// REWRITES-NEXT:     fn argz_replace(_0: *mut *mut i8, _1: *mut usize, _2: *const i8, _3: *const i8, _4: *mut u32) -> i32;
// REWRITES-NEXT:     fn argz_stringify(_0: *mut i8, _1: usize, _2: i32);
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn envz_add(_0: *mut *mut i8, _1: *mut usize, _2: *const i8, _3: *const i8) -> i32;
// REWRITES-NEXT:     fn envz_get(_0: *const i8, _1: usize, _2: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn envz_entry(_0: *const i8, _1: usize, _2: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn envz_remove(_0: *mut *mut i8, _1: *mut usize, _2: *const i8);
// REWRITES-NEXT:     fn envz_strip(_0: *mut *mut i8, _1: *mut usize);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_string_extensions() -> i32 {
// REWRITES-NEXT: let mut __slate_alloca_frame0: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, std::ptr::null_mut(), std::ptr::null_mut(), std::ptr::null_mut(), [0; 6], [0; 4], [0; 5], aligned::Aligned([0; 16]), 0);
// REWRITES-NEXT: *__slate_alloca_frame0.7 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: __slate_alloca_frame0.6 = [97, 98, 99, 97, 0];
// REWRITES-NEXT: __slate_alloca_frame0.5 = [103, 110, 117, 0];
// REWRITES-NEXT: __slate_alloca_frame0.4 = [97, 58, 58, 98, 99, 0];
// REWRITES-NEXT: let _v0: *mut i8 = __slate_alloca_frame0.4.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: __slate_alloca_frame0.3 = _v0;
// REWRITES-NEXT: let _v1: *mut i8 = __slate_alloca_frame0.7.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v3: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: u64 = 6;
// REWRITES-NEXT: let _v6: *mut core::ffi::c_void = unsafe { mempcpy(_v2 as *mut core::ffi::c_void, (_v3 as *mut core::ffi::c_void) as *const core::ffi::c_void, _v5 as usize) };
// REWRITES-NEXT: __slate_alloca_frame0.1 = _v6 as *mut i8;
// REWRITES-NEXT: __slate_alloca_frame0.0 = 0;
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let _v9: *mut i8 = b":\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let _v10: *mut i8 = unsafe { strsep(std::ptr::addr_of_mut!(__slate_alloca_frame0.3) as *mut *mut i8, _v9 as *const i8) };
// REWRITES-NEXT:         __slate_alloca_frame0.2 = _v10;
// REWRITES-NEXT:         let _v11: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         if !(_v10 != _v11) {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let _v14: i32 = 10;
// REWRITES-NEXT:                     let _v15: i32 = __slate_alloca_frame0.0 * _v14;
// REWRITES-NEXT:                     let _v16: *mut i8 = __slate_alloca_frame0.2;
// REWRITES-NEXT:                     let _v17: u64 = (unsafe { strlen(_v16 as *const i8) }) as u64;
// REWRITES-NEXT:                     __slate_alloca_frame0.0 = _v15 + (_v17 as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v20: *mut i8 = __slate_alloca_frame0.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: u64 = 3;
// REWRITES-NEXT: let _v23: *mut core::ffi::c_void = unsafe { memfrob((_v20 as *mut core::ffi::c_void) as *mut core::ffi::c_void, _v22 as usize) };
// REWRITES-NEXT: let _v24: *mut i8 = __slate_alloca_frame0.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v26: u64 = 3;
// REWRITES-NEXT: let _v27: *mut core::ffi::c_void = unsafe { memfrob((_v24 as *mut core::ffi::c_void) as *mut core::ffi::c_void, _v26 as usize) };
// REWRITES-NEXT: let _v28: *mut i8 = __slate_alloca_frame0.1;
// REWRITES-NEXT: let _v29: *mut i8 = __slate_alloca_frame0.7.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v30: i64 = unsafe { _v28.offset_from(_v29) as i64 };
// REWRITES-NEXT: let _v31: i32 = _v30 as i32;
// REWRITES-NEXT: let _v32: *mut i8 = __slate_alloca_frame0.7.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v33: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v34: i32 = unsafe { strcmp(_v32 as *const i8, _v33 as *const i8) };
// REWRITES-NEXT: let _v35: i32 = 0;
// REWRITES-NEXT: let _v39: i64 = (_v31 + ((_v34 == _v35) as i32)) as i64;
// REWRITES-NEXT: let _v40: *mut i8 = __slate_alloca_frame0.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v42: i32 = 97;
// REWRITES-NEXT: let _v43: u64 = 4;
// REWRITES-NEXT: let _v44: *mut core::ffi::c_void = unsafe { memrchr((_v40 as *mut core::ffi::c_void) as *const core::ffi::c_void, _v42 as i32, _v43 as usize) };
// REWRITES-NEXT: let _v45: *mut i8 = _v44 as *mut i8;
// REWRITES-NEXT: let _v46: *mut i8 = __slate_alloca_frame0.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v47: i64 = unsafe { _v45.offset_from(_v46) as i64 };
// REWRITES-NEXT: let _v48: i64 = _v39 + _v47;
// REWRITES-NEXT: let _v49: *mut i8 = __slate_alloca_frame0.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v51: i32 = 99;
// REWRITES-NEXT: let _v52: *mut core::ffi::c_void = unsafe { rawmemchr((_v49 as *mut core::ffi::c_void) as *const core::ffi::c_void, _v51 as i32) };
// REWRITES-NEXT: let _v53: *mut i8 = _v52 as *mut i8;
// REWRITES-NEXT: let _v54: *mut i8 = __slate_alloca_frame0.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v55: i64 = unsafe { _v53.offset_from(_v54) as i64 };
// REWRITES-NEXT: let _v56: i64 = _v48 + _v55;
// REWRITES-NEXT: let _v57: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v58: i32 = 122;
// REWRITES-NEXT: let _v59: *mut i8 = unsafe { strchrnul(_v57 as *const i8, _v58 as i32) };
// REWRITES-NEXT: let _v60: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v61: i64 = unsafe { _v59.offset_from(_v60) as i64 };
// REWRITES-NEXT: let _v62: i64 = _v56 + _v61;
// REWRITES-NEXT: let _v63: *mut i8 = b"GNU Library\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v64: *mut i8 = b"library\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v65: *mut i8 = unsafe { strcasestr(_v63 as *const i8, _v64 as *const i8) };
// REWRITES-NEXT: let _v66: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let _v70: i64 = _v62 + (((_v65 != _v66) as i32) as i64);
// REWRITES-NEXT: let _v71: *mut i8 = b"release-2\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v72: *mut i8 = b"release-10\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v73: i32 = unsafe { strverscmp(_v71 as *const i8, _v72 as *const i8) };
// REWRITES-NEXT: let _v74: i32 = 0;
// REWRITES-NEXT: let _v78: i64 = _v70 + (((_v73 < _v74) as i32) as i64);
// REWRITES-NEXT: let _v79: *mut i8 = __slate_alloca_frame0.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v80: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v81: i32 = unsafe { strcmp(_v79 as *const i8, _v80 as *const i8) };
// REWRITES-NEXT: let _v82: i32 = 0;
// REWRITES-NEXT: let _v89: i64 = _v78 + (((_v81 == _v82) as i32) as i64) + (__slate_alloca_frame0.0 as i64);
// REWRITES-NEXT: let _v90: i32 = 22;
// REWRITES-NEXT: let _v91: *mut i8 = (unsafe { strerrorname_np(_v90 as i32) }) as *mut i8;
// REWRITES-NEXT: let _v92: *mut i8 = b"EINVAL\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v93: i32 = unsafe { strcmp(_v91 as *const i8, _v92 as *const i8) };
// REWRITES-NEXT: let _v94: i32 = 0;
// REWRITES-NEXT: let _v98: i64 = _v89 + (((_v93 == _v94) as i32) as i64);
// REWRITES-NEXT: let _v99: i32 = 22;
// REWRITES-NEXT: let _v100: *mut i8 = (unsafe { strerrordesc_np(_v99 as i32) }) as *mut i8;
// REWRITES-NEXT: let _v101: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: __slate_alloca_frame0.8 = (_v98 + (((_v100 != _v101) as i32) as i64)) as i32;
// REWRITES-NEXT: return __slate_alloca_frame0.8;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_argz_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut argz: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut length: u64 = 0;
// REWRITES-NEXT: let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 6]> = aligned::Aligned([std::ptr::null_mut(); 6]);
// REWRITES-NEXT: let mut replacements: u32 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: argz = std::ptr::null_mut();
// REWRITES-NEXT: length = 0;
// REWRITES-NEXT: replacements = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: let _v4: *mut i8 = b"one:two:three\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = 58;
// REWRITES-NEXT: let _v6: i32 = unsafe { argz_create_sep(_v4 as *const i8, _v5 as i32, std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize) };
// REWRITES-NEXT: let _v7: i32 = 0;
// REWRITES-NEXT: total = total + ((_v6 == _v7) as i32);
// REWRITES-NEXT: let _v14: u64 = (unsafe { argz_count(argz as *const i8, length as usize) }) as u64;
// REWRITES-NEXT: let _v15: u64 = 3;
// REWRITES-NEXT: total = total + ((_v14 == _v15) as i32);
// REWRITES-NEXT: let _v20: *mut i8 = argz;
// REWRITES-NEXT: let _v21: u64 = length;
// REWRITES-NEXT: let _v22: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-NEXT: unsafe { argz_extract(_v20 as *const i8, _v21 as usize, _v22 as *mut *mut i8) };
// REWRITES-NEXT: let _v23: i64 = 1;
// REWRITES-NEXT: let _v24: *mut i8 = arguments[(_v23 as usize)];
// REWRITES-NEXT: let _v25: *mut i8 = b"two\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v26: i32 = unsafe { strcmp(_v24 as *const i8, _v25 as *const i8) };
// REWRITES-NEXT: let _v27: i32 = 0;
// REWRITES-NEXT: total = total + ((_v26 == _v27) as i32);
// REWRITES-NEXT: let _v32: *mut i8 = b"four\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v33: i32 = unsafe { argz_add(std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v32 as *const i8) };
// REWRITES-NEXT: let _v34: i32 = 0;
// REWRITES-NEXT: total = total + ((_v33 == _v34) as i32);
// REWRITES-NEXT: let _v39: *mut i8 = b"three\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v40: *mut i8 = b"THREE\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v41: i32 = unsafe { argz_replace(std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v39 as *const i8, _v40 as *const i8, std::ptr::addr_of_mut!(replacements) as *mut u32) };
// REWRITES-NEXT: let _v42: i32 = 0;
// REWRITES-NEXT: total = total + ((_v41 == _v42) as i32);
// REWRITES-NEXT: let _v48: u32 = 1;
// REWRITES-NEXT: total = total + ((replacements == _v48) as i32);
// REWRITES-NEXT: let _v55: u64 = (unsafe { argz_count(argz as *const i8, length as usize) }) as u64;
// REWRITES-NEXT: let _v56: u64 = 4;
// REWRITES-NEXT: total = total + ((_v55 == _v56) as i32);
// REWRITES-NEXT: let _v63: i32 = 44;
// REWRITES-NEXT: unsafe { argz_stringify(argz as *mut i8, length as usize, _v63 as i32) };
// REWRITES-NEXT: let _v64: *mut i8 = argz;
// REWRITES-NEXT: let _v65: *mut i8 = b"one,two,THREE,four\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v66: i32 = unsafe { strcmp(_v64 as *const i8, _v65 as *const i8) };
// REWRITES-NEXT: let _v67: i32 = 0;
// REWRITES-NEXT: total = total + ((_v66 == _v67) as i32);
// REWRITES-NEXT: unsafe { free((argz as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_envz_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut envz: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut length: u64 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: envz = std::ptr::null_mut();
// REWRITES-NEXT: length = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: let _v3: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: *mut i8 = b"one\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = unsafe { envz_add(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v3 as *const i8, _v4 as *const i8) };
// REWRITES-NEXT: let _v6: i32 = 0;
// REWRITES-NEXT: total = total + ((_v5 == _v6) as i32);
// REWRITES-NEXT: let _v11: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let _v13: i32 = unsafe { envz_add(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v11 as *const i8, _v12 as *const i8) };
// REWRITES-NEXT: let _v14: i32 = 0;
// REWRITES-NEXT: total = total + ((_v13 == _v14) as i32);
// REWRITES-NEXT: let _v19: *mut i8 = envz;
// REWRITES-NEXT: let _v20: u64 = length;
// REWRITES-NEXT: let _v21: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: *mut i8 = unsafe { envz_get(_v19 as *const i8, _v20 as usize, _v21 as *const i8) };
// REWRITES-NEXT: let _v23: *mut i8 = b"one\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v24: i32 = unsafe { strcmp(_v22 as *const i8, _v23 as *const i8) };
// REWRITES-NEXT: let _v25: i32 = 0;
// REWRITES-NEXT: total = total + ((_v24 == _v25) as i32);
// REWRITES-NEXT: let _v30: *mut i8 = envz;
// REWRITES-NEXT: let _v31: u64 = length;
// REWRITES-NEXT: let _v32: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v33: *mut i8 = unsafe { envz_entry(_v30 as *const i8, _v31 as usize, _v32 as *const i8) };
// REWRITES-NEXT: let _v34: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v33 != _v34) as i32);
// REWRITES-NEXT: let _v39: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { envz_remove(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, _v39 as *const i8) };
// REWRITES-NEXT: let _v40: *mut i8 = envz;
// REWRITES-NEXT: let _v41: u64 = length;
// REWRITES-NEXT: let _v42: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v43: *mut i8 = unsafe { envz_get(_v40 as *const i8, _v41 as usize, _v42 as *const i8) };
// REWRITES-NEXT: let _v44: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v43 == _v44) as i32);
// REWRITES-NEXT: unsafe { envz_strip(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize) };
// REWRITES-NEXT: let _v50: u64 = 0;
// REWRITES-NEXT: total = total + ((length == _v50) as i32);
// REWRITES-NEXT: unsafe { free((envz as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = gnu_string_extensions();
// REWRITES-NEXT: let _v3: i32 = gnu_argz_extensions();
// REWRITES-NEXT: let _v4: i32 = gnu_envz_extensions();
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
