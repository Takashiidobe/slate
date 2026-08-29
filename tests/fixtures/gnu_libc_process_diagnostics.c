#define _GNU_SOURCE
#include <error.h>
#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>

static void handle_exit(int status, void *arg) {
  int *captured = (int *)arg;
  *captured = status;
  printf("on_exit:%d\n", status);
}

int main(void) {
  int captured = -1;
  on_exit(handle_exit, &captured);

  int   mcheck_enabled = mcheck(NULL) == 0;
  void *block          = malloc(16);
  enum mcheck_status probe = mprobe(block);
  printf("mcheck:%d %d\n", mcheck_enabled, probe == MCHECK_OK);
  free(block);

  error_one_per_line = 0;
  error(0, 0, "first message");
  error(0, 0, "second message");
  printf("count_after_two:%u\n", error_message_count);

  error_one_per_line = 1;
  error_at_line(0, 0, "sample.c", 42, "deduped message");
  error_at_line(0, 0, "sample.c", 42, "deduped message");
  printf("count_after_dedup:%u\n", error_message_count);

  error_at_line(0, 0, "sample.c", 43, "different line");
  printf("count_after_new_line:%u\n", error_message_count);

  error(5, 0, "fatal message");
  printf("unreachable\n");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum mcheck_status {
// LOWERING-NEXT:     MCHECK_DISABLED = -1,
// LOWERING-NEXT:     MCHECK_OK = 0,
// LOWERING-NEXT:     MCHECK_HEAD = 1,
// LOWERING-NEXT:     MCHECK_TAIL = 2,
// LOWERING-NEXT:     MCHECK_FREE = 3,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     static mut error_message_count: u32;
// LOWERING-NEXT:     static mut error_one_per_line: i32;
// LOWERING-NEXT:     fn on_exit(_0: Option<unsafe extern "C" fn(i32, *mut core::ffi::c_void)>, _1: *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn mcheck(_0: Option<unsafe extern "C" fn(i32)>) -> i32;
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn mprobe(_0: *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn error(_0: i32, _1: i32, _2: *const i8, ...);
// LOWERING-NEXT:     fn error_at_line(_0: i32, _1: i32, _2: *const i8, _3: u32, _4: *const i8, ...);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn handle_exit(arg0: i32, arg1: *mut core::ffi::c_void) {
// LOWERING-NEXT:     let mut status: i32 = 0;
// LOWERING-NEXT:     let mut arg: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut captured: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     status = arg0;
// LOWERING-NEXT:     arg = arg1;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = arg;
// LOWERING-NEXT:     let _v1: *mut i32 = _v0 as *mut i32;
// LOWERING-NEXT:     captured = _v1;
// LOWERING-NEXT:     let _v2: i32 = status;
// LOWERING-NEXT:     let _v3: *mut i32 = captured;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v3 = _v2;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v4: *mut i8 = b"on_exit:%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i32 = status;
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v4 as *const i8, _v5) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut captured: i32 = 0;
// LOWERING-NEXT:     let mut mcheck_enabled: i32 = 0;
// LOWERING-NEXT:     let mut block: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut probe: aligned::Aligned<aligned::A4, mcheck_status> = aligned::Aligned(mcheck_status::MCHECK_DISABLED);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = -1;
// LOWERING-NEXT:     captured = _v1;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = std::ptr::addr_of_mut!(captured) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v3: i32 = unsafe { on_exit(Some(handle_exit), _v2 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v4: Option<unsafe extern "C" fn(i32)> = None;
// LOWERING-NEXT:     let _v5: i32 = unsafe { mcheck(_v4) };
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     let _v7: bool = _v5 == _v6;
// LOWERING-NEXT:     let _v8: i32 = _v7 as i32;
// LOWERING-NEXT:     mcheck_enabled = _v8;
// LOWERING-NEXT:     let _v9: u64 = 16;
// LOWERING-NEXT:     let _v10: *mut core::ffi::c_void = unsafe { malloc(_v9 as usize) };
// LOWERING-NEXT:     block = _v10;
// LOWERING-NEXT:     let _v11: *mut core::ffi::c_void = block;
// LOWERING-NEXT:     let _v12: i32 = unsafe { mprobe(_v11 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     *probe = unsafe { std::mem::transmute(_v12) };
// LOWERING-NEXT:     let _v13: *mut i8 = b"mcheck:%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: i32 = mcheck_enabled;
// LOWERING-NEXT:     let _v15: i32 = *probe as i32;
// LOWERING-NEXT:     let _v16: i32 = mcheck_status::MCHECK_OK as i32;
// LOWERING-NEXT:     let _v17: bool = _v15 == _v16;
// LOWERING-NEXT:     let _v18: i32 = _v17 as i32;
// LOWERING-NEXT:     let _v19: i32 = unsafe { printf(_v13 as *const i8, _v14, _v18) };
// LOWERING-NEXT:     let _v20: *mut core::ffi::c_void = block;
// LOWERING-NEXT:     unsafe { free(_v20 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v21: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error_one_per_line = _v21;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v22: i32 = 0;
// LOWERING-NEXT:     let _v23: i32 = 0;
// LOWERING-NEXT:     let _v24: *mut i8 = b"first message\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { error(_v22 as i32, _v23 as i32, _v24 as *const i8) };
// LOWERING-NEXT:     let _v25: i32 = 0;
// LOWERING-NEXT:     let _v26: i32 = 0;
// LOWERING-NEXT:     let _v27: *mut i8 = b"second message\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { error(_v25 as i32, _v26 as i32, _v27 as *const i8) };
// LOWERING-NEXT:     let _v28: *mut i8 = b"count_after_two:%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v29: u32 = unsafe { error_message_count };
// LOWERING-NEXT:     let _v30: i32 = unsafe { printf(_v28 as *const i8, _v29) };
// LOWERING-NEXT:     let _v31: i32 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error_one_per_line = _v31;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v32: i32 = 0;
// LOWERING-NEXT:     let _v33: i32 = 0;
// LOWERING-NEXT:     let _v34: *mut i8 = b"sample.c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v35: u32 = 42;
// LOWERING-NEXT:     let _v36: *mut i8 = b"deduped message\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { error_at_line(_v32 as i32, _v33 as i32, _v34 as *const i8, _v35 as u32, _v36 as *const i8) };
// LOWERING-NEXT:     let _v37: i32 = 0;
// LOWERING-NEXT:     let _v38: i32 = 0;
// LOWERING-NEXT:     let _v39: *mut i8 = b"sample.c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v40: u32 = 42;
// LOWERING-NEXT:     let _v41: *mut i8 = b"deduped message\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { error_at_line(_v37 as i32, _v38 as i32, _v39 as *const i8, _v40 as u32, _v41 as *const i8) };
// LOWERING-NEXT:     let _v42: *mut i8 = b"count_after_dedup:%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v43: u32 = unsafe { error_message_count };
// LOWERING-NEXT:     let _v44: i32 = unsafe { printf(_v42 as *const i8, _v43) };
// LOWERING-NEXT:     let _v45: i32 = 0;
// LOWERING-NEXT:     let _v46: i32 = 0;
// LOWERING-NEXT:     let _v47: *mut i8 = b"sample.c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v48: u32 = 43;
// LOWERING-NEXT:     let _v49: *mut i8 = b"different line\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { error_at_line(_v45 as i32, _v46 as i32, _v47 as *const i8, _v48 as u32, _v49 as *const i8) };
// LOWERING-NEXT:     let _v50: *mut i8 = b"count_after_new_line:%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v51: u32 = unsafe { error_message_count };
// LOWERING-NEXT:     let _v52: i32 = unsafe { printf(_v50 as *const i8, _v51) };
// LOWERING-NEXT:     let _v53: i32 = 5;
// LOWERING-NEXT:     let _v54: i32 = 0;
// LOWERING-NEXT:     let _v55: *mut i8 = b"fatal message\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { error(_v53 as i32, _v54 as i32, _v55 as *const i8) };
// LOWERING-NEXT:     let _v56: *mut i8 = b"unreachable\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v57: i32 = unsafe { printf(_v56 as *const i8) };
// LOWERING-NEXT:     let _v58: i32 = 0;
// LOWERING-NEXT:     __retval = _v58;
// LOWERING-NEXT:     let _v59: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v59 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum mcheck_status {
// REWRITES-NEXT:     MCHECK_DISABLED = -1,
// REWRITES-NEXT:     MCHECK_OK = 0,
// REWRITES-NEXT:     MCHECK_HEAD = 1,
// REWRITES-NEXT:     MCHECK_TAIL = 2,
// REWRITES-NEXT:     MCHECK_FREE = 3,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     static mut error_message_count: u32;
// REWRITES-NEXT:     static mut error_one_per_line: i32;
// REWRITES-NEXT:     fn on_exit(_0: Option<unsafe extern "C" fn(i32, *mut core::ffi::c_void)>, _1: *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn mcheck(_0: Option<unsafe extern "C" fn(i32)>) -> i32;
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn mprobe(_0: *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn error(_0: i32, _1: i32, _2: *const i8, ...);
// REWRITES-NEXT:     fn error_at_line(_0: i32, _1: i32, _2: *const i8, _3: u32, _4: *const i8, ...);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn handle_exit(arg0: i32, arg1: *mut core::ffi::c_void) {
// REWRITES-NEXT: let mut status: i32 = arg0;
// REWRITES-NEXT: let mut arg: *mut core::ffi::c_void = arg1;
// REWRITES-NEXT: let mut captured: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: captured = arg as *mut i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *captured = status;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v4: *mut i8 = b"on_exit:%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v4 as *const i8, status) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut captured: i32 = 0;
// REWRITES-NEXT: let mut mcheck_enabled: i32 = 0;
// REWRITES-NEXT: let mut block: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut probe: aligned::Aligned<aligned::A4, mcheck_status> = aligned::Aligned(mcheck_status::MCHECK_DISABLED);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: captured = -1;
// REWRITES-NEXT: let _v2: *mut core::ffi::c_void = std::ptr::addr_of_mut!(captured) as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v3: i32 = unsafe { on_exit(Some(handle_exit), _v2 as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v4: Option<unsafe extern "C" fn(i32)> = None;
// REWRITES-NEXT: let _v5: i32 = unsafe { mcheck(_v4) };
// REWRITES-NEXT: let _v6: i32 = 0;
// REWRITES-NEXT: mcheck_enabled = (_v5 == _v6) as i32;
// REWRITES-NEXT: let _v9: u64 = 16;
// REWRITES-NEXT: block = unsafe { malloc(_v9 as usize) };
// REWRITES-NEXT: let _v12: i32 = unsafe { mprobe(block as *mut core::ffi::c_void) };
// REWRITES-NEXT: *probe = unsafe { std::mem::transmute(_v12) };
// REWRITES-NEXT: let _v13: *mut i8 = b"mcheck:%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: i32 = mcheck_enabled;
// REWRITES-NEXT: let _v15: i32 = *probe as i32;
// REWRITES-NEXT: let _v16: i32 = mcheck_status::MCHECK_OK as i32;
// REWRITES-NEXT: let _v19: i32 = unsafe { printf(_v13 as *const i8, _v14, (_v15 == _v16) as i32) };
// REWRITES-NEXT: unsafe { free(block as *mut core::ffi::c_void) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         error_one_per_line = 0;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v22: i32 = 0;
// REWRITES-NEXT: let _v23: i32 = 0;
// REWRITES-NEXT: let _v24: *mut i8 = b"first message\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { error(_v22 as i32, _v23 as i32, _v24 as *const i8) };
// REWRITES-NEXT: let _v25: i32 = 0;
// REWRITES-NEXT: let _v26: i32 = 0;
// REWRITES-NEXT: let _v27: *mut i8 = b"second message\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { error(_v25 as i32, _v26 as i32, _v27 as *const i8) };
// REWRITES-NEXT: let _v28: *mut i8 = b"count_after_two:%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v30: i32 = unsafe { printf(_v28 as *const i8, unsafe { error_message_count }) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         error_one_per_line = 1;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v32: i32 = 0;
// REWRITES-NEXT: let _v33: i32 = 0;
// REWRITES-NEXT: let _v34: *mut i8 = b"sample.c\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v35: u32 = 42;
// REWRITES-NEXT: let _v36: *mut i8 = b"deduped message\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { error_at_line(_v32 as i32, _v33 as i32, _v34 as *const i8, _v35 as u32, _v36 as *const i8) };
// REWRITES-NEXT: let _v37: i32 = 0;
// REWRITES-NEXT: let _v38: i32 = 0;
// REWRITES-NEXT: let _v39: *mut i8 = b"sample.c\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v40: u32 = 42;
// REWRITES-NEXT: let _v41: *mut i8 = b"deduped message\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { error_at_line(_v37 as i32, _v38 as i32, _v39 as *const i8, _v40 as u32, _v41 as *const i8) };
// REWRITES-NEXT: let _v42: *mut i8 = b"count_after_dedup:%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v44: i32 = unsafe { printf(_v42 as *const i8, unsafe { error_message_count }) };
// REWRITES-NEXT: let _v45: i32 = 0;
// REWRITES-NEXT: let _v46: i32 = 0;
// REWRITES-NEXT: let _v47: *mut i8 = b"sample.c\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v48: u32 = 43;
// REWRITES-NEXT: let _v49: *mut i8 = b"different line\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { error_at_line(_v45 as i32, _v46 as i32, _v47 as *const i8, _v48 as u32, _v49 as *const i8) };
// REWRITES-NEXT: let _v50: *mut i8 = b"count_after_new_line:%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v52: i32 = unsafe { printf(_v50 as *const i8, unsafe { error_message_count }) };
// REWRITES-NEXT: let _v53: i32 = 5;
// REWRITES-NEXT: let _v54: i32 = 0;
// REWRITES-NEXT: let _v55: *mut i8 = b"fatal message\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { error(_v53 as i32, _v54 as i32, _v55 as *const i8) };
// REWRITES-NEXT: let _v56: *mut i8 = b"unreachable\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v57: i32 = unsafe { printf(_v56 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
