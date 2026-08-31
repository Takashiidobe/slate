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
// LOWERING-NEXT: fn __slate_strndupa_finish({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> *mut i8 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{arg[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memcpy({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{arg[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add({{arg[0-9]+}} as usize) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_environment_extensions() -> i32 {
// LOWERING-NEXT:     let mut directory: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut canonical: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut current: aligned::Aligned<aligned::A16, [i8; 4096]> = aligned::Aligned([0; 4096]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { setenv({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { secure_getenv({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { unsetenv({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { get_current_dir_name() };
// LOWERING-NEXT:     directory = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b".\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { canonicalize_file_name({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     canonical = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4096;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { getcwd({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = directory;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = directory;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = canonical;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = canonical;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = directory;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = canonical;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{_v[0-9]+}} as usize];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strcpy({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { strnlen({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let mut bi_alloca2: Vec<u8> = vec![0; {{_v[0-9]+}} as usize];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca2.as_mut_ptr() as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { strnlen({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = __slate_strndupa_finish({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_time_extensions() -> i32 {
// LOWERING-NEXT:     let mut epoch: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut local: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     epoch = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     local = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 70;
// LOWERING-NEXT:     epoch.tm_year = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     epoch.tm_mon = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     epoch.tm_mday = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { timegm(std::ptr::addr_of_mut!(epoch) as *mut tm) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 70;
// LOWERING-NEXT:     local.tm_year = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     local.tm_mon = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     local.tm_mday = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { timelocal(std::ptr::addr_of_mut!(local) as *mut tm) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = -1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_pattern_extensions() -> i32 {
// LOWERING-NEXT:     let mut expression: re_pattern_buffer = re_pattern_buffer { re_nsub: 0, __opaque: std::ptr::null_mut(), __padding: [std::ptr::null_mut(); 4], __nsub2: 0, __padding2: 0 };
// LOWERING-NEXT:     let mut paths: glob_t = glob_t { gl_pathc: 0, gl_pathv: std::ptr::null_mut(), gl_offs: 0, __reserved1: 0, __reserved2: [std::ptr::null_mut(); 5] };
// LOWERING-NEXT:     expression = re_pattern_buffer { re_nsub: 0, __opaque: std::ptr::null_mut(), __padding: [std::ptr::null_mut(); 4], __nsub2: 0, __padding2: 0 };
// LOWERING-NEXT:     paths = glob_t { gl_pathc: 0, gl_pathv: std::ptr::null_mut(), gl_offs: 0, __reserved1: 0, __reserved2: [std::ptr::null_mut(); 5] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"file-+(one|two).c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"file-two.c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { fnmatch({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { re_set_syntax({{_v[0-9]+}} as u64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"sl(a|e)te\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 9;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe { re_compile_pattern({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { re_match(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as i64, {{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe { regfree(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"/dev/{null,zero}\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1024;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut i8, i32) -> i32> = None;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { glob({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32, {{_v[0-9]+}}, std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = paths.gl_pathc;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = paths.gl_pathv;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"/dev/null\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = paths.gl_pathv;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"/dev/zero\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe { globfree(std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn gnu_runtime_extensions() -> i32 {
// LOWERING-NEXT:     let mut random_bytes: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let mut frames: aligned::Aligned<aligned::A16, [*mut core::ffi::c_void; 8]> = aligned::Aligned([std::ptr::null_mut(); 8]);
// LOWERING-NEXT:     let mut information: Dl_info = Dl_info { dli_fname: std::ptr::null_mut(), dli_fbase: std::ptr::null_mut(), dli_sname: std::ptr::null_mut(), dli_saddr: std::ptr::null_mut() };
// LOWERING-NEXT:     information = Dl_info { dli_fname: std::ptr::null_mut(), dli_fbase: std::ptr::null_mut(), dli_sname: std::ptr::null_mut(), dli_saddr: std::ptr::null_mut() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 30;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { sysconf({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { getauxval({{_v[0-9]+}} as u64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { gettid() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 186;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { syscall({{_v[0-9]+}} as i64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = random_bytes.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { getentropy({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { arc4random_uniform({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { get_nprocs() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { get_phys_pages() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = frames.as_mut_ptr() as *mut *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { backtrace({{_v[0-9]+}} as *mut *mut core::ffi::c_void, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { std::mem::transmute::<Option<unsafe extern "C" fn() -> i32>, *mut core::ffi::c_void>(Some(gnu_runtime_extensions)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { dladdr({{_v[0-9]+}} as *const core::ffi::c_void, std::ptr::addr_of_mut!(information) as *mut Dl_info) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = information.dli_fname;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe { gnu_get_libc_version() }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { program_invocation_name };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { program_invocation_short_name };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_environment_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_time_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_pattern_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_runtime_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: fn __slate_strndupa_finish({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> *mut i8 {
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{arg[0-9]+}} as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memcpy(({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut core::ffi::c_void, ({{arg[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void, {{arg[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i8 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add({{arg[0-9]+}} as usize) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_environment_extensions() -> i32 {
// REWRITES-NEXT: let mut directory: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut canonical: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut current: aligned::Aligned<aligned::A16, [i8; 4096]> = aligned::Aligned([0; 4096]);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { setenv({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { secure_getenv({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { unsetenv({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: directory = unsafe { get_current_dir_name() };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b".\0".as_ptr() as *mut i8;
// REWRITES-NEXT: canonical = unsafe { canonicalize_file_name({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4096;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { getcwd({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != ({{_v[0-9]+}} as *mut i8)) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = directory != ({{_v[0-9]+}} as *mut i8);
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = directory;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = canonical != ({{_v[0-9]+}} as *mut i8);
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = canonical;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: unsafe { free((directory as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: unsafe { free((canonical as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let mut bi_alloca: Vec<u8> = vec![0; ({{_v[0-9]+}} + {{_v[0-9]+}}) as usize];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { strcpy({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = (unsafe { "slate-truncated".as_bytes().iter().position(|__slate_byte| *__slate_byte == 0u8).unwrap_or("slate-truncated".as_bytes().len()).min(({{_v[0-9]+}} as usize) as usize) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let mut bi_alloca2: Vec<u8> = vec![0; ({{_v[0-9]+}} + {{_v[0-9]+}}) as usize];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca2.as_mut_ptr() as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = (unsafe { "slate-truncated".as_bytes().iter().position(|__slate_byte| *__slate_byte == 0u8).unwrap_or("slate-truncated".as_bytes().len()).min(({{_v[0-9]+}} as usize) as usize) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = __slate_strndupa_finish({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: return {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_time_extensions() -> i32 {
// REWRITES-NEXT: let mut epoch: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let mut local: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: epoch.tm_year = 70;
// REWRITES-NEXT: epoch.tm_mon = 0;
// REWRITES-NEXT: epoch.tm_mday = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { timegm(std::ptr::addr_of_mut!(epoch) as *mut tm) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: local.tm_year = 70;
// REWRITES-NEXT: local.tm_mon = 0;
// REWRITES-NEXT: local.tm_mday = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { timelocal(std::ptr::addr_of_mut!(local) as *mut tm) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = -1;
// REWRITES-NEXT: return {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_pattern_extensions() -> i32 {
// REWRITES-NEXT: let mut expression: re_pattern_buffer = re_pattern_buffer { re_nsub: 0, __opaque: std::ptr::null_mut(), __padding: [std::ptr::null_mut(); 4], __nsub2: 0, __padding2: 0 };
// REWRITES-NEXT: let mut paths: glob_t = glob_t { gl_pathc: 0, gl_pathv: std::ptr::null_mut(), gl_offs: 0, __reserved1: 0, __reserved2: [std::ptr::null_mut(); 5] };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"file-+(one|two).c\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"file-two.c\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { fnmatch({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = unsafe { re_set_syntax(({{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}} << {{_v[0-9]+}}) as u64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"sl(a|e)te\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 9;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = (unsafe { re_compile_pattern({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) }) as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == ({{_v[0-9]+}} as *mut i8)) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { re_match(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as i64, {{_v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: unsafe { regfree(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"/dev/{null,zero}\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1024;
// REWRITES-NEXT: let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut i8, i32) -> i32> = None;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { glob({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32, {{_v[0-9]+}}, std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + ((paths.gl_pathc == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut *mut i8 = paths.gl_pathv;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"/dev/null\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut *mut i8 = paths.gl_pathv;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"/dev/zero\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: unsafe { globfree(std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// REWRITES-NEXT: return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn gnu_runtime_extensions() -> i32 {
// REWRITES-NEXT: let mut random_bytes: [u8; 8] = [0; 8];
// REWRITES-NEXT: let mut frames: aligned::Aligned<aligned::A16, [*mut core::ffi::c_void; 8]> = aligned::Aligned([std::ptr::null_mut(); 8]);
// REWRITES-NEXT: let mut information: Dl_info = Dl_info { dli_fname: std::ptr::null_mut(), dli_fbase: std::ptr::null_mut(), dli_sname: std::ptr::null_mut(), dli_saddr: std::ptr::null_mut() };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 30;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { sysconf({{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 6;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = unsafe { getauxval({{_v[0-9]+}} as u64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == ({{_v[0-9]+}} as u64)) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { gettid() };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 186;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { syscall({{_v[0-9]+}} as i64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == ({{_v[0-9]+}} as i32)) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = random_bytes.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { getentropy(({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { arc4random_uniform({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { get_nprocs() };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} > {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { get_phys_pages() };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} > {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = frames.as_mut_ptr() as *mut *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { backtrace({{_v[0-9]+}} as *mut *mut core::ffi::c_void, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} > {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { std::mem::transmute::<Option<unsafe extern "C" fn() -> i32>, *mut core::ffi::c_void>(Some(gnu_runtime_extensions)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { dladdr({{_v[0-9]+}} as *const core::ffi::c_void, std::ptr::addr_of_mut!(information) as *mut Dl_info) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32) + ((information.dli_fname != ({{_v[0-9]+}} as *mut i8)) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = (unsafe { gnu_get_libc_version() }) as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: return {{_v[0-9]+}} + ((((unsafe { *{{_v[0-9]+}} }) as i32) != {{_v[0-9]+}}) as i32) + (((unsafe { program_invocation_name }) != ({{_v[0-9]+}} as *mut i8)) as i32) + (((unsafe { program_invocation_short_name }) != ({{_v[0-9]+}} as *mut i8)) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = gnu_environment_extensions();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = gnu_time_extensions();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = gnu_pattern_extensions();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = gnu_runtime_extensions();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
