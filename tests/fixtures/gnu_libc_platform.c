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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Dl_info {
// COMMON-LOWERING-NEXT:     dli_fbase: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     dli_saddr: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct glob_t {
// COMMON-LOWERING-NEXT:     gl_pathc: u64,
// COMMON-LOWERING-NEXT:     gl_offs: u64,
// COMMON-LOWERING-NEXT:     __reserved1: i32,
// COMMON-LOWERING-NEXT:     __reserved2: [*mut core::ffi::c_void; 5],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct re_pattern_buffer {
// COMMON-LOWERING-NEXT:     re_nsub: u64,
// COMMON-LOWERING-NEXT:     __opaque: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     __padding: [*mut core::ffi::c_void; 4],
// COMMON-LOWERING-NEXT:     __nsub2: u64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct tm {
// COMMON-LOWERING-NEXT:     tm_sec: i32,
// COMMON-LOWERING-NEXT:     tm_min: i32,
// COMMON-LOWERING-NEXT:     tm_hour: i32,
// COMMON-LOWERING-NEXT:     tm_mday: i32,
// COMMON-LOWERING-NEXT:     tm_mon: i32,
// COMMON-LOWERING-NEXT:     tm_year: i32,
// COMMON-LOWERING-NEXT:     tm_wday: i32,
// COMMON-LOWERING-NEXT:     tm_yday: i32,
// COMMON-LOWERING-NEXT:     tm_isdst: i32,
// COMMON-LOWERING-NEXT:     tm_gmtoff: i64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn setenv(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn secure_getenv(_0: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn unsetenv(_0: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn get_current_dir_name() -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn canonicalize_file_name(_0: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn getcwd(_0: *mut core::ffi::c_char, _1: usize) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     fn strcpy(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn strnlen(_0: *const core::ffi::c_char, _1: usize) -> usize;
// COMMON-LOWERING-NEXT:     fn memcpy(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn timegm(_0: *mut tm) -> i64;
// COMMON-LOWERING-NEXT:     fn timelocal(_0: *mut tm) -> i64;
// COMMON-LOWERING-NEXT:     fn fnmatch(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn re_set_syntax(_0: u64) -> u64;
// COMMON-LOWERING-NEXT:     fn re_compile_pattern(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: *mut re_pattern_buffer,
// COMMON-LOWERING-NEXT:     ) -> *const core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn re_match(
// COMMON-LOWERING-NEXT:         _0: *mut re_pattern_buffer,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:         _3: i64,
// COMMON-LOWERING-NEXT:         _4: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     ) -> i64;
// COMMON-LOWERING-NEXT:     fn regfree(_0: *mut re_pattern_buffer);
// COMMON-LOWERING-NEXT:     fn glob(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: i32,
// COMMON-LOWERING-NEXT:         _3: *mut glob_t,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn globfree(_0: *mut glob_t);
// COMMON-LOWERING-NEXT:     fn sysconf(_0: i32) -> i64;
// COMMON-LOWERING-NEXT:     fn getauxval(_0: u64) -> u64;
// COMMON-LOWERING-NEXT:     fn gettid() -> i32;
// COMMON-LOWERING-NEXT:     fn syscall(_0: i64, ...) -> i64;
// COMMON-LOWERING-NEXT:     fn getentropy(_0: *mut core::ffi::c_void, _1: usize) -> i32;
// COMMON-LOWERING-NEXT:     fn arc4random_uniform(_0: u32) -> u32;
// COMMON-LOWERING-NEXT:     fn get_nprocs() -> i32;
// COMMON-LOWERING-NEXT:     fn get_phys_pages() -> i64;
// COMMON-LOWERING-NEXT:     fn backtrace(_0: *mut *mut core::ffi::c_void, _1: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn dladdr(_0: *const core::ffi::c_void, _1: *mut Dl_info) -> i32;
// COMMON-LOWERING-NEXT:     fn gnu_get_libc_version() -> *const core::ffi::c_char;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_environment_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_time_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_pattern_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_runtime_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_environment_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         setenv(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { unsetenv({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     directory = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     canonical = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4096;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:             strcmp(
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:             strcmp(
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{__v[0-9]+}} as usize];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:         strcpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strnlen({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as usize) }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let mut bi_alloca2: Vec<u8> = vec![0; {{__v[0-9]+}} as usize];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca2.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strnlen({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as usize) }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_time_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut epoch: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut local: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     epoch = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     local = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 70;
// COMMON-LOWERING-NEXT:     epoch.tm_year = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     epoch.tm_mon = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     epoch.tm_mday = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { timegm(std::ptr::addr_of_mut!(epoch) as *mut tm) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 70;
// COMMON-LOWERING-NEXT:     local.tm_year = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     local.tm_mon = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     local.tm_mday = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { timelocal(std::ptr::addr_of_mut!(local) as *mut tm) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = -1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_pattern_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut expression: re_pattern_buffer = re_pattern_buffer {
// COMMON-LOWERING-NEXT:         re_nsub: 0,
// COMMON-LOWERING-NEXT:         __opaque: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         __padding: [std::ptr::null_mut(); 4],
// COMMON-LOWERING-NEXT:         __nsub2: 0,
// COMMON-LOWERING-NEXT:         __padding2: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut paths: glob_t = glob_t {
// COMMON-LOWERING-NEXT:         gl_pathc: 0,
// COMMON-LOWERING-NEXT:         gl_pathv: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         gl_offs: 0,
// COMMON-LOWERING-NEXT:         __reserved1: 0,
// COMMON-LOWERING-NEXT:         __reserved2: [std::ptr::null_mut(); 5],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: re_pattern_buffer = re_pattern_buffer {
// COMMON-LOWERING-NEXT:         re_nsub: 0,
// COMMON-LOWERING-NEXT:         __opaque: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         __padding: [std::ptr::null_mut(); 4],
// COMMON-LOWERING-NEXT:         __nsub2: 0,
// COMMON-LOWERING-NEXT:         __padding2: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     expression = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: glob_t = glob_t {
// COMMON-LOWERING-NEXT:         gl_pathc: 0,
// COMMON-LOWERING-NEXT:         gl_pathv: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         gl_offs: 0,
// COMMON-LOWERING-NEXT:         __reserved1: 0,
// COMMON-LOWERING-NEXT:         __reserved2: [std::ptr::null_mut(); 5],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     paths = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         fnmatch(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { re_set_syntax({{__v[0-9]+}} as u64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 9;
// COMMON-LOWERING-NEXT:         re_compile_pattern(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-LOWERING-NEXT:         re_match(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i64,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe { regfree(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1024;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         glob(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(paths) as *mut glob_t,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = paths.gl_pathc;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe { globfree(std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn gnu_runtime_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut random_bytes: [u8; 8] = [0; 8];
// COMMON-LOWERING-NEXT:     let mut information: Dl_info = Dl_info {
// COMMON-LOWERING-NEXT:         dli_fname: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         dli_fbase: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         dli_sname: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         dli_saddr: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Dl_info = Dl_info {
// COMMON-LOWERING-NEXT:         dli_fname: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         dli_fbase: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         dli_sname: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         dli_saddr: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     information = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 30;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { sysconf({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { getauxval({{__v[0-9]+}} as u64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { gettid() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { syscall({{__v[0-9]+}} as i64) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = random_bytes.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { getentropy({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { arc4random_uniform({{__v[0-9]+}} as u32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { get_nprocs() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { get_phys_pages() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut *mut core::ffi::c_void = frames.as_mut_ptr() as *mut *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { backtrace({{__v[0-9]+}} as *mut *mut core::ffi::c_void, {{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> i32>, *mut core::ffi::c_void>(
// COMMON-LOWERING-NEXT:             Some(gnu_runtime_extensions),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         dladdr(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(information) as *mut Dl_info,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         memcpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     dli_fname: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     dli_sname: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     gl_pathv: *mut *mut i8,
// LOWERING-X86_64-GNU-NEXT:     __padding2: i8,
// LOWERING-X86_64-GNU-NEXT:     tm_zone: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     static mut program_invocation_name: *mut i8;
// LOWERING-X86_64-GNU-NEXT:     static mut program_invocation_short_name: *mut i8;
// LOWERING-X86_64-GNU-NEXT:         _2: Option<unsafe extern "C-unwind" fn(*mut i8, i32) -> i32>,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut directory: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut canonical: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut current: aligned::Aligned<aligned::A16, [i8; 4096]> = aligned::Aligned([0; 4096]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { secure_getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { get_current_dir_name() }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b".\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { canonicalize_file_name({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { getcwd({{__v[0-9]+}} as *mut core::ffi::c_char, {{__v[0-9]+}} as usize) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = directory;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = directory;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = canonical;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = canonical;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = directory;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = canonical;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate-truncated\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = __slate_strndupa_finish({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"file-+(one|two).c\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"file-two.c\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"sl(a|e)te\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"/dev/{null,zero}\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut i8, i32) -> i32> = None;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = paths.gl_pathv;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"/dev/null\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = paths.gl_pathv;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"/dev/zero\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut frames: aligned::Aligned<aligned::A16, [*mut core::ffi::c_void; 8]> =
// LOWERING-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 8]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 186;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = information.dli_fname;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { gnu_get_libc_version() }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { program_invocation_name };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { program_invocation_short_name };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT: fn __slate_strndupa_finish({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{arg[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add({{arg[0-9]+}} as usize) };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     dli_fname: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     dli_sname: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     gl_pathv: *mut *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     __padding2: u8,
// LOWERING-AARCH64-GNU-NEXT:     tm_zone: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     static mut program_invocation_name: *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     static mut program_invocation_short_name: *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         _2: Option<unsafe extern "C-unwind" fn(*mut u8, i32) -> i32>,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut directory: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut canonical: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut current: [u8; 4096] = [0; 4096];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ready\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { secure_getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ready\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GNU_LIBC_VALUE\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { get_current_dir_name() }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b".\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { canonicalize_file_name({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = current.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { getcwd({{__v[0-9]+}} as *mut core::ffi::c_char, {{__v[0-9]+}} as usize) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = directory;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = directory;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = current.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = canonical;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = canonical;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = current.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = directory;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = canonical;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate-truncated\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate-truncated\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate-truncated\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = __slate_strndupa_finish({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"file-+(one|two).c\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"file-two.c\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"sl(a|e)te\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"/dev/{null,zero}\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut u8, i32) -> i32> = None;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = paths.gl_pathv;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"/dev/null\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = paths.gl_pathv;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"/dev/zero\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut frames: [*mut core::ffi::c_void; 8] = [std::ptr::null_mut(); 8];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 178;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = information.dli_fname;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { gnu_get_libc_version() }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { program_invocation_name };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { program_invocation_short_name };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT: fn __slate_strndupa_finish({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: u64) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{arg[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 0;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{arg[0-9]+}} as usize) };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Dl_info {
// COMMON-REWRITES-NEXT:     dli_fbase: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     dli_saddr: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct glob_t {
// COMMON-REWRITES-NEXT:     gl_pathc: u64,
// COMMON-REWRITES-NEXT:     gl_offs: u64,
// COMMON-REWRITES-NEXT:     __reserved1: i32,
// COMMON-REWRITES-NEXT:     __reserved2: [*mut core::ffi::c_void; 5],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct re_pattern_buffer {
// COMMON-REWRITES-NEXT:     re_nsub: u64,
// COMMON-REWRITES-NEXT:     __opaque: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     __padding: [*mut core::ffi::c_void; 4],
// COMMON-REWRITES-NEXT:     __nsub2: u64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct tm {
// COMMON-REWRITES-NEXT:     tm_sec: i32,
// COMMON-REWRITES-NEXT:     tm_min: i32,
// COMMON-REWRITES-NEXT:     tm_hour: i32,
// COMMON-REWRITES-NEXT:     tm_mday: i32,
// COMMON-REWRITES-NEXT:     tm_mon: i32,
// COMMON-REWRITES-NEXT:     tm_year: i32,
// COMMON-REWRITES-NEXT:     tm_wday: i32,
// COMMON-REWRITES-NEXT:     tm_yday: i32,
// COMMON-REWRITES-NEXT:     tm_isdst: i32,
// COMMON-REWRITES-NEXT:     tm_gmtoff: i64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn setenv(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn secure_getenv(_0: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn unsetenv(_0: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn get_current_dir_name() -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn canonicalize_file_name(_0: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn getcwd(_0: *mut core::ffi::c_char, _1: usize) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     fn strcpy(_0: *mut core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn strnlen(_0: *const core::ffi::c_char, _1: usize) -> usize;
// COMMON-REWRITES-NEXT:     fn memcpy(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn timegm(_0: *mut tm) -> i64;
// COMMON-REWRITES-NEXT:     fn timelocal(_0: *mut tm) -> i64;
// COMMON-REWRITES-NEXT:     fn fnmatch(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn re_set_syntax(_0: u64) -> u64;
// COMMON-REWRITES-NEXT:     fn re_compile_pattern(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: *mut re_pattern_buffer,
// COMMON-REWRITES-NEXT:     ) -> *const core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn re_match(
// COMMON-REWRITES-NEXT:         _0: *mut re_pattern_buffer,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:         _3: i64,
// COMMON-REWRITES-NEXT:         _4: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     ) -> i64;
// COMMON-REWRITES-NEXT:     fn regfree(_0: *mut re_pattern_buffer);
// COMMON-REWRITES-NEXT:     fn glob(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: i32,
// COMMON-REWRITES-NEXT:         _3: *mut glob_t,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn globfree(_0: *mut glob_t);
// COMMON-REWRITES-NEXT:     fn sysconf(_0: i32) -> i64;
// COMMON-REWRITES-NEXT:     fn getauxval(_0: u64) -> u64;
// COMMON-REWRITES-NEXT:     fn gettid() -> i32;
// COMMON-REWRITES-NEXT:     fn syscall(_0: i64, ...) -> i64;
// COMMON-REWRITES-NEXT:     fn getentropy(_0: *mut core::ffi::c_void, _1: usize) -> i32;
// COMMON-REWRITES-NEXT:     fn arc4random_uniform(_0: u32) -> u32;
// COMMON-REWRITES-NEXT:     fn get_nprocs() -> i32;
// COMMON-REWRITES-NEXT:     fn get_phys_pages() -> i64;
// COMMON-REWRITES-NEXT:     fn backtrace(_0: *mut *mut core::ffi::c_void, _1: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn dladdr(_0: *const core::ffi::c_void, _1: *mut Dl_info) -> i32;
// COMMON-REWRITES-NEXT:     fn gnu_get_libc_version() -> *const core::ffi::c_char;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = gnu_environment_extensions();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = gnu_time_extensions();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = gnu_pattern_extensions();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = gnu_runtime_extensions();
// COMMON-REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}} + {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_environment_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         setenv(
// COMMON-REWRITES-NEXT:             c"SLATE_GNU_LIBC_VALUE".as_ptr(),
// COMMON-REWRITES-NEXT:             c"ready".as_ptr(),
// COMMON-REWRITES-NEXT:             1 as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"ready".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { unsetenv(c"SLATE_GNU_LIBC_VALUE".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:             strcmp(
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:             strcmp(
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     unsafe { free(directory as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     unsafe { free(canonical as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-REWRITES-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; (5 + {{__v[0-9]+}}) as usize];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"slate".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         "slate-truncated"
// COMMON-REWRITES-NEXT:             .as_bytes()
// COMMON-REWRITES-NEXT:             .iter()
// COMMON-REWRITES-NEXT:             .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:             .unwrap_or("slate-truncated".as_bytes().len())
// COMMON-REWRITES-NEXT:             .min((5 as u64) as usize)
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     let mut bi_alloca2: Vec<u8> = vec![0; ({{__v[0-9]+}} + 1) as usize];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca2.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         "slate-truncated"
// COMMON-REWRITES-NEXT:             .as_bytes()
// COMMON-REWRITES-NEXT:             .iter()
// COMMON-REWRITES-NEXT:             .position(|__slate_byte| *__slate_byte == 0u8)
// COMMON-REWRITES-NEXT:             .unwrap_or("slate-truncated".as_bytes().len())
// COMMON-REWRITES-NEXT:             .min((5 as u64) as usize)
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"slate".as_ptr()) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_time_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut epoch: tm = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut local: tm = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     epoch = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     local = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     epoch.tm_year = 70;
// COMMON-REWRITES-NEXT:     epoch.tm_mon = 0;
// COMMON-REWRITES-NEXT:     epoch.tm_mday = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { timegm(std::ptr::addr_of_mut!(epoch) as *mut tm) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     local.tm_year = 70;
// COMMON-REWRITES-NEXT:     local.tm_mon = 0;
// COMMON-REWRITES-NEXT:     local.tm_mday = 2;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { timelocal(std::ptr::addr_of_mut!(local) as *mut tm) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} != -1) as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_pattern_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut expression: re_pattern_buffer = re_pattern_buffer {
// COMMON-REWRITES-NEXT:         re_nsub: 0,
// COMMON-REWRITES-NEXT:         __opaque: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         __padding: [std::ptr::null_mut(); 4],
// COMMON-REWRITES-NEXT:         __nsub2: 0,
// COMMON-REWRITES-NEXT:         __padding2: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut paths: glob_t = glob_t {
// COMMON-REWRITES-NEXT:         gl_pathc: 0,
// COMMON-REWRITES-NEXT:         gl_pathv: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         gl_offs: 0,
// COMMON-REWRITES-NEXT:         __reserved1: 0,
// COMMON-REWRITES-NEXT:         __reserved2: [std::ptr::null_mut(); 5],
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         fnmatch(
// COMMON-REWRITES-NEXT:             c"file-+(one|two).c".as_ptr(),
// COMMON-REWRITES-NEXT:             c"file-two.c".as_ptr(),
// COMMON-REWRITES-NEXT:             32 as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         re_set_syntax(
// COMMON-REWRITES-NEXT:             (1 << (1 as i32) << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32) << (1 as i32) << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// COMMON-REWRITES-NEXT:                 | 1 << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)
// COMMON-REWRITES-NEXT:                     << (1 as i32)) as u64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         re_compile_pattern(
// COMMON-REWRITES-NEXT:             c"sl(a|e)te".as_ptr(),
// COMMON-REWRITES-NEXT:             (9 as u64) as usize,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-REWRITES-NEXT:         re_match(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer,
// COMMON-REWRITES-NEXT:             c"slate".as_ptr(),
// COMMON-REWRITES-NEXT:             (5 as u64) as usize,
// COMMON-REWRITES-NEXT:             0 as i64,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 5) as i32);
// COMMON-REWRITES-NEXT:     unsafe { regfree(std::ptr::addr_of_mut!(expression) as *mut re_pattern_buffer) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         glob(
// COMMON-REWRITES-NEXT:             c"/dev/{null,zero}".as_ptr(),
// COMMON-REWRITES-NEXT:             1024 as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(paths) as *mut glob_t,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32) + ((paths.gl_pathc == 2) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         strcmp(
// COMMON-REWRITES-NEXT:             (unsafe { *{{__v[0-9]+}} }) as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             c"/dev/null".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         strcmp(
// COMMON-REWRITES-NEXT:             (unsafe { *{{__v[0-9]+}} }) as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             c"/dev/zero".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     unsafe { globfree(std::ptr::addr_of_mut!(paths) as *mut glob_t) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn gnu_runtime_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut random_bytes: [u8; 8] = [0; 8];
// COMMON-REWRITES-NEXT:     let mut information: Dl_info = Dl_info {
// COMMON-REWRITES-NEXT:         dli_fname: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         dli_fbase: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         dli_sname: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         dli_saddr: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     information = Dl_info {
// COMMON-REWRITES-NEXT:         dli_fname: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         dli_fbase: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         dli_sname: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         dli_saddr: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { sysconf(30 as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { getauxval(6 as u64) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == ({{__v[0-9]+}} as u64)) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { gettid() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == ({{__v[0-9]+}} as i32)) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = random_bytes.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { getentropy({{__v[0-9]+}} as *mut core::ffi::c_void, (8 as u64) as usize) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { arc4random_uniform(1 as u32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { get_nprocs() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { get_phys_pages() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut *mut core::ffi::c_void = frames.as_mut_ptr() as *mut *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { backtrace({{__v[0-9]+}} as *mut *mut core::ffi::c_void, 8 as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> i32>, *mut core::ffi::c_void>(
// COMMON-REWRITES-NEXT:             Some(gnu_runtime_extensions),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         dladdr(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(information) as *mut Dl_info,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 =
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         + ((((unsafe { *{{__v[0-9]+}} }) as i32) != 0) as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         memcpy(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{arg[0-9]+}} as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     dli_fname: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     dli_sname: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     gl_pathv: *mut *mut i8,
// REWRITES-X86_64-GNU-NEXT:     __padding2: i8,
// REWRITES-X86_64-GNU-NEXT:     tm_zone: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     static mut program_invocation_name: *mut i8;
// REWRITES-X86_64-GNU-NEXT:     static mut program_invocation_short_name: *mut i8;
// REWRITES-X86_64-GNU-NEXT:         _2: Option<unsafe extern "C-unwind" fn(*mut i8, i32) -> i32>,
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let mut directory: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut canonical: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut current: aligned::Aligned<aligned::A16, [i8; 4096]> = aligned::Aligned([0; 4096]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { secure_getenv(c"SLATE_GNU_LIBC_VALUE".as_ptr()) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     directory = (unsafe { get_current_dir_name() }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     canonical = (unsafe { canonicalize_file_name(c".".as_ptr()) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { getcwd({{__v[0-9]+}} as *mut core::ffi::c_char, (4096 as u64) as usize) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != ({{__v[0-9]+}} as *mut i8)) as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = directory != ({{__v[0-9]+}} as *mut i8);
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = directory;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = canonical != ({{__v[0-9]+}} as *mut i8);
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = canonical;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = current.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strcpy({{__v[0-9]+}} as *mut core::ffi::c_char, c"slate".as_ptr()) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"slate-truncated".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = __slate_strndupa_finish({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == ({{__v[0-9]+}} as *mut i8)) as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut i8, i32) -> i32> = None;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = paths.gl_pathv;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = paths.gl_pathv;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-X86_64-GNU-NEXT:     let mut frames: aligned::Aligned<aligned::A16, [*mut core::ffi::c_void; 8]> =
// REWRITES-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 8]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { syscall(186 as i64) };
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32) + ((information.dli_fname != ({{__v[0-9]+}} as *mut i8)) as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { gnu_get_libc_version() }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-X86_64-GNU-NEXT:         + (((unsafe { program_invocation_name }) != ({{__v[0-9]+}} as *mut i8)) as i32)
// REWRITES-X86_64-GNU-NEXT:         + (((unsafe { program_invocation_short_name }) != ({{__v[0-9]+}} as *mut i8)) as i32)
// REWRITES-X86_64-GNU-NEXT: fn __slate_strndupa_finish({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{arg[0-9]+}} as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 0;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add({{arg[0-9]+}} as usize) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     dli_fname: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     dli_sname: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     gl_pathv: *mut *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     __padding2: u8,
// REWRITES-AARCH64-GNU-NEXT:     tm_zone: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     static mut program_invocation_name: *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     static mut program_invocation_short_name: *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         _2: Option<unsafe extern "C-unwind" fn(*mut u8, i32) -> i32>,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut directory: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let mut canonical: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let mut current: [u8; 4096] = [0; 4096];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { secure_getenv(c"SLATE_GNU_LIBC_VALUE".as_ptr()) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     directory = (unsafe { get_current_dir_name() }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     canonical = (unsafe { canonicalize_file_name(c".".as_ptr()) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = current.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { getcwd({{__v[0-9]+}} as *mut core::ffi::c_char, (4096 as u64) as usize) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != ({{__v[0-9]+}} as *mut u8)) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: bool = directory != ({{__v[0-9]+}} as *mut u8);
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = directory;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = current.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: bool = canonical != ({{__v[0-9]+}} as *mut u8);
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = canonical;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = current.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strcpy({{__v[0-9]+}} as *mut core::ffi::c_char, c"slate".as_ptr()) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"slate-truncated".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = __slate_strndupa_finish({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == ({{__v[0-9]+}} as *mut u8)) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut u8, i32) -> i32> = None;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = paths.gl_pathv;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = paths.gl_pathv;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-AARCH64-GNU-NEXT:     let mut frames: [*mut core::ffi::c_void; 8] = [std::ptr::null_mut(); 8];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { syscall(178 as i64) };
// REWRITES-AARCH64-GNU-NEXT:         {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32) + ((information.dli_fname != ({{__v[0-9]+}} as *mut u8)) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { gnu_get_libc_version() }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-AARCH64-GNU-NEXT:         + (((unsafe { program_invocation_name }) != ({{__v[0-9]+}} as *mut u8)) as i32)
// REWRITES-AARCH64-GNU-NEXT:         + (((unsafe { program_invocation_short_name }) != ({{__v[0-9]+}} as *mut u8)) as i32)
// REWRITES-AARCH64-GNU-NEXT: fn __slate_strndupa_finish({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: u64) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{arg[0-9]+}} as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 0;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{arg[0-9]+}} as usize) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
