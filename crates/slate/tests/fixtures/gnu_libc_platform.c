#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <execinfo.h>
#include <fnmatch.h>
#include <glob.h>
#include <gnu/libc-version.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/random.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

static int gnu_environment_extensions(void) {
  char *directory;
  char *canonical;
  char  current[4096];
  int   total = 0;

  total += setenv("SLATE_GNU_LIBC_VALUE", "ready", 1) == 0;
  total += strcmp(secure_getenv("SLATE_GNU_LIBC_VALUE"), "ready") == 0;
  total += unsetenv("SLATE_GNU_LIBC_VALUE") == 0;

  directory  = get_current_dir_name();
  canonical  = canonicalize_file_name(".");
  total     += getcwd(current, sizeof(current)) != NULL;
  total     += directory != NULL && strcmp(directory, current) == 0;
  total     += canonical != NULL && strcmp(canonical, current) == 0;
  free(directory);
  free(canonical);

  total += strcmp(strdupa("slate"), "slate") == 0;
  total += strcmp(strndupa("slate-truncated", 5), "slate") == 0;
  return total;
}

static int gnu_time_extensions(void) {
  struct tm epoch = {};
  struct tm local = {};
  time_t    timestamp;
  int       total = 0;

  epoch.tm_year  = 70;
  epoch.tm_mon   = 0;
  epoch.tm_mday  = 1;
  timestamp      = timegm(&epoch);
  total         += timestamp == 0;

  local.tm_year  = 70;
  local.tm_mon   = 0;
  local.tm_mday  = 2;
  total         += timelocal(&local) != (time_t)-1;
  return total;
}

static int gnu_pattern_extensions(void) {
  regex_t     expression = {};
  glob_t      paths      = {};
  const char *error;
  int         total = 0;

  total += fnmatch("file-+(one|two).c", "file-two.c", FNM_EXTMATCH) == 0;
  re_set_syntax(RE_SYNTAX_POSIX_EXTENDED);
  error  = re_compile_pattern("sl(a|e)te", 9, &expression);
  total += error == NULL;
  total += re_match(&expression, "slate", 5, 0, NULL) == 5;
  regfree(&expression);

  total += glob("/dev/{null,zero}", GLOB_BRACE, NULL, &paths) == 0;
  total += paths.gl_pathc == 2;
  total += strcmp(paths.gl_pathv[0], "/dev/null") == 0;
  total += strcmp(paths.gl_pathv[1], "/dev/zero") == 0;
  globfree(&paths);
  return total;
}

static int gnu_runtime_extensions(void) {
  unsigned char random_bytes[8];
  void         *frames[8];
  Dl_info       information = {};
  long          page_size   = sysconf(_SC_PAGESIZE);
  int           total       = 0;

  total += getauxval(AT_PAGESZ) == (unsigned long)page_size;
  total += gettid() == (pid_t)syscall(SYS_gettid);
  total += getentropy(random_bytes, sizeof(random_bytes)) == 0;
  total += arc4random_uniform(1) == 0;
  total += get_nprocs() > 0;
  total += get_phys_pages() > 0;
  total += backtrace(frames, 8) > 0;
  total += dladdr((void *)&gnu_runtime_extensions, &information) != 0;
  total += information.dli_fname != NULL;
  total += gnu_get_libc_version()[0] != '\0';
  total += program_invocation_name != NULL;
  total += program_invocation_short_name != NULL;
  return total;
}

int main(void) {
  printf("%d %d %d\n", gnu_environment_extensions(), gnu_time_extensions(),
         gnu_pattern_extensions() + gnu_runtime_extensions());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Dl_info {
// LOWERING-NEXT:     dli_fname: *mut i8,
// LOWERING-NEXT:     dli_fbase: *mut core::ffi::c_void,
// LOWERING-NEXT:     dli_sname: *mut i8,
// LOWERING-NEXT:     dli_saddr: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct glob_t {
// LOWERING-NEXT:     gl_pathc: u64,
// LOWERING-NEXT:     gl_pathv: *mut *mut i8,
// LOWERING-NEXT:     gl_offs: u64,
// LOWERING-NEXT:     __reserved1: i32,
// LOWERING-NEXT:     __reserved2: [*mut core::ffi::c_void; 5],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct re_pattern_buffer {
// LOWERING-NEXT:     re_nsub: u64,
// LOWERING-NEXT:     __opaque: *mut core::ffi::c_void,
// LOWERING-NEXT:     __padding: [*mut core::ffi::c_void; 4],
// LOWERING-NEXT:     __nsub2: u64,
// LOWERING-NEXT:     __padding2: i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct tm {
// LOWERING-NEXT:     tm_sec: i32,
// LOWERING-NEXT:     tm_min: i32,
// LOWERING-NEXT:     tm_hour: i32,
// LOWERING-NEXT:     tm_mday: i32,
// LOWERING-NEXT:     tm_mon: i32,
// LOWERING-NEXT:     tm_year: i32,
// LOWERING-NEXT:     tm_wday: i32,
// LOWERING-NEXT:     tm_yday: i32,
// LOWERING-NEXT:     tm_isdst: i32,
// LOWERING-NEXT:     tm_gmtoff: i64,
// LOWERING-NEXT:     tm_zone: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     static mut program_invocation_name: *mut i8;
// LOWERING-NEXT:     static mut program_invocation_short_name: *mut i8;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn setenv(_0: *const i8, _1: *const i8, _2: i32) -> i32;
// LOWERING-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// LOWERING-NEXT:     fn secure_getenv(_0: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn unsetenv(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn get_current_dir_name() -> *mut i8;
// LOWERING-NEXT:     fn canonicalize_file_name(_0: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn getcwd(_0: *mut i8, _1: usize) -> *mut i8;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn strcpy(_0: *mut i8, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strnlen(_0: *const i8, _1: usize) -> usize;
// LOWERING-NEXT:     fn timegm(_0: *mut tm) -> i64;
// LOWERING-NEXT:     fn timelocal(_0: *mut tm) -> i64;
// LOWERING-NEXT:     fn fnmatch(_0: *const i8, _1: *const i8, _2: i32) -> i32;
// LOWERING-NEXT:     fn re_set_syntax(_0: u64) -> u64;
// LOWERING-NEXT:     fn re_compile_pattern(_0: *const i8, _1: usize, _2: *mut re_pattern_buffer) -> *const i8;
// LOWERING-NEXT:     fn re_match(_0: *mut re_pattern_buffer, _1: *const i8, _2: usize, _3: i64, _4: *mut core::ffi::c_void) -> i64;
// LOWERING-NEXT:     fn regfree(_0: *mut re_pattern_buffer);
// LOWERING-NEXT:     fn glob(_0: *const i8, _1: i32, _2: Option<unsafe extern "C" fn(*mut i8, i32) -> i32>, _3: *mut glob_t) -> i32;
// LOWERING-NEXT:     fn globfree(_0: *mut glob_t);
// LOWERING-NEXT:     fn sysconf(_0: i32) -> i64;
// LOWERING-NEXT:     fn getauxval(_0: u64) -> u64;
// LOWERING-NEXT:     fn gettid() -> i32;
// LOWERING-NEXT:     fn syscall(_0: i64, ...) -> i64;
// LOWERING-NEXT:     fn getentropy(_0: *mut core::ffi::c_void, _1: usize) -> i32;
// LOWERING-NEXT:     fn arc4random_uniform(_0: u32) -> u32;
// LOWERING-NEXT:     fn get_nprocs() -> i32;
// LOWERING-NEXT:     fn get_phys_pages() -> i64;
// LOWERING-NEXT:     fn backtrace(_0: *mut *mut core::ffi::c_void, _1: i32) -> i32;
// LOWERING-NEXT:     fn dladdr(_0: *const core::ffi::c_void, _1: *mut Dl_info) -> i32;
// LOWERING-NEXT:     fn gnu_get_libc_version() -> *const i8;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn __slate_strndupa_finish(arg0: *mut core::ffi::c_void, arg1: *mut i8, arg2: u64) -> *mut i8 {
// LOWERING-NEXT:     let mut buf: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut s: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: u64 = 0;
// LOWERING-NEXT:     let mut __retval: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut out: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     buf = arg0;
// LOWERING-NEXT:     s = arg1;
// LOWERING-NEXT:     len = arg2;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = buf;
// LOWERING-NEXT:     let _v1: *mut i8 = _v0 as *mut i8;
// LOWERING-NEXT:     out = _v1;
// LOWERING-NEXT:     let _v2: *mut i8 = out;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = _v2 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v4: *mut i8 = s;
// LOWERING-NEXT:     let _v5: *mut core::ffi::c_void = _v4 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v6: u64 = len;
// LOWERING-NEXT:     let _v7: *mut core::ffi::c_void = unsafe { memcpy(_v3 as *mut core::ffi::c_void, _v5 as *const core::ffi::c_void, _v6 as usize) };
// LOWERING-NEXT:     let _v8: i8 = 0;
// LOWERING-NEXT:     let _v9: u64 = len;
// LOWERING-NEXT:     let _v10: *mut i8 = out;
// LOWERING-NEXT:     let _v11: *mut i8 = unsafe { _v10.add(_v9 as usize) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v11 = _v8;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: *mut i8 = out;
// LOWERING-NEXT:     __retval = _v12;
// LOWERING-NEXT:     let _v13: *mut i8 = __retval;
// LOWERING-NEXT:     return _v13;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_environment_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut directory: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut canonical: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut current: aligned::Aligned<aligned::A16, [i8; 4096]> = aligned::Aligned([0; 4096]);
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     total = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: i32 = 1;
// LOWERING-NEXT:     let _v4: i32 = unsafe { setenv(_v1 as *const i8, _v2 as *const i8, _v3 as i32) };
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     let _v6: bool = _v4 == _v5;
// LOWERING-NEXT:     let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:     let _v8: i32 = total;
// LOWERING-NEXT:     let _v9: i32 = _v8 + _v7;
// LOWERING-NEXT:     total = _v9;
// LOWERING-NEXT:     let _v10: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: *mut i8 = unsafe { secure_getenv(_v10 as *const i8) };
// LOWERING-NEXT:     let _v12: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v13: i32 = unsafe { strcmp(_v11 as *const i8, _v12 as *const i8) };
// LOWERING-NEXT:     let _v14: i32 = 0;
// LOWERING-NEXT:     let _v15: bool = _v13 == _v14;
// LOWERING-NEXT:     let _v16: i32 = _v15 as i32;
// LOWERING-NEXT:     let _v17: i32 = total;
// LOWERING-NEXT:     let _v18: i32 = _v17 + _v16;
// LOWERING-NEXT:     total = _v18;
// LOWERING-NEXT:     let _v19: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: i32 = unsafe { unsetenv(_v19 as *const i8) };
// LOWERING-NEXT:     let _v21: i32 = 0;
// LOWERING-NEXT:     let _v22: bool = _v20 == _v21;
// LOWERING-NEXT:     let _v23: i32 = _v22 as i32;
// LOWERING-NEXT:     let _v24: i32 = total;
// LOWERING-NEXT:     let _v25: i32 = _v24 + _v23;
// LOWERING-NEXT:     total = _v25;
// LOWERING-NEXT:     let _v26: *mut i8 = unsafe { get_current_dir_name() };
// LOWERING-NEXT:     directory = _v26;
// LOWERING-NEXT:     let _v27: *mut i8 = b".\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v28: *mut i8 = unsafe { canonicalize_file_name(_v27 as *const i8) };
// LOWERING-NEXT:     canonical = _v28;
// LOWERING-NEXT:     let _v29: *mut i8 = current.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v30: u64 = 4096;
// LOWERING-NEXT:     let _v31: *mut i8 = unsafe { getcwd(_v29 as *mut i8, _v30 as usize) };
// LOWERING-NEXT:     let _v32: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v33: *mut i8 = _v32 as *mut i8;
// LOWERING-NEXT:     let _v34: bool = _v31 != _v33;
// LOWERING-NEXT:     let _v35: i32 = _v34 as i32;
// LOWERING-NEXT:     let _v36: i32 = total;
// LOWERING-NEXT:     let _v37: i32 = _v36 + _v35;
// LOWERING-NEXT:     total = _v37;
// LOWERING-NEXT:     let _v38: *mut i8 = directory;
// LOWERING-NEXT:     let _v39: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v40: *mut i8 = _v39 as *mut i8;
// LOWERING-NEXT:     let _v41: bool = _v38 != _v40;
// LOWERING-NEXT:     let _v42: bool = if _v41 {
// LOWERING-NEXT:         let _v43: *mut i8 = directory;
// LOWERING-NEXT:         let _v44: *mut i8 = current.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:         let _v45: i32 = unsafe { strcmp(_v43 as *const i8, _v44 as *const i8) };
// LOWERING-NEXT:         let _v46: i32 = 0;
// LOWERING-NEXT:         let _v47: bool = _v45 == _v46;
// LOWERING-NEXT:         _v47
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v48: bool = false;
// LOWERING-NEXT:         _v48
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v49: i32 = _v42 as i32;
// LOWERING-NEXT:     let _v50: i32 = total;
// LOWERING-NEXT:     let _v51: i32 = _v50 + _v49;
// LOWERING-NEXT:     total = _v51;
// LOWERING-NEXT:     let _v52: *mut i8 = canonical;
// LOWERING-NEXT:     let _v53: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v54: *mut i8 = _v53 as *mut i8;
// LOWERING-NEXT:     let _v55: bool = _v52 != _v54;
// LOWERING-NEXT:     let _v56: bool = if _v55 {
// LOWERING-NEXT:         let _v57: *mut i8 = canonical;
// LOWERING-NEXT:         let _v58: *mut i8 = current.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:         let _v59: i32 = unsafe { strcmp(_v57 as *const i8, _v58 as *const i8) };
// LOWERING-NEXT:         let _v60: i32 = 0;
// LOWERING-NEXT:         let _v61: bool = _v59 == _v60;
// LOWERING-NEXT:         _v61
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v62: bool = false;
// LOWERING-NEXT:         _v62
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v63: i32 = _v56 as i32;
// LOWERING-NEXT:     let _v64: i32 = total;
// LOWERING-NEXT:     let _v65: i32 = _v64 + _v63;
// LOWERING-NEXT:     total = _v65;
// LOWERING-NEXT:     let _v66: *mut i8 = directory;
// LOWERING-NEXT:     let _v67: *mut core::ffi::c_void = _v66 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v67 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v68: *mut i8 = canonical;
// LOWERING-NEXT:     let _v69: *mut core::ffi::c_void = _v68 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v69 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v70: u64 = 5;
// LOWERING-NEXT:     let _v71: u64 = 1;
// LOWERING-NEXT:     let _v72: u64 = _v70 + _v71;
// LOWERING-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; _v72 as usize];
// LOWERING-NEXT:     let _v73: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v74: *mut i8 = _v73 as *mut i8;
// LOWERING-NEXT:     let _v75: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v76: *mut i8 = unsafe { strcpy(_v74 as *mut i8, _v75 as *const i8) };
// LOWERING-NEXT:     let _v77: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v78: i32 = unsafe { strcmp(_v76 as *const i8, _v77 as *const i8) };
// LOWERING-NEXT:     let _v79: i32 = 0;
// LOWERING-NEXT:     let _v80: bool = _v78 == _v79;
// LOWERING-NEXT:     let _v81: i32 = _v80 as i32;
// LOWERING-NEXT:     let _v82: i32 = total;
// LOWERING-NEXT:     let _v83: i32 = _v82 + _v81;
// LOWERING-NEXT:     total = _v83;
// LOWERING-NEXT:     let _v84: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v85: u64 = 5;
// LOWERING-NEXT:     let _v86: u64 = (unsafe { strnlen(_v84 as *const i8, _v85 as usize) }) as u64;
// LOWERING-NEXT:     let _v87: u64 = 1;
// LOWERING-NEXT:     let _v88: u64 = _v86 + _v87;
// LOWERING-NEXT:     let mut bi_alloca2: Vec<u8> = vec![0; _v88 as usize];
// LOWERING-NEXT:     let _v89: *mut core::ffi::c_void = bi_alloca2.as_mut_ptr() as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v90: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v91: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v92: u64 = 5;
// LOWERING-NEXT:     let _v93: u64 = (unsafe { strnlen(_v91 as *const i8, _v92 as usize) }) as u64;
// LOWERING-NEXT:     let _v94: *mut i8 = __slate_strndupa_finish(_v89 as *mut core::ffi::c_void, _v90, _v93);
// LOWERING-NEXT:     let _v95: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v96: i32 = unsafe { strcmp(_v94 as *const i8, _v95 as *const i8) };
// LOWERING-NEXT:     let _v97: i32 = 0;
// LOWERING-NEXT:     let _v98: bool = _v96 == _v97;
// LOWERING-NEXT:     let _v99: i32 = _v98 as i32;
// LOWERING-NEXT:     let _v100: i32 = total;
// LOWERING-NEXT:     let _v101: i32 = _v100 + _v99;
// LOWERING-NEXT:     total = _v101;
// LOWERING-NEXT:     let _v102: i32 = total;
// LOWERING-NEXT:     __retval = _v102;
// LOWERING-NEXT:     let _v103: i32 = __retval;
// LOWERING-NEXT:     return _v103;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_time_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut epoch: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut local: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut timestamp: i64 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     epoch = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     local = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     total = _v0;
// LOWERING-NEXT:     let _v1: i32 = 70;
// LOWERING-NEXT:     epoch.tm_year = _v1;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     epoch.tm_mon = _v2;
// LOWERING-NEXT:     let _v3: i32 = 1;
// LOWERING-NEXT:     epoch.tm_mday = _v3;
// LOWERING-NEXT:     let _v4: i64 = unsafe { timegm(std::ptr::addr_of_mut!(epoch) as *mut tm) };
// LOWERING-NEXT:     timestamp = _v4;
// LOWERING-NEXT:     let _v5: i64 = timestamp;
// LOWERING-NEXT:     let _v6: i64 = 0;
// LOWERING-NEXT:     let _v7: bool = _v5 == _v6;
// LOWERING-NEXT:     let _v8: i32 = _v7 as i32;
// LOWERING-NEXT:     let _v9: i32 = total;
// LOWERING-NEXT:     let _v10: i32 = _v9 + _v8;
// LOWERING-NEXT:     total = _v10;
// LOWERING-NEXT:     let _v11: i32 = 70;
// LOWERING-NEXT:     local.tm_year = _v11;
// LOWERING-NEXT:     let _v12: i32 = 0;
// LOWERING-NEXT:     local.tm_mon = _v12;
// LOWERING-NEXT:     let _v13: i32 = 2;
// LOWERING-NEXT:     local.tm_mday = _v13;
// LOWERING-NEXT:     let _v14: i64 = unsafe { timelocal(std::ptr::addr_of_mut!(local) as *mut tm) };
// LOWERING-NEXT:     let _v15: i64 = -1;
// LOWERING-NEXT:     let _v16: bool = _v14 != _v15;
// LOWERING-NEXT:     let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:     let _v18: i32 = total;
// LOWERING-NEXT:     let _v19: i32 = _v18 + _v17;
// LOWERING-NEXT:     total = _v19;
// LOWERING-NEXT:     let _v20: i32 = total;
// LOWERING-NEXT:     __retval = _v20;
// LOWERING-NEXT:     let _v21: i32 = __retval;
// LOWERING-NEXT:     return _v21;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_pattern_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut expression: re_pattern_buffer = re_pattern_buffer { re_nsub: 0, __opaque: std::ptr::null_mut(), __padding: [std::ptr::null_mut(); 4], __nsub2: 0, __padding2: 0 };
// LOWERING-NEXT:     let mut paths: glob_t = glob_t { gl_pathc: 0, gl_pathv: std::ptr::null_mut(), gl_offs: 0, __reserved1: 0, __reserved2: [std::ptr::null_mut(); 5] };
// LOWERING-NEXT:     let mut error: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     expression = re_pattern_buffer { re_nsub: 0, __opaque: std::ptr::null_mut(), __padding: [std::ptr::null_mut(); 4], __nsub2: 0, __padding2: 0 };
// LOWERING-NEXT:     paths = glob_t { gl_pathc: 0, gl_pathv: std::ptr::null_mut(), gl_offs: 0, __reserved1: 0, __reserved2: [std::ptr::null_mut(); 5] };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     total = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"file-+(one|two).c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i8 = b"file-two.c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: i32 = 32;
// LOWERING-NEXT:     let _v4: i32 = unsafe { fnmatch(_v1 as *const i8, _v2 as *const i8, _v3 as i32) };
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     let _v6: bool = _v4 == _v5;
// LOWERING-NEXT:     let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:     let _v8: i32 = total;
// LOWERING-NEXT:     let _v9: i32 = _v8 + _v7;
// LOWERING-NEXT:     total = _v9;
// LOWERING-NEXT:     let _v10: u64 = 1;
// LOWERING-NEXT:     let _v11: i32 = 1;
// LOWERING-NEXT:     let _v12: u64 = _v10 << _v11;
// LOWERING-NEXT:     let _v13: i32 = 1;
// LOWERING-NEXT:     let _v14: u64 = _v12 << _v13;
// LOWERING-NEXT:     let _v15: u64 = 1;
// LOWERING-NEXT:     let _v16: i32 = 1;
// LOWERING-NEXT:     let _v17: u64 = _v15 << _v16;
// LOWERING-NEXT:     let _v18: i32 = 1;
// LOWERING-NEXT:     let _v19: u64 = _v17 << _v18;
// LOWERING-NEXT:     let _v20: i32 = 1;
// LOWERING-NEXT:     let _v21: u64 = _v19 << _v20;
// LOWERING-NEXT:     let _v22: i32 = 1;
// LOWERING-NEXT:     let _v23: u64 = _v21 << _v22;
// LOWERING-NEXT:     let _v24: i32 = 1;
// LOWERING-NEXT:     let _v25: u64 = _v23 << _v24;
// LOWERING-NEXT:     let _v26: i32 = 1;
// LOWERING-NEXT:     let _v27: u64 = _v25 << _v26;
// LOWERING-NEXT:     let _v28: u64 = _v14 | _v27;
// LOWERING-NEXT:     let _v29: u64 = 1;
// LOWERING-NEXT:     let _v30: i32 = 1;
// LOWERING-NEXT:     let _v31: u64 = _v29 << _v30;
// LOWERING-NEXT:     let _v32: i32 = 1;
// LOWERING-NEXT:     let _v33: u64 = _v31 << _v32;
// LOWERING-NEXT:     let _v34: i32 = 1;
// LOWERING-NEXT:     let _v35: u64 = _v33 << _v34;
// LOWERING-NEXT:     let _v36: i32 = 1;
// LOWERING-NEXT:     let _v37: u64 = _v35 << _v36;
// LOWERING-NEXT:     let _v38: i32 = 1;
// LOWERING-NEXT:     let _v39: u64 = _v37 << _v38;
// LOWERING-NEXT:     let _v40: i32 = 1;
// LOWERING-NEXT:     let _v41: u64 = _v39 << _v40;
// LOWERING-NEXT:     let _v42: i32 = 1;
// LOWERING-NEXT:     let _v43: u64 = _v41 << _v42;
// LOWERING-NEXT:     let _v44: u64 = _v28 | _v43;
// LOWERING-NEXT:     let _v45: u64 = 1;
// LOWERING-NEXT:     let _v46: i32 = 1;
// LOWERING-NEXT:     let _v47: u64 = _v45 << _v46;
// LOWERING-NEXT:     let _v48: i32 = 1;
// LOWERING-NEXT:     let _v49: u64 = _v47 << _v48;
// LOWERING-NEXT:     let _v50: i32 = 1;
// LOWERING-NEXT:     let _v51: u64 = _v49 << _v50;
// LOWERING-NEXT:     let _v52: i32 = 1;
// LOWERING-NEXT:     let _v53: u64 = _v51 << _v52;
// LOWERING-NEXT:     let _v54: i32 = 1;
// LOWERING-NEXT:     let _v55: u64 = _v53 << _v54;
// LOWERING-NEXT:     let _v56: i32 = 1;
// LOWERING-NEXT:     let _v57: u64 = _v55 << _v56;
// LOWERING-NEXT:     let _v58: i32 = 1;
// LOWERING-NEXT:     let _v59: u64 = _v57 << _v58;
// LOWERING-NEXT:     let _v60: i32 = 1;
// LOWERING-NEXT:     let _v61: u64 = _v59 << _v60;
// LOWERING-NEXT:     let _v62: i32 = 1;
// LOWERING-NEXT:     let _v63: u64 = _v61 << _v62;
// LOWERING-NEXT:     let _v64: u64 = _v44 | _v63;
// LOWERING-NEXT:     let _v65: u64 = 1;
// LOWERING-NEXT:     let _v66: i32 = 1;
// LOWERING-NEXT:     let _v67: u64 = _v65 << _v66;
// LOWERING-NEXT:     let _v68: i32 = 1;
// LOWERING-NEXT:     let _v69: u64 = _v67 << _v68;
// LOWERING-NEXT:     let _v70: i32 = 1;
// LOWERING-NEXT:     let _v71: u64 = _v69 << _v70;
// LOWERING-NEXT:     let _v72: i32 = 1;
// LOWERING-NEXT:     let _v73: u64 = _v71 << _v72;
// LOWERING-NEXT:     let _v74: i32 = 1;
// LOWERING-NEXT:     let _v75: u64 = _v73 << _v74;
// LOWERING-NEXT:     let _v76: i32 = 1;
// LOWERING-NEXT:     let _v77: u64 = _v75 << _v76;
// LOWERING-NEXT:     let _v78: i32 = 1;
// LOWERING-NEXT:     let _v79: u64 = _v77 << _v78;
// LOWERING-NEXT:     let _v80: i32 = 1;
// LOWERING-NEXT:     let _v81: u64 = _v79 << _v80;
// LOWERING-NEXT:     let _v82: i32 = 1;
// LOWERING-NEXT:     let _v83: u64 = _v81 << _v82;
// LOWERING-NEXT:     let _v84: i32 = 1;
// LOWERING-NEXT:     let _v85: u64 = _v83 << _v84;
// LOWERING-NEXT:     let _v86: i32 = 1;
// LOWERING-NEXT:     let _v87: u64 = _v85 << _v86;
// LOWERING-NEXT:     let _v88: i32 = 1;
// LOWERING-NEXT:     let _v89: u64 = _v87 << _v88;
// LOWERING-NEXT:     let _v90: i32 = 1;
// LOWERING-NEXT:     let _v91: u64 = _v89 << _v90;
// LOWERING-NEXT:     let _v92: i32 = 1;
// LOWERING-NEXT:     let _v93: u64 = _v91 << _v92;
// LOWERING-NEXT:     let _v94: i32 = 1;
// LOWERING-NEXT:     let _v95: u64 = _v93 << _v94;
// LOWERING-NEXT:     let _v96: i32 = 1;
// LOWERING-NEXT:     let _v97: u64 = _v95 << _v96;
// LOWERING-NEXT:     let _v98: u64 = _v64 | _v97;
// LOWERING-NEXT:     let _v99: u64 = 1;
// LOWERING-NEXT:     let _v100: i32 = 1;
// LOWERING-NEXT:     let _v101: u64 = _v99 << _v100;
// LOWERING-NEXT:     let _v102: i32 = 1;
// LOWERING-NEXT:     let _v103: u64 = _v101 << _v102;
// LOWERING-NEXT:     let _v104: i32 = 1;
// LOWERING-NEXT:     let _v105: u64 = _v103 << _v104;
// LOWERING-NEXT:     let _v106: u64 = _v98 | _v105;
// LOWERING-NEXT:     let _v107: u64 = 1;
// LOWERING-NEXT:     let _v108: i32 = 1;
// LOWERING-NEXT:     let _v109: u64 = _v107 << _v108;
// LOWERING-NEXT:     let _v110: i32 = 1;
// LOWERING-NEXT:     let _v111: u64 = _v109 << _v110;
// LOWERING-NEXT:     let _v112: i32 = 1;
// LOWERING-NEXT:     let _v113: u64 = _v111 << _v112;
// LOWERING-NEXT:     let _v114: i32 = 1;
// LOWERING-NEXT:     let _v115: u64 = _v113 << _v114;
// LOWERING-NEXT:     let _v116: u64 = _v106 | _v115;
// LOWERING-NEXT:     let _v117: u64 = 1;
// LOWERING-NEXT:     let _v118: i32 = 1;
// LOWERING-NEXT:     let _v119: u64 = _v117 << _v118;
// LOWERING-NEXT:     let _v120: i32 = 1;
// LOWERING-NEXT:     let _v121: u64 = _v119 << _v120;
// LOWERING-NEXT:     let _v122: i32 = 1;
// LOWERING-NEXT:     let _v123: u64 = _v121 << _v122;
// LOWERING-NEXT:     let _v124: i32 = 1;
// LOWERING-NEXT:     let _v125: u64 = _v123 << _v124;
// LOWERING-NEXT:     let _v126: i32 = 1;
// LOWERING-NEXT:     let _v127: u64 = _v125 << _v126;
// LOWERING-NEXT:     let _v128: i32 = 1;
// LOWERING-NEXT:     let _v129: u64 = _v127 << _v128;
// LOWERING-NEXT:     let _v130: i32 = 1;
// LOWERING-NEXT:     let _v131: u64 = _v129 << _v130;
// LOWERING-NEXT:     let _v132: i32 = 1;
// LOWERING-NEXT:     let _v133: u64 = _v131 << _v132;
// LOWERING-NEXT:     let _v134: i32 = 1;
// LOWERING-NEXT:     let _v135: u64 = _v133 << _v134;
// LOWERING-NEXT:     let _v136: i32 = 1;
// LOWERING-NEXT:     let _v137: u64 = _v135 << _v136;
// LOWERING-NEXT:     let _v138: i32 = 1;
// LOWERING-NEXT:     let _v139: u64 = _v137 << _v138;
// LOWERING-NEXT:     let _v140: i32 = 1;
// LOWERING-NEXT:     let _v141: u64 = _v139 << _v140;
// LOWERING-NEXT:     let _v142: u64 = _v116 | _v141;
// LOWERING-NEXT:     let _v143: u64 = 1;
// LOWERING-NEXT:     let _v144: i32 = 1;
// LOWERING-NEXT:     let _v145: u64 = _v143 << _v144;
// LOWERING-NEXT:     let _v146: i32 = 1;
// LOWERING-NEXT:     let _v147: u64 = _v145 << _v146;
// LOWERING-NEXT:     let _v148: i32 = 1;
// LOWERING-NEXT:     let _v149: u64 = _v147 << _v148;
// LOWERING-NEXT:     let _v150: i32 = 1;
// LOWERING-NEXT:     let _v151: u64 = _v149 << _v150;
// LOWERING-NEXT:     let _v152: i32 = 1;
// LOWERING-NEXT:     let _v153: u64 = _v151 << _v152;
// LOWERING-NEXT:     let _v154: i32 = 1;
// LOWERING-NEXT:     let _v155: u64 = _v153 << _v154;
// LOWERING-NEXT:     let _v156: i32 = 1;
// LOWERING-NEXT:     let _v157: u64 = _v155 << _v156;
// LOWERING-NEXT:     let _v158: i32 = 1;
// LOWERING-NEXT:     let _v159: u64 = _v157 << _v158;
// LOWERING-NEXT:     let _v160: i32 = 1;
// LOWERING-NEXT:     let _v161: u64 = _v159 << _v160;
// LOWERING-NEXT:     let _v162: i32 = 1;
// LOWERING-NEXT:     let _v163: u64 = _v161 << _v162;
// LOWERING-NEXT:     let _v164: i32 = 1;
// LOWERING-NEXT:     let _v165: u64 = _v163 << _v164;
// LOWERING-NEXT:     let _v166: i32 = 1;
// LOWERING-NEXT:     let _v167: u64 = _v165 << _v166;
// LOWERING-NEXT:     let _v168: i32 = 1;
// LOWERING-NEXT:     let _v169: u64 = _v167 << _v168;
// LOWERING-NEXT:     let _v170: u64 = _v142 | _v169;
// LOWERING-NEXT:     let _v171: u64 = 1;
// LOWERING-NEXT:     let _v172: i32 = 1;
// LOWERING-NEXT:     let _v173: u64 = _v171 << _v172;
// LOWERING-NEXT:     let _v174: i32 = 1;
// LOWERING-NEXT:     let _v175: u64 = _v173 << _v174;
// LOWERING-NEXT:     let _v176: i32 = 1;
// LOWERING-NEXT:     let _v177: u64 = _v175 << _v176;
// LOWERING-NEXT:     let _v178: i32 = 1;
// LOWERING-NEXT:     let _v179: u64 = _v177 << _v178;
// LOWERING-NEXT:     let _v180: i32 = 1;
// LOWERING-NEXT:     let _v181: u64 = _v179 << _v180;
// LOWERING-NEXT:     let _v182: i32 = 1;
// LOWERING-NEXT:     let _v183: u64 = _v181 << _v182;
// LOWERING-NEXT:     let _v184: i32 = 1;
// LOWERING-NEXT:     let _v185: u64 = _v183 << _v184;
// LOWERING-NEXT:     let _v186: i32 = 1;
// LOWERING-NEXT:     let _v187: u64 = _v185 << _v186;
// LOWERING-NEXT:     let _v188: i32 = 1;
// LOWERING-NEXT:     let _v189: u64 = _v187 << _v188;
// LOWERING-NEXT:     let _v190: i32 = 1;
// LOWERING-NEXT:     let _v191: u64 = _v189 << _v190;
// LOWERING-NEXT:     let _v192: i32 = 1;
// LOWERING-NEXT:     let _v193: u64 = _v191 << _v192;
// LOWERING-NEXT:     let _v194: i32 = 1;
// LOWERING-NEXT:     let _v195: u64 = _v193 << _v194;
// LOWERING-NEXT:     let _v196: i32 = 1;
// LOWERING-NEXT:     let _v197: u64 = _v195 << _v196;
// LOWERING-NEXT:     let _v198: i32 = 1;
// LOWERING-NEXT:     let _v199: u64 = _v197 << _v198;
// LOWERING-NEXT:     let _v200: i32 = 1;
// LOWERING-NEXT:     let _v201: u64 = _v199 << _v200;
// LOWERING-NEXT:     let _v202: u64 = _v170 | _v201;
// LOWERING-NEXT:     let _v203: u64 = 1;
// LOWERING-NEXT:     let _v204: i32 = 1;
// LOWERING-NEXT:     let _v205: u64 = _v203 << _v204;
// LOWERING-NEXT:     let _v206: i32 = 1;
// LOWERING-NEXT:     let _v207: u64 = _v205 << _v206;
// LOWERING-NEXT:     let _v208: i32 = 1;
// LOWERING-NEXT:     let _v209: u64 = _v207 << _v208;
// LOWERING-NEXT:     let _v210: i32 = 1;
// LOWERING-NEXT:     let _v211: u64 = _v209 << _v210;
// LOWERING-NEXT:     let _v212: i32 = 1;
// LOWERING-NEXT:     let _v213: u64 = _v211 << _v212;
// LOWERING-NEXT:     let _v214: u64 = _v202 | _v213;
// LOWERING-NEXT:     let _v215: u64 = 1;
// LOWERING-NEXT:     let _v216: i32 = 1;
// LOWERING-NEXT:     let _v217: u64 = _v215 << _v216;
// LOWERING-NEXT:     let _v218: i32 = 1;
// LOWERING-NEXT:     let _v219: u64 = _v217 << _v218;
// LOWERING-NEXT:     let _v220: i32 = 1;
// LOWERING-NEXT:     let _v221: u64 = _v219 << _v220;
// LOWERING-NEXT:     let _v222: i32 = 1;
// LOWERING-NEXT:     let _v223: u64 = _v221 << _v222;
// LOWERING-NEXT:     let _v224: i32 = 1;
// LOWERING-NEXT:     let _v225: u64 = _v223 << _v224;
// LOWERING-NEXT:     let _v226: i32 = 1;
// LOWERING-NEXT:     let _v227: u64 = _v225 << _v226;
// LOWERING-NEXT:     let _v228: i32 = 1;
// LOWERING-NEXT:     let _v229: u64 = _v227 << _v228;
// LOWERING-NEXT:     let _v230: i32 = 1;
// LOWERING-NEXT:     let _v231: u64 = _v229 << _v230;
// LOWERING-NEXT:     let _v232: i32 = 1;
// LOWERING-NEXT:     let _v233: u64 = _v231 << _v232;
// LOWERING-NEXT:     let _v234: i32 = 1;
// LOWERING-NEXT:     let _v235: u64 = _v233 << _v234;
// LOWERING-NEXT:     let _v236: i32 = 1;
// LOWERING-NEXT:     let _v237: u64 = _v235 << _v236;
// LOWERING-NEXT:     let _v238: i32 = 1;
// LOWERING-NEXT:     let _v239: u64 = _v237 << _v238;
// LOWERING-NEXT:     let _v240: i32 = 1;
// LOWERING-NEXT:     let _v241: u64 = _v239 << _v240;
// LOWERING-NEXT:     let _v242: i32 = 1;
// LOWERING-NEXT:     let _v243: u64 = _v241 << _v242;
// LOWERING-NEXT:     let _v244: i32 = 1;
// LOWERING-NEXT:     let _v245: u64 = _v243 << _v244;
// LOWERING-NEXT:     let _v246: i32 = 1;
// LOWERING-NEXT:     let _v247: u64 = _v245 << _v246;
// LOWERING-NEXT:     let _v248: i32 = 1;
// LOWERING-NEXT:     let _v249: u64 = _v247 << _v248;
// LOWERING-NEXT:     let _v250: u64 = _v214 | _v249;
// LOWERING-NEXT:     let _v251: u64 = unsafe { re_set_syntax(_v250 as u64) };
// LOWERING-NEXT:     let _v252: *mut i8 = b"sl(a|e)te\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v253: u64 = 9;
// LOWERING-NEXT:     let _v254: *mut i8 = (unsafe { re_compile_pattern(_v252 as *const i8, _v253 as usize, std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) }) as *mut i8;
// LOWERING-NEXT:     error = _v254;
// LOWERING-NEXT:     let _v255: *mut i8 = error;
// LOWERING-NEXT:     let _v256: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v257: *mut i8 = _v256 as *mut i8;
// LOWERING-NEXT:     let _v258: bool = _v255 == _v257;
// LOWERING-NEXT:     let _v259: i32 = _v258 as i32;
// LOWERING-NEXT:     let _v260: i32 = total;
// LOWERING-NEXT:     let _v261: i32 = _v260 + _v259;
// LOWERING-NEXT:     total = _v261;
// LOWERING-NEXT:     let _v262: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v263: u64 = 5;
// LOWERING-NEXT:     let _v264: i64 = 0;
// LOWERING-NEXT:     let _v265: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v266: i64 = unsafe { re_match(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer, _v262 as *const i8, _v263 as usize, _v264 as i64, _v265 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v267: i64 = 5;
// LOWERING-NEXT:     let _v268: bool = _v266 == _v267;
// LOWERING-NEXT:     let _v269: i32 = _v268 as i32;
// LOWERING-NEXT:     let _v270: i32 = total;
// LOWERING-NEXT:     let _v271: i32 = _v270 + _v269;
// LOWERING-NEXT:     total = _v271;
// LOWERING-NEXT:     unsafe { regfree(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) };
// LOWERING-NEXT:     let _v272: *mut i8 = b"/dev/{null,zero}\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v273: i32 = 1024;
// LOWERING-NEXT:     let _v274: Option<unsafe extern "C" fn(*mut i8, i32) -> i32> = None;
// LOWERING-NEXT:     let _v275: i32 = unsafe { glob(_v272 as *const i8, _v273 as i32, _v274, std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// LOWERING-NEXT:     let _v276: i32 = 0;
// LOWERING-NEXT:     let _v277: bool = _v275 == _v276;
// LOWERING-NEXT:     let _v278: i32 = _v277 as i32;
// LOWERING-NEXT:     let _v279: i32 = total;
// LOWERING-NEXT:     let _v280: i32 = _v279 + _v278;
// LOWERING-NEXT:     total = _v280;
// LOWERING-NEXT:     let _v281: u64 = paths.gl_pathc;
// LOWERING-NEXT:     let _v282: u64 = 2;
// LOWERING-NEXT:     let _v283: bool = _v281 == _v282;
// LOWERING-NEXT:     let _v284: i32 = _v283 as i32;
// LOWERING-NEXT:     let _v285: i32 = total;
// LOWERING-NEXT:     let _v286: i32 = _v285 + _v284;
// LOWERING-NEXT:     total = _v286;
// LOWERING-NEXT:     let _v287: i64 = 0;
// LOWERING-NEXT:     let _v288: *mut *mut i8 = paths.gl_pathv;
// LOWERING-NEXT:     let _v289: *mut *mut i8 = unsafe { _v288.add(0) };
// LOWERING-NEXT:     let _v290: *mut i8 = unsafe { *_v289 };
// LOWERING-NEXT:     let _v291: *mut i8 = b"/dev/null\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v292: i32 = unsafe { strcmp(_v290 as *const i8, _v291 as *const i8) };
// LOWERING-NEXT:     let _v293: i32 = 0;
// LOWERING-NEXT:     let _v294: bool = _v292 == _v293;
// LOWERING-NEXT:     let _v295: i32 = _v294 as i32;
// LOWERING-NEXT:     let _v296: i32 = total;
// LOWERING-NEXT:     let _v297: i32 = _v296 + _v295;
// LOWERING-NEXT:     total = _v297;
// LOWERING-NEXT:     let _v298: i64 = 1;
// LOWERING-NEXT:     let _v299: *mut *mut i8 = paths.gl_pathv;
// LOWERING-NEXT:     let _v300: *mut *mut i8 = unsafe { _v299.add(1) };
// LOWERING-NEXT:     let _v301: *mut i8 = unsafe { *_v300 };
// LOWERING-NEXT:     let _v302: *mut i8 = b"/dev/zero\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v303: i32 = unsafe { strcmp(_v301 as *const i8, _v302 as *const i8) };
// LOWERING-NEXT:     let _v304: i32 = 0;
// LOWERING-NEXT:     let _v305: bool = _v303 == _v304;
// LOWERING-NEXT:     let _v306: i32 = _v305 as i32;
// LOWERING-NEXT:     let _v307: i32 = total;
// LOWERING-NEXT:     let _v308: i32 = _v307 + _v306;
// LOWERING-NEXT:     total = _v308;
// LOWERING-NEXT:     unsafe { globfree(std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// LOWERING-NEXT:     let _v309: i32 = total;
// LOWERING-NEXT:     __retval = _v309;
// LOWERING-NEXT:     let _v310: i32 = __retval;
// LOWERING-NEXT:     return _v310;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn gnu_runtime_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut random_bytes: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let mut frames: aligned::Aligned<aligned::A16, [*mut core::ffi::c_void; 8]> = aligned::Aligned([std::ptr::null_mut(); 8]);
// LOWERING-NEXT:     let mut information: Dl_info = Dl_info { dli_fname: std::ptr::null_mut(), dli_fbase: std::ptr::null_mut(), dli_sname: std::ptr::null_mut(), dli_saddr: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut page_size: i64 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     information = Dl_info { dli_fname: std::ptr::null_mut(), dli_fbase: std::ptr::null_mut(), dli_sname: std::ptr::null_mut(), dli_saddr: std::ptr::null_mut() };
// LOWERING-NEXT:     let _v0: i32 = 30;
// LOWERING-NEXT:     let _v1: i64 = unsafe { sysconf(_v0 as i32) };
// LOWERING-NEXT:     page_size = _v1;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     total = _v2;
// LOWERING-NEXT:     let _v3: u64 = 6;
// LOWERING-NEXT:     let _v4: u64 = unsafe { getauxval(_v3 as u64) };
// LOWERING-NEXT:     let _v5: i64 = page_size;
// LOWERING-NEXT:     let _v6: u64 = _v5 as u64;
// LOWERING-NEXT:     let _v7: bool = _v4 == _v6;
// LOWERING-NEXT:     let _v8: i32 = _v7 as i32;
// LOWERING-NEXT:     let _v9: i32 = total;
// LOWERING-NEXT:     let _v10: i32 = _v9 + _v8;
// LOWERING-NEXT:     total = _v10;
// LOWERING-NEXT:     let _v11: i32 = unsafe { gettid() };
// LOWERING-NEXT:     let _v12: i64 = 186;
// LOWERING-NEXT:     let _v13: i64 = unsafe { syscall(_v12 as i64) };
// LOWERING-NEXT:     let _v14: i32 = _v13 as i32;
// LOWERING-NEXT:     let _v15: bool = _v11 == _v14;
// LOWERING-NEXT:     let _v16: i32 = _v15 as i32;
// LOWERING-NEXT:     let _v17: i32 = total;
// LOWERING-NEXT:     let _v18: i32 = _v17 + _v16;
// LOWERING-NEXT:     total = _v18;
// LOWERING-NEXT:     let _v19: *mut u8 = random_bytes.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v20: *mut core::ffi::c_void = _v19 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v21: u64 = 8;
// LOWERING-NEXT:     let _v22: i32 = unsafe { getentropy(_v20 as *mut core::ffi::c_void, _v21 as usize) };
// LOWERING-NEXT:     let _v23: i32 = 0;
// LOWERING-NEXT:     let _v24: bool = _v22 == _v23;
// LOWERING-NEXT:     let _v25: i32 = _v24 as i32;
// LOWERING-NEXT:     let _v26: i32 = total;
// LOWERING-NEXT:     let _v27: i32 = _v26 + _v25;
// LOWERING-NEXT:     total = _v27;
// LOWERING-NEXT:     let _v28: u32 = 1;
// LOWERING-NEXT:     let _v29: u32 = unsafe { arc4random_uniform(_v28 as u32) };
// LOWERING-NEXT:     let _v30: u32 = 0;
// LOWERING-NEXT:     let _v31: bool = _v29 == _v30;
// LOWERING-NEXT:     let _v32: i32 = _v31 as i32;
// LOWERING-NEXT:     let _v33: i32 = total;
// LOWERING-NEXT:     let _v34: i32 = _v33 + _v32;
// LOWERING-NEXT:     total = _v34;
// LOWERING-NEXT:     let _v35: i32 = unsafe { get_nprocs() };
// LOWERING-NEXT:     let _v36: i32 = 0;
// LOWERING-NEXT:     let _v37: bool = _v35 > _v36;
// LOWERING-NEXT:     let _v38: i32 = _v37 as i32;
// LOWERING-NEXT:     let _v39: i32 = total;
// LOWERING-NEXT:     let _v40: i32 = _v39 + _v38;
// LOWERING-NEXT:     total = _v40;
// LOWERING-NEXT:     let _v41: i64 = unsafe { get_phys_pages() };
// LOWERING-NEXT:     let _v42: i64 = 0;
// LOWERING-NEXT:     let _v43: bool = _v41 > _v42;
// LOWERING-NEXT:     let _v44: i32 = _v43 as i32;
// LOWERING-NEXT:     let _v45: i32 = total;
// LOWERING-NEXT:     let _v46: i32 = _v45 + _v44;
// LOWERING-NEXT:     total = _v46;
// LOWERING-NEXT:     let _v47: *mut *mut core::ffi::c_void = frames.as_mut_ptr() as *mut *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v48: i32 = 8;
// LOWERING-NEXT:     let _v49: i32 = unsafe { backtrace(_v47 as *mut *mut core::ffi::c_void, _v48 as i32) };
// LOWERING-NEXT:     let _v50: i32 = 0;
// LOWERING-NEXT:     let _v51: bool = _v49 > _v50;
// LOWERING-NEXT:     let _v52: i32 = _v51 as i32;
// LOWERING-NEXT:     let _v53: i32 = total;
// LOWERING-NEXT:     let _v54: i32 = _v53 + _v52;
// LOWERING-NEXT:     total = _v54;
// LOWERING-NEXT:     let _v55: *mut core::ffi::c_void = unsafe { std::mem::transmute::<Option<unsafe extern "C" fn() -> i32>, *mut core::ffi::c_void>(Some(gnu_runtime_extensions)) };
// LOWERING-NEXT:     let _v56: i32 = unsafe { dladdr(_v55 as *const core::ffi::c_void, std::ptr::addr_of_mut!(information) as *mut Dl_info) };
// LOWERING-NEXT:     let _v57: i32 = 0;
// LOWERING-NEXT:     let _v58: bool = _v56 != _v57;
// LOWERING-NEXT:     let _v59: i32 = _v58 as i32;
// LOWERING-NEXT:     let _v60: i32 = total;
// LOWERING-NEXT:     let _v61: i32 = _v60 + _v59;
// LOWERING-NEXT:     total = _v61;
// LOWERING-NEXT:     let _v62: *mut i8 = information.dli_fname;
// LOWERING-NEXT:     let _v63: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v64: *mut i8 = _v63 as *mut i8;
// LOWERING-NEXT:     let _v65: bool = _v62 != _v64;
// LOWERING-NEXT:     let _v66: i32 = _v65 as i32;
// LOWERING-NEXT:     let _v67: i32 = total;
// LOWERING-NEXT:     let _v68: i32 = _v67 + _v66;
// LOWERING-NEXT:     total = _v68;
// LOWERING-NEXT:     let _v69: i64 = 0;
// LOWERING-NEXT:     let _v70: *mut i8 = (unsafe { gnu_get_libc_version() }) as *mut i8;
// LOWERING-NEXT:     let _v71: *mut i8 = unsafe { _v70.add(0) };
// LOWERING-NEXT:     let _v72: i8 = unsafe { *_v71 };
// LOWERING-NEXT:     let _v73: i32 = _v72 as i32;
// LOWERING-NEXT:     let _v74: i32 = 0;
// LOWERING-NEXT:     let _v75: bool = _v73 != _v74;
// LOWERING-NEXT:     let _v76: i32 = _v75 as i32;
// LOWERING-NEXT:     let _v77: i32 = total;
// LOWERING-NEXT:     let _v78: i32 = _v77 + _v76;
// LOWERING-NEXT:     total = _v78;
// LOWERING-NEXT:     let _v79: *mut i8 = unsafe { program_invocation_name };
// LOWERING-NEXT:     let _v80: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v81: *mut i8 = _v80 as *mut i8;
// LOWERING-NEXT:     let _v82: bool = _v79 != _v81;
// LOWERING-NEXT:     let _v83: i32 = _v82 as i32;
// LOWERING-NEXT:     let _v84: i32 = total;
// LOWERING-NEXT:     let _v85: i32 = _v84 + _v83;
// LOWERING-NEXT:     total = _v85;
// LOWERING-NEXT:     let _v86: *mut i8 = unsafe { program_invocation_short_name };
// LOWERING-NEXT:     let _v87: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v88: *mut i8 = _v87 as *mut i8;
// LOWERING-NEXT:     let _v89: bool = _v86 != _v88;
// LOWERING-NEXT:     let _v90: i32 = _v89 as i32;
// LOWERING-NEXT:     let _v91: i32 = total;
// LOWERING-NEXT:     let _v92: i32 = _v91 + _v90;
// LOWERING-NEXT:     total = _v92;
// LOWERING-NEXT:     let _v93: i32 = total;
// LOWERING-NEXT:     __retval = _v93;
// LOWERING-NEXT:     let _v94: i32 = __retval;
// LOWERING-NEXT:     return _v94;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = gnu_environment_extensions();
// LOWERING-NEXT:     let _v3: i32 = gnu_time_extensions();
// LOWERING-NEXT:     let _v4: i32 = gnu_pattern_extensions();
// LOWERING-NEXT:     let _v5: i32 = gnu_runtime_extensions();
// LOWERING-NEXT:     let _v6: i32 = _v4 + _v5;
// LOWERING-NEXT:     let _v7: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v6) };
// LOWERING-NEXT:     let _v8: i32 = 0;
// LOWERING-NEXT:     __retval = _v8;
// LOWERING-NEXT:     let _v9: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v9 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Dl_info {
// REWRITES-NEXT:     dli_fname: *mut i8,
// REWRITES-NEXT:     dli_fbase: *mut core::ffi::c_void,
// REWRITES-NEXT:     dli_sname: *mut i8,
// REWRITES-NEXT:     dli_saddr: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct glob_t {
// REWRITES-NEXT:     gl_pathc: u64,
// REWRITES-NEXT:     gl_pathv: *mut *mut i8,
// REWRITES-NEXT:     gl_offs: u64,
// REWRITES-NEXT:     __reserved1: i32,
// REWRITES-NEXT:     __reserved2: [*mut core::ffi::c_void; 5],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct re_pattern_buffer {
// REWRITES-NEXT:     re_nsub: u64,
// REWRITES-NEXT:     __opaque: *mut core::ffi::c_void,
// REWRITES-NEXT:     __padding: [*mut core::ffi::c_void; 4],
// REWRITES-NEXT:     __nsub2: u64,
// REWRITES-NEXT:     __padding2: i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct tm {
// REWRITES-NEXT:     tm_sec: i32,
// REWRITES-NEXT:     tm_min: i32,
// REWRITES-NEXT:     tm_hour: i32,
// REWRITES-NEXT:     tm_mday: i32,
// REWRITES-NEXT:     tm_mon: i32,
// REWRITES-NEXT:     tm_year: i32,
// REWRITES-NEXT:     tm_wday: i32,
// REWRITES-NEXT:     tm_yday: i32,
// REWRITES-NEXT:     tm_isdst: i32,
// REWRITES-NEXT:     tm_gmtoff: i64,
// REWRITES-NEXT:     tm_zone: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     static mut program_invocation_name: *mut i8;
// REWRITES-NEXT:     static mut program_invocation_short_name: *mut i8;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn setenv(_0: *const i8, _1: *const i8, _2: i32) -> i32;
// REWRITES-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// REWRITES-NEXT:     fn secure_getenv(_0: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn unsetenv(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn get_current_dir_name() -> *mut i8;
// REWRITES-NEXT:     fn canonicalize_file_name(_0: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn getcwd(_0: *mut i8, _1: usize) -> *mut i8;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn strcpy(_0: *mut i8, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strnlen(_0: *const i8, _1: usize) -> usize;
// REWRITES-NEXT:     fn timegm(_0: *mut tm) -> i64;
// REWRITES-NEXT:     fn timelocal(_0: *mut tm) -> i64;
// REWRITES-NEXT:     fn fnmatch(_0: *const i8, _1: *const i8, _2: i32) -> i32;
// REWRITES-NEXT:     fn re_set_syntax(_0: u64) -> u64;
// REWRITES-NEXT:     fn re_compile_pattern(_0: *const i8, _1: usize, _2: *mut re_pattern_buffer) -> *const i8;
// REWRITES-NEXT:     fn re_match(_0: *mut re_pattern_buffer, _1: *const i8, _2: usize, _3: i64, _4: *mut core::ffi::c_void) -> i64;
// REWRITES-NEXT:     fn regfree(_0: *mut re_pattern_buffer);
// REWRITES-NEXT:     fn glob(_0: *const i8, _1: i32, _2: Option<unsafe extern "C" fn(*mut i8, i32) -> i32>, _3: *mut glob_t) -> i32;
// REWRITES-NEXT:     fn globfree(_0: *mut glob_t);
// REWRITES-NEXT:     fn sysconf(_0: i32) -> i64;
// REWRITES-NEXT:     fn getauxval(_0: u64) -> u64;
// REWRITES-NEXT:     fn gettid() -> i32;
// REWRITES-NEXT:     fn syscall(_0: i64, ...) -> i64;
// REWRITES-NEXT:     fn getentropy(_0: *mut core::ffi::c_void, _1: usize) -> i32;
// REWRITES-NEXT:     fn arc4random_uniform(_0: u32) -> u32;
// REWRITES-NEXT:     fn get_nprocs() -> i32;
// REWRITES-NEXT:     fn get_phys_pages() -> i64;
// REWRITES-NEXT:     fn backtrace(_0: *mut *mut core::ffi::c_void, _1: i32) -> i32;
// REWRITES-NEXT:     fn dladdr(_0: *const core::ffi::c_void, _1: *mut Dl_info) -> i32;
// REWRITES-NEXT:     fn gnu_get_libc_version() -> *const i8;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn __slate_strndupa_finish(arg0: *mut core::ffi::c_void, arg1: *mut i8, arg2: u64) -> *mut i8 {
// REWRITES-NEXT: let mut buf: *mut core::ffi::c_void = arg0;
// REWRITES-NEXT: let mut s: *mut i8 = arg1;
// REWRITES-NEXT: let mut len: u64 = arg2;
// REWRITES-NEXT: let mut __retval: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut out: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: out = buf as *mut i8;
// REWRITES-NEXT: let _v7: *mut core::ffi::c_void = unsafe { memcpy((out as *mut core::ffi::c_void) as *mut core::ffi::c_void, (s as *mut core::ffi::c_void) as *const core::ffi::c_void, len as usize) };
// REWRITES-NEXT: let _v8: i8 = 0;
// REWRITES-NEXT: let _v10: *mut i8 = out;
// REWRITES-NEXT: let _v11: *mut i8 = unsafe { _v10.add(len as usize) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v11 = _v8;
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = out;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_environment_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut directory: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut canonical: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut current: aligned::Aligned<aligned::A16, [i8; 4096]> = aligned::Aligned([0; 4096]);
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = 1;
// REWRITES-NEXT: let _v4: i32 = unsafe { setenv(_v1 as *const i8, _v2 as *const i8, _v3 as i32) };
// REWRITES-NEXT: let _v5: i32 = 0;
// REWRITES-NEXT: total = total + ((_v4 == _v5) as i32);
// REWRITES-NEXT: let _v10: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: *mut i8 = unsafe { secure_getenv(_v10 as *const i8) };
// REWRITES-NEXT: let _v12: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: i32 = unsafe { strcmp(_v11 as *const i8, _v12 as *const i8) };
// REWRITES-NEXT: let _v14: i32 = 0;
// REWRITES-NEXT: total = total + ((_v13 == _v14) as i32);
// REWRITES-NEXT: let _v19: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: i32 = unsafe { unsetenv(_v19 as *const i8) };
// REWRITES-NEXT: let _v21: i32 = 0;
// REWRITES-NEXT: total = total + ((_v20 == _v21) as i32);
// REWRITES-NEXT: directory = unsafe { get_current_dir_name() };
// REWRITES-NEXT: let _v27: *mut i8 = b".\0".as_ptr() as *mut i8;
// REWRITES-NEXT: canonical = unsafe { canonicalize_file_name(_v27 as *const i8) };
// REWRITES-NEXT: let _v29: *mut i8 = current.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v30: u64 = 4096;
// REWRITES-NEXT: let _v31: *mut i8 = unsafe { getcwd(_v29 as *mut i8, _v30 as usize) };
// REWRITES-NEXT: let _v32: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v31 != (_v32 as *mut i8)) as i32);
// REWRITES-NEXT: let _v39: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v41: bool = directory != (_v39 as *mut i8);
// REWRITES-NEXT: let _v42: bool = if _v41 {
// REWRITES-NEXT:         let _v43: *mut i8 = directory;
// REWRITES-NEXT:         let _v44: *mut i8 = current.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:         let _v45: i32 = unsafe { strcmp(_v43 as *const i8, _v44 as *const i8) };
// REWRITES-NEXT:         let _v46: i32 = 0;
// REWRITES-NEXT:         let _v47: bool = _v45 == _v46;
// REWRITES-NEXT:     _v47
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v48: bool = false;
// REWRITES-NEXT:     _v48
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v42 as i32);
// REWRITES-NEXT: let _v53: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v55: bool = canonical != (_v53 as *mut i8);
// REWRITES-NEXT: let _v56: bool = if _v55 {
// REWRITES-NEXT:         let _v57: *mut i8 = canonical;
// REWRITES-NEXT:         let _v58: *mut i8 = current.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:         let _v59: i32 = unsafe { strcmp(_v57 as *const i8, _v58 as *const i8) };
// REWRITES-NEXT:         let _v60: i32 = 0;
// REWRITES-NEXT:         let _v61: bool = _v59 == _v60;
// REWRITES-NEXT:     _v61
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v62: bool = false;
// REWRITES-NEXT:     _v62
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v56 as i32);
// REWRITES-NEXT: unsafe { free((directory as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: unsafe { free((canonical as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v70: u64 = 5;
// REWRITES-NEXT: let _v71: u64 = 1;
// REWRITES-NEXT: let mut bi_alloca: Vec<u8> = vec![0; (_v70 + _v71) as usize];
// REWRITES-NEXT: let _v73: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v74: *mut i8 = _v73 as *mut i8;
// REWRITES-NEXT: let _v75: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v76: *mut i8 = unsafe { strcpy(_v74 as *mut i8, _v75 as *const i8) };
// REWRITES-NEXT: let _v77: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v78: i32 = unsafe { strcmp(_v76 as *const i8, _v77 as *const i8) };
// REWRITES-NEXT: let _v79: i32 = 0;
// REWRITES-NEXT: total = total + ((_v78 == _v79) as i32);
// REWRITES-NEXT: let _v84: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v85: u64 = 5;
// REWRITES-NEXT: let _v86: u64 = (unsafe { "slate-truncated".as_bytes().iter().position(|__slate_byte| *__slate_byte == 0u8).unwrap_or("slate-truncated".as_bytes().len()).min((_v85 as usize) as usize) }) as u64;
// REWRITES-NEXT: let _v87: u64 = 1;
// REWRITES-NEXT: let mut bi_alloca2: Vec<u8> = vec![0; (_v86 + _v87) as usize];
// REWRITES-NEXT: let _v89: *mut core::ffi::c_void = bi_alloca2.as_mut_ptr() as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v90: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v91: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v92: u64 = 5;
// REWRITES-NEXT: let _v93: u64 = (unsafe { "slate-truncated".as_bytes().iter().position(|__slate_byte| *__slate_byte == 0u8).unwrap_or("slate-truncated".as_bytes().len()).min((_v92 as usize) as usize) }) as u64;
// REWRITES-NEXT: let _v94: *mut i8 = __slate_strndupa_finish(_v89 as *mut core::ffi::c_void, _v90, _v93);
// REWRITES-NEXT: let _v95: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v96: i32 = unsafe { strcmp(_v94 as *const i8, _v95 as *const i8) };
// REWRITES-NEXT: let _v97: i32 = 0;
// REWRITES-NEXT: total = total + ((_v96 == _v97) as i32);
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_time_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut epoch: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let mut local: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let mut timestamp: i64 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: epoch.tm_year = 70;
// REWRITES-NEXT: epoch.tm_mon = 0;
// REWRITES-NEXT: epoch.tm_mday = 1;
// REWRITES-NEXT: timestamp = unsafe { timegm(std::ptr::addr_of_mut!(epoch) as *mut tm) };
// REWRITES-NEXT: let _v6: i64 = 0;
// REWRITES-NEXT: total = total + ((timestamp == _v6) as i32);
// REWRITES-NEXT: local.tm_year = 70;
// REWRITES-NEXT: local.tm_mon = 0;
// REWRITES-NEXT: local.tm_mday = 2;
// REWRITES-NEXT: let _v14: i64 = unsafe { timelocal(std::ptr::addr_of_mut!(local) as *mut tm) };
// REWRITES-NEXT: let _v15: i64 = -1;
// REWRITES-NEXT: total = total + ((_v14 != _v15) as i32);
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_pattern_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut expression: re_pattern_buffer = re_pattern_buffer { re_nsub: 0, __opaque: std::ptr::null_mut(), __padding: [std::ptr::null_mut(); 4], __nsub2: 0, __padding2: 0 };
// REWRITES-NEXT: let mut paths: glob_t = glob_t { gl_pathc: 0, gl_pathv: std::ptr::null_mut(), gl_offs: 0, __reserved1: 0, __reserved2: [std::ptr::null_mut(); 5] };
// REWRITES-NEXT: let mut error: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"file-+(one|two).c\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = b"file-two.c\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = 32;
// REWRITES-NEXT: let _v4: i32 = unsafe { fnmatch(_v1 as *const i8, _v2 as *const i8, _v3 as i32) };
// REWRITES-NEXT: let _v5: i32 = 0;
// REWRITES-NEXT: total = total + ((_v4 == _v5) as i32);
// REWRITES-NEXT: let _v10: u64 = 1;
// REWRITES-NEXT: let _v11: i32 = 1;
// REWRITES-NEXT: let _v13: i32 = 1;
// REWRITES-NEXT: let _v15: u64 = 1;
// REWRITES-NEXT: let _v16: i32 = 1;
// REWRITES-NEXT: let _v18: i32 = 1;
// REWRITES-NEXT: let _v20: i32 = 1;
// REWRITES-NEXT: let _v22: i32 = 1;
// REWRITES-NEXT: let _v24: i32 = 1;
// REWRITES-NEXT: let _v26: i32 = 1;
// REWRITES-NEXT: let _v29: u64 = 1;
// REWRITES-NEXT: let _v30: i32 = 1;
// REWRITES-NEXT: let _v32: i32 = 1;
// REWRITES-NEXT: let _v34: i32 = 1;
// REWRITES-NEXT: let _v36: i32 = 1;
// REWRITES-NEXT: let _v38: i32 = 1;
// REWRITES-NEXT: let _v40: i32 = 1;
// REWRITES-NEXT: let _v42: i32 = 1;
// REWRITES-NEXT: let _v45: u64 = 1;
// REWRITES-NEXT: let _v46: i32 = 1;
// REWRITES-NEXT: let _v48: i32 = 1;
// REWRITES-NEXT: let _v50: i32 = 1;
// REWRITES-NEXT: let _v52: i32 = 1;
// REWRITES-NEXT: let _v54: i32 = 1;
// REWRITES-NEXT: let _v56: i32 = 1;
// REWRITES-NEXT: let _v58: i32 = 1;
// REWRITES-NEXT: let _v60: i32 = 1;
// REWRITES-NEXT: let _v62: i32 = 1;
// REWRITES-NEXT: let _v65: u64 = 1;
// REWRITES-NEXT: let _v66: i32 = 1;
// REWRITES-NEXT: let _v68: i32 = 1;
// REWRITES-NEXT: let _v70: i32 = 1;
// REWRITES-NEXT: let _v72: i32 = 1;
// REWRITES-NEXT: let _v74: i32 = 1;
// REWRITES-NEXT: let _v76: i32 = 1;
// REWRITES-NEXT: let _v78: i32 = 1;
// REWRITES-NEXT: let _v80: i32 = 1;
// REWRITES-NEXT: let _v82: i32 = 1;
// REWRITES-NEXT: let _v84: i32 = 1;
// REWRITES-NEXT: let _v86: i32 = 1;
// REWRITES-NEXT: let _v88: i32 = 1;
// REWRITES-NEXT: let _v90: i32 = 1;
// REWRITES-NEXT: let _v92: i32 = 1;
// REWRITES-NEXT: let _v94: i32 = 1;
// REWRITES-NEXT: let _v96: i32 = 1;
// REWRITES-NEXT: let _v99: u64 = 1;
// REWRITES-NEXT: let _v100: i32 = 1;
// REWRITES-NEXT: let _v102: i32 = 1;
// REWRITES-NEXT: let _v104: i32 = 1;
// REWRITES-NEXT: let _v107: u64 = 1;
// REWRITES-NEXT: let _v108: i32 = 1;
// REWRITES-NEXT: let _v110: i32 = 1;
// REWRITES-NEXT: let _v112: i32 = 1;
// REWRITES-NEXT: let _v114: i32 = 1;
// REWRITES-NEXT: let _v117: u64 = 1;
// REWRITES-NEXT: let _v118: i32 = 1;
// REWRITES-NEXT: let _v120: i32 = 1;
// REWRITES-NEXT: let _v122: i32 = 1;
// REWRITES-NEXT: let _v124: i32 = 1;
// REWRITES-NEXT: let _v126: i32 = 1;
// REWRITES-NEXT: let _v128: i32 = 1;
// REWRITES-NEXT: let _v130: i32 = 1;
// REWRITES-NEXT: let _v132: i32 = 1;
// REWRITES-NEXT: let _v134: i32 = 1;
// REWRITES-NEXT: let _v136: i32 = 1;
// REWRITES-NEXT: let _v138: i32 = 1;
// REWRITES-NEXT: let _v140: i32 = 1;
// REWRITES-NEXT: let _v143: u64 = 1;
// REWRITES-NEXT: let _v144: i32 = 1;
// REWRITES-NEXT: let _v146: i32 = 1;
// REWRITES-NEXT: let _v148: i32 = 1;
// REWRITES-NEXT: let _v150: i32 = 1;
// REWRITES-NEXT: let _v152: i32 = 1;
// REWRITES-NEXT: let _v154: i32 = 1;
// REWRITES-NEXT: let _v156: i32 = 1;
// REWRITES-NEXT: let _v158: i32 = 1;
// REWRITES-NEXT: let _v160: i32 = 1;
// REWRITES-NEXT: let _v162: i32 = 1;
// REWRITES-NEXT: let _v164: i32 = 1;
// REWRITES-NEXT: let _v166: i32 = 1;
// REWRITES-NEXT: let _v168: i32 = 1;
// REWRITES-NEXT: let _v171: u64 = 1;
// REWRITES-NEXT: let _v172: i32 = 1;
// REWRITES-NEXT: let _v174: i32 = 1;
// REWRITES-NEXT: let _v176: i32 = 1;
// REWRITES-NEXT: let _v178: i32 = 1;
// REWRITES-NEXT: let _v180: i32 = 1;
// REWRITES-NEXT: let _v182: i32 = 1;
// REWRITES-NEXT: let _v184: i32 = 1;
// REWRITES-NEXT: let _v186: i32 = 1;
// REWRITES-NEXT: let _v188: i32 = 1;
// REWRITES-NEXT: let _v190: i32 = 1;
// REWRITES-NEXT: let _v192: i32 = 1;
// REWRITES-NEXT: let _v194: i32 = 1;
// REWRITES-NEXT: let _v196: i32 = 1;
// REWRITES-NEXT: let _v198: i32 = 1;
// REWRITES-NEXT: let _v200: i32 = 1;
// REWRITES-NEXT: let _v203: u64 = 1;
// REWRITES-NEXT: let _v204: i32 = 1;
// REWRITES-NEXT: let _v206: i32 = 1;
// REWRITES-NEXT: let _v208: i32 = 1;
// REWRITES-NEXT: let _v210: i32 = 1;
// REWRITES-NEXT: let _v212: i32 = 1;
// REWRITES-NEXT: let _v215: u64 = 1;
// REWRITES-NEXT: let _v216: i32 = 1;
// REWRITES-NEXT: let _v218: i32 = 1;
// REWRITES-NEXT: let _v220: i32 = 1;
// REWRITES-NEXT: let _v222: i32 = 1;
// REWRITES-NEXT: let _v224: i32 = 1;
// REWRITES-NEXT: let _v226: i32 = 1;
// REWRITES-NEXT: let _v228: i32 = 1;
// REWRITES-NEXT: let _v230: i32 = 1;
// REWRITES-NEXT: let _v232: i32 = 1;
// REWRITES-NEXT: let _v234: i32 = 1;
// REWRITES-NEXT: let _v236: i32 = 1;
// REWRITES-NEXT: let _v238: i32 = 1;
// REWRITES-NEXT: let _v240: i32 = 1;
// REWRITES-NEXT: let _v242: i32 = 1;
// REWRITES-NEXT: let _v244: i32 = 1;
// REWRITES-NEXT: let _v246: i32 = 1;
// REWRITES-NEXT: let _v248: i32 = 1;
// REWRITES-NEXT: let _v251: u64 = unsafe { re_set_syntax((_v10 << _v11 << _v13 | _v15 << _v16 << _v18 << _v20 << _v22 << _v24 << _v26 | _v29 << _v30 << _v32 << _v34 << _v36 << _v38 << _v40 << _v42 | _v45 << _v46 << _v48 << _v50 << _v52 << _v54 << _v56 << _v58 << _v60 << _v62 | _v65 << _v66 << _v68 << _v70 << _v72 << _v74 << _v76 << _v78 << _v80 << _v82 << _v84 << _v86 << _v88 << _v90 << _v92 << _v94 << _v96 | _v99 << _v100 << _v102 << _v104 | _v107 << _v108 << _v110 << _v112 << _v114 | _v117 << _v118 << _v120 << _v122 << _v124 << _v126 << _v128 << _v130 << _v132 << _v134 << _v136 << _v138 << _v140 | _v143 << _v144 << _v146 << _v148 << _v150 << _v152 << _v154 << _v156 << _v158 << _v160 << _v162 << _v164 << _v166 << _v168 | _v171 << _v172 << _v174 << _v176 << _v178 << _v180 << _v182 << _v184 << _v186 << _v188 << _v190 << _v192 << _v194 << _v196 << _v198 << _v200 | _v203 << _v204 << _v206 << _v208 << _v210 << _v212 | _v215 << _v216 << _v218 << _v220 << _v222 << _v224 << _v226 << _v228 << _v230 << _v232 << _v234 << _v236 << _v238 << _v240 << _v242 << _v244 << _v246 << _v248) as u64) };
// REWRITES-NEXT: let _v252: *mut i8 = b"sl(a|e)te\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v253: u64 = 9;
// REWRITES-NEXT: error = (unsafe { re_compile_pattern(_v252 as *const i8, _v253 as usize, std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) }) as *mut i8;
// REWRITES-NEXT: let _v256: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((error == (_v256 as *mut i8)) as i32);
// REWRITES-NEXT: let _v262: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v263: u64 = 5;
// REWRITES-NEXT: let _v264: i64 = 0;
// REWRITES-NEXT: let _v265: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v266: i64 = unsafe { re_match(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer, _v262 as *const i8, _v263 as usize, _v264 as i64, _v265 as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v267: i64 = 5;
// REWRITES-NEXT: total = total + ((_v266 == _v267) as i32);
// REWRITES-NEXT: unsafe { regfree(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) };
// REWRITES-NEXT: let _v272: *mut i8 = b"/dev/{null,zero}\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v273: i32 = 1024;
// REWRITES-NEXT: let _v274: Option<unsafe extern "C" fn(*mut i8, i32) -> i32> = None;
// REWRITES-NEXT: let _v275: i32 = unsafe { glob(_v272 as *const i8, _v273 as i32, _v274, std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// REWRITES-NEXT: let _v276: i32 = 0;
// REWRITES-NEXT: total = total + ((_v275 == _v276) as i32);
// REWRITES-NEXT: let _v282: u64 = 2;
// REWRITES-NEXT: total = total + ((paths.gl_pathc == _v282) as i32);
// REWRITES-NEXT: let _v287: i64 = 0;
// REWRITES-NEXT: let _v288: *mut *mut i8 = paths.gl_pathv;
// REWRITES-NEXT: let _v289: *mut *mut i8 = unsafe { _v288.add(0) };
// REWRITES-NEXT: let _v290: *mut i8 = unsafe { *_v289 };
// REWRITES-NEXT: let _v291: *mut i8 = b"/dev/null\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v292: i32 = unsafe { strcmp(_v290 as *const i8, _v291 as *const i8) };
// REWRITES-NEXT: let _v293: i32 = 0;
// REWRITES-NEXT: total = total + ((_v292 == _v293) as i32);
// REWRITES-NEXT: let _v298: i64 = 1;
// REWRITES-NEXT: let _v299: *mut *mut i8 = paths.gl_pathv;
// REWRITES-NEXT: let _v300: *mut *mut i8 = unsafe { _v299.add(1) };
// REWRITES-NEXT: let _v301: *mut i8 = unsafe { *_v300 };
// REWRITES-NEXT: let _v302: *mut i8 = b"/dev/zero\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v303: i32 = unsafe { strcmp(_v301 as *const i8, _v302 as *const i8) };
// REWRITES-NEXT: let _v304: i32 = 0;
// REWRITES-NEXT: total = total + ((_v303 == _v304) as i32);
// REWRITES-NEXT: unsafe { globfree(std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn gnu_runtime_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut random_bytes: [u8; 8] = [0; 8];
// REWRITES-NEXT: let mut frames: aligned::Aligned<aligned::A16, [*mut core::ffi::c_void; 8]> = aligned::Aligned([std::ptr::null_mut(); 8]);
// REWRITES-NEXT: let mut information: Dl_info = Dl_info { dli_fname: std::ptr::null_mut(), dli_fbase: std::ptr::null_mut(), dli_sname: std::ptr::null_mut(), dli_saddr: std::ptr::null_mut() };
// REWRITES-NEXT: let mut page_size: i64 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: let _v0: i32 = 30;
// REWRITES-NEXT: page_size = unsafe { sysconf(_v0 as i32) };
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: let _v3: u64 = 6;
// REWRITES-NEXT: let _v4: u64 = unsafe { getauxval(_v3 as u64) };
// REWRITES-NEXT: total = total + ((_v4 == (page_size as u64)) as i32);
// REWRITES-NEXT: let _v11: i32 = unsafe { gettid() };
// REWRITES-NEXT: let _v12: i64 = 186;
// REWRITES-NEXT: let _v13: i64 = unsafe { syscall(_v12 as i64) };
// REWRITES-NEXT: total = total + ((_v11 == (_v13 as i32)) as i32);
// REWRITES-NEXT: let _v19: *mut u8 = random_bytes.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v21: u64 = 8;
// REWRITES-NEXT: let _v22: i32 = unsafe { getentropy((_v19 as *mut core::ffi::c_void) as *mut core::ffi::c_void, _v21 as usize) };
// REWRITES-NEXT: let _v23: i32 = 0;
// REWRITES-NEXT: total = total + ((_v22 == _v23) as i32);
// REWRITES-NEXT: let _v28: u32 = 1;
// REWRITES-NEXT: let _v29: u32 = unsafe { arc4random_uniform(_v28 as u32) };
// REWRITES-NEXT: let _v30: u32 = 0;
// REWRITES-NEXT: total = total + ((_v29 == _v30) as i32);
// REWRITES-NEXT: let _v35: i32 = unsafe { get_nprocs() };
// REWRITES-NEXT: let _v36: i32 = 0;
// REWRITES-NEXT: total = total + ((_v35 > _v36) as i32);
// REWRITES-NEXT: let _v41: i64 = unsafe { get_phys_pages() };
// REWRITES-NEXT: let _v42: i64 = 0;
// REWRITES-NEXT: total = total + ((_v41 > _v42) as i32);
// REWRITES-NEXT: let _v47: *mut *mut core::ffi::c_void = frames.as_mut_ptr() as *mut *mut core::ffi::c_void;
// REWRITES-NEXT: let _v48: i32 = 8;
// REWRITES-NEXT: let _v49: i32 = unsafe { backtrace(_v47 as *mut *mut core::ffi::c_void, _v48 as i32) };
// REWRITES-NEXT: let _v50: i32 = 0;
// REWRITES-NEXT: total = total + ((_v49 > _v50) as i32);
// REWRITES-NEXT: let _v55: *mut core::ffi::c_void = unsafe { std::mem::transmute::<Option<unsafe extern "C" fn() -> i32>, *mut core::ffi::c_void>(Some(gnu_runtime_extensions)) };
// REWRITES-NEXT: let _v56: i32 = unsafe { dladdr(_v55 as *const core::ffi::c_void, std::ptr::addr_of_mut!(information) as *mut Dl_info) };
// REWRITES-NEXT: let _v57: i32 = 0;
// REWRITES-NEXT: total = total + ((_v56 != _v57) as i32);
// REWRITES-NEXT: let _v63: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((information.dli_fname != (_v63 as *mut i8)) as i32);
// REWRITES-NEXT: let _v69: i64 = 0;
// REWRITES-NEXT: let _v70: *mut i8 = (unsafe { gnu_get_libc_version() }) as *mut i8;
// REWRITES-NEXT: let _v71: *mut i8 = unsafe { _v70.add(0) };
// REWRITES-NEXT: let _v74: i32 = 0;
// REWRITES-NEXT: total = total + ((((unsafe { *_v71 }) as i32) != _v74) as i32);
// REWRITES-NEXT: let _v80: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: total = total + (((unsafe { program_invocation_name }) != (_v80 as *mut i8)) as i32);
// REWRITES-NEXT: let _v87: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: total = total + (((unsafe { program_invocation_short_name }) != (_v87 as *mut i8)) as i32);
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = gnu_environment_extensions();
// REWRITES-NEXT: let _v3: i32 = gnu_time_extensions();
// REWRITES-NEXT: let _v4: i32 = gnu_pattern_extensions();
// REWRITES-NEXT: let _v5: i32 = gnu_runtime_extensions();
// REWRITES-NEXT: let _v7: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4 + _v5) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
