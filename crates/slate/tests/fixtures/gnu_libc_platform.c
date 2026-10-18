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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_alloca {
// CHECK-NEXT:     use std::alloc::{Layout, alloc, dealloc, handle_alloc_error};
// CHECK-NEXT:     use std::cell::UnsafeCell;
// CHECK-NEXT:     const ALIGN: usize = 16;
// CHECK-NEXT:     const BLOCK: usize = 1 << 20;
// CHECK-NEXT:     struct Block {
// CHECK-NEXT:         base: *mut u8,
// CHECK-NEXT:         size: usize,
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Block {
// CHECK-NEXT:         fn new(size: usize) -> Self {
// CHECK-NEXT:             let layout = Layout::from_size_align(size, ALIGN).unwrap();
// CHECK-NEXT:             let base = unsafe { alloc(layout) };
// CHECK-NEXT:             if base.is_null() {
// CHECK-NEXT:                 handle_alloc_error(layout);
// CHECK-NEXT:             }
// CHECK-NEXT:             Block { base, size }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Drop for Block {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 dealloc(self.base, Layout::from_size_align_unchecked(self.size, ALIGN))
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     struct Stack {
// CHECK-NEXT:         blocks: Vec<Block>,
// CHECK-NEXT:         block: usize,
// CHECK-NEXT:         offset: usize,
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Stack {
// CHECK-NEXT:         fn allocate(&mut self, size: usize, align: usize) -> *mut u8 {
// CHECK-NEXT:             if self.blocks.is_empty() {
// CHECK-NEXT:                 self.blocks.push(Block::new(BLOCK));
// CHECK-NEXT:             }
// CHECK-NEXT:             let needed = size.saturating_add(align);
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 let block = &self.blocks[self.block];
// CHECK-NEXT:                 let base = block.base as usize;
// CHECK-NEXT:                 let start = (base + self.offset).next_multiple_of(align) - base;
// CHECK-NEXT:                 if start <= block.size && block.size - start >= size {
// CHECK-NEXT:                     self.offset = start + size;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         let pointer = block.base.add(start);
// CHECK-NEXT:                         pointer.write_bytes(0, size);
// CHECK-NEXT:                         return pointer;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 self.block += 1;
// CHECK-NEXT:                 self.offset = 0;
// CHECK-NEXT:                 if self.block == self.blocks.len()
// CHECK-NEXT:                     || self.blocks[self.block].size < needed
// CHECK-NEXT:                 {
// CHECK-NEXT:                     self.blocks.truncate(self.block);
// CHECK-NEXT:                     self.blocks
// CHECK-NEXT:                         .push(Block::new(needed.next_multiple_of(ALIGN).max(BLOCK)));
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     thread_local! {
// CHECK-NEXT:         static STACK : UnsafeCell < Stack > = const { UnsafeCell::new(Stack { blocks :
// CHECK-NEXT:         Vec::new(), block : 0, offset : 0 }) };
// CHECK-NEXT:     }
// CHECK-NEXT:     pub struct Mark {
// CHECK-NEXT:         block: usize,
// CHECK-NEXT:         offset: usize,
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Mark {
// CHECK-NEXT:         #[inline]
// CHECK-NEXT:         pub fn new() -> Self {
// CHECK-NEXT:             STACK
// CHECK-NEXT:                 .with(|stack| {
// CHECK-NEXT:                     let stack = unsafe { &*stack.get() };
// CHECK-NEXT:                     Mark {
// CHECK-NEXT:                         block: stack.block,
// CHECK-NEXT:                         offset: stack.offset,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 })
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Drop for Mark {
// CHECK-NEXT:         #[inline]
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             STACK
// CHECK-NEXT:                 .with(|stack| {
// CHECK-NEXT:                     let stack = unsafe { &mut *stack.get() };
// CHECK-NEXT:                     stack.block = self.block;
// CHECK-NEXT:                     stack.offset = self.offset;
// CHECK-NEXT:                 })
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[inline]
// CHECK-NEXT:     pub fn alloca(size: usize, align: usize) -> *mut std::ffi::c_void {
// CHECK-NEXT:         STACK
// CHECK-NEXT:             .with(|stack| unsafe { (*stack.get()).allocate(size, align.max(ALIGN)) })
// CHECK-NEXT:             .cast()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut program_invocation_name: *mut i8;
// CHECK-NEXT:     static mut program_invocation_short_name: *mut i8;
// CHECK-NEXT:     fn dladdr(__address: *const (), __info: *mut [[SLATERECORD_0:__SlateRecord[0-9]+]]) -> i32;
// CHECK-NEXT:     fn backtrace(__array: *mut *mut (), __size: i32) -> i32;
// CHECK-NEXT:     fn fnmatch(__pattern: *const i8, __name: *const i8, __flags: i32) -> i32;
// CHECK-NEXT:     fn glob(
// CHECK-NEXT:         __pattern: *const i8,
// CHECK-NEXT:         __flags: i32,
// CHECK-NEXT:         __errfunc: Option<unsafe extern "C-unwind" fn(*const i8, i32) -> i32>,
// CHECK-NEXT:         __pglob: *mut [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn globfree(__pglob: *mut [[SLATERECORD_1]]);
// CHECK-NEXT:     fn gnu_get_libc_version() -> *const i8;
// CHECK-NEXT:     fn re_set_syntax(__syntax: u64) -> u64;
// CHECK-NEXT:     fn re_compile_pattern(
// CHECK-NEXT:         __pattern: *const i8,
// CHECK-NEXT:         __length: u64,
// CHECK-NEXT:         __buffer: *mut re_pattern_buffer,
// CHECK-NEXT:     ) -> *const i8;
// CHECK-NEXT:     fn re_match(
// CHECK-NEXT:         __buffer: *mut re_pattern_buffer,
// CHECK-NEXT:         __String: *const i8,
// CHECK-NEXT:         __length: i32,
// CHECK-NEXT:         __start: i32,
// CHECK-NEXT:         __regs: *mut re_registers,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn regfree(__preg: *mut re_pattern_buffer);
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn arc4random_uniform(__upper_bound: u32) -> u32;
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT:     fn secure_getenv(__name: *const i8) -> *mut i8;
// CHECK-NEXT:     fn setenv(__name: *const i8, __value: *const i8, __replace: i32) -> i32;
// CHECK-NEXT:     fn unsetenv(__name: *const i8) -> i32;
// CHECK-NEXT:     fn canonicalize_file_name(__name: *const i8) -> *mut i8;
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn strcmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT:     fn strnlen(__string: *const i8, __maxlen: u64) -> u64;
// CHECK-NEXT:     fn getauxval(__type: u64) -> u64;
// CHECK-NEXT:     fn getentropy(__buffer: *mut (), __length: u64) -> i32;
// CHECK-NEXT:     fn get_nprocs() -> i32;
// CHECK-NEXT:     fn get_phys_pages() -> i64;
// CHECK-NEXT:     fn timegm(__tp: *mut tm) -> i64;
// CHECK-NEXT:     fn timelocal(__tp: *mut tm) -> i64;
// CHECK-NEXT:     fn getcwd(__buf: *mut i8, __size: u64) -> *mut i8;
// CHECK-NEXT:     fn get_current_dir_name() -> *mut i8;
// CHECK-NEXT:     fn sysconf(__name: i32) -> i64;
// CHECK-NEXT:     fn syscall(__sysno: i64, ...) -> i64;
// CHECK-NEXT:     fn gettid() -> i32;
// CHECK-NEXT:     #[link_name = "alloca"]
// CHECK-NEXT:     fn __builtin_alloca([[V_0:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     dli_fname: *const i8,
// CHECK-NEXT:     dli_fbase: *mut (),
// CHECK-NEXT:     dli_sname: *const i8,
// CHECK-NEXT:     dli_saddr: *mut (),
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct stat {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1]] {
// CHECK-NEXT:     gl_pathc: u64,
// CHECK-NEXT:     gl_pathv: *mut *mut i8,
// CHECK-NEXT:     gl_offs: u64,
// CHECK-NEXT:     gl_flags: i32,
// CHECK-NEXT:     gl_closedir: Option<unsafe extern "C-unwind" fn(*mut ())>,
// CHECK-NEXT:     gl_readdir: Option<unsafe extern "C-unwind" fn(*mut ()) -> *mut dirent>,
// CHECK-NEXT:     gl_opendir: Option<unsafe extern "C-unwind" fn(*const i8) -> *mut ()>,
// CHECK-NEXT:     gl_lstat: Option<unsafe extern "C-unwind" fn(*const i8, *mut stat) -> i32>,
// CHECK-NEXT:     gl_stat: Option<unsafe extern "C-unwind" fn(*const i8, *mut stat) -> i32>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct dirent {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct re_pattern_buffer {
// CHECK-NEXT:     buffer: *mut re_dfa_t,
// CHECK-NEXT:     allocated: u64,
// CHECK-NEXT:     used: u64,
// CHECK-NEXT:     syntax: u64,
// CHECK-NEXT:     fastmap: *mut i8,
// CHECK-NEXT:     translate: *mut u8,
// CHECK-NEXT:     re_nsub: u64,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits17U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct re_dfa_t {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct re_registers {
// CHECK-NEXT:     num_regs: u32,
// CHECK-NEXT:     start: *mut i32,
// CHECK-NEXT:     end: *mut i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tm {
// CHECK-NEXT:     tm_sec: i32,
// CHECK-NEXT:     tm_min: i32,
// CHECK-NEXT:     tm_hour: i32,
// CHECK-NEXT:     tm_mday: i32,
// CHECK-NEXT:     tm_mon: i32,
// CHECK-NEXT:     tm_year: i32,
// CHECK-NEXT:     tm_wday: i32,
// CHECK-NEXT:     tm_yday: i32,
// CHECK-NEXT:     tm_isdst: i32,
// CHECK-NEXT:     tm_gmtoff: i64,
// CHECK-NEXT:     tm_zone: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits17U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub can_be_null: u32,
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub regs_allocated: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub fastmap_accurate: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub no_sub: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub not_bol: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub not_eol: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub newline_anchor: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_environment_extensions() -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let mut current: __SlateAlign16<[i8; 4096]> = __SlateAlign16([0 as i8; 4096]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     total
// CHECK-NEXT:         += ((unsafe {
// CHECK-NEXT:             setenv(c"SLATE_GNU_LIBC_VALUE".as_ptr(), c"ready".as_ptr(), 1 as i32)
// CHECK-NEXT:         }) == 0) as i32;
// CHECK-NEXT:     total
// CHECK-NEXT:         += ((unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 secure_getenv(c"SLATE_GNU_LIBC_VALUE".as_ptr()) as *const i8,
// CHECK-NEXT:                 c"ready".as_ptr(),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32;
// CHECK-NEXT:     total += ((unsafe { unsetenv(c"SLATE_GNU_LIBC_VALUE".as_ptr()) }) == 0) as i32;
// CHECK-NEXT:     let directory: *mut i8 = unsafe { get_current_dir_name() };
// CHECK-NEXT:     let canonical: *mut i8 = unsafe { canonicalize_file_name(c".".as_ptr()) };
// CHECK-NEXT:     total
// CHECK-NEXT:         += ((unsafe { getcwd(current.0.as_mut_ptr() as *mut i8, 4096 as u64) })
// CHECK-NEXT:             != std::ptr::null_mut::<i8>()) as i32;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_1]]
// CHECK-NEXT:         + ((directory != std::ptr::null_mut::<i8>()
// CHECK-NEXT:             && (unsafe {
// CHECK-NEXT:                 strcmp(directory as *const i8, current.0.as_mut_ptr() as *const i8)
// CHECK-NEXT:             }) == 0) as i32);
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_2]]
// CHECK-NEXT:         + ((canonical != std::ptr::null_mut::<i8>()
// CHECK-NEXT:             && (unsafe {
// CHECK-NEXT:                 strcmp(canonical as *const i8, current.0.as_mut_ptr() as *const i8)
// CHECK-NEXT:             }) == 0) as i32);
// CHECK-NEXT:     unsafe { free(directory as *mut ()) };
// CHECK-NEXT:     unsafe { free(canonical as *mut ()) };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut i8;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __old: *const i8 = c"slate".as_ptr();
// CHECK-NEXT:         let __len: u64 = unsafe { strlen(__old) }.wrapping_add(1);
// CHECK-NEXT:         let __new: *mut i8 = (__slate_alloca::alloca(__len as usize, 16usize) as *mut ())
// CHECK-NEXT:             as *mut i8;
// CHECK-NEXT:         [[V_3]] = (unsafe {
// CHECK-NEXT:             {
// CHECK-NEXT:                 std::ptr::copy_nonoverlapping(
// CHECK-NEXT:                     __old as *const u8,
// CHECK-NEXT:                     __new as *mut u8,
// CHECK-NEXT:                     __len as usize,
// CHECK-NEXT:                 );
// CHECK-NEXT:                 __new as *mut ()
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     total += ((unsafe { strcmp([[V_3]] as *const i8, c"slate".as_ptr()) }) == 0) as i32;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: *mut i8;
// CHECK-NEXT:     let __old: *const i8 = c"slate-truncated".as_ptr();
// CHECK-NEXT:     let __len: u64 = unsafe { 5 };
// CHECK-NEXT:     let __new: *mut i8 = (__slate_alloca::alloca(__len.wrapping_add(1) as usize, 16usize)
// CHECK-NEXT:         as *mut ()) as *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *__new.offset(__len as isize) = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_4]] = (unsafe {
// CHECK-NEXT:         {
// CHECK-NEXT:             std::ptr::copy_nonoverlapping(
// CHECK-NEXT:                 __old as *const u8,
// CHECK-NEXT:                 __new as *mut u8,
// CHECK-NEXT:                 __len as usize,
// CHECK-NEXT:             );
// CHECK-NEXT:             __new as *mut ()
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = ((unsafe { strcmp([[V_4]] as *const i8, c"slate".as_ptr()) }) == 0)
// CHECK-NEXT:         as i32;
// CHECK-NEXT:     total += [[V_5]];
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_time_extensions() -> i32 {
// CHECK-NEXT:     let mut epoch: tm = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: tm = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut local: tm = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: tm = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut timestamp: i64 = 0;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     epoch.tm_year = 70;
// CHECK-NEXT:     epoch.tm_mon = 0;
// CHECK-NEXT:     epoch.tm_mday = 1;
// CHECK-NEXT:     timestamp = unsafe { timegm(std::ptr::addr_of_mut!(epoch)) };
// CHECK-NEXT:     total += (timestamp == 0) as i32;
// CHECK-NEXT:     local.tm_year = 70;
// CHECK-NEXT:     local.tm_mon = 0;
// CHECK-NEXT:     local.tm_mday = 2;
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = ((unsafe { timelocal(std::ptr::addr_of_mut!(local)) }) != -1)
// CHECK-NEXT:         as i32;
// CHECK-NEXT:     total += [[V_6]];
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_pattern_extensions() -> i32 {
// CHECK-NEXT:     let mut expression: re_pattern_buffer = {
// CHECK-NEXT:         let mut [[T_0]]: re_pattern_buffer = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut paths: [[SLATERECORD_1]] = {
// CHECK-NEXT:         let mut [[T_1]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         fnmatch(
// CHECK-NEXT:             c"file-+(one|two).c".as_ptr(),
// CHECK-NEXT:             c"file-two.c".as_ptr(),
// CHECK-NEXT:             (1 as i32) << (5 as i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 0) as i32;
// CHECK-NEXT:     total += [[V_7]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         re_set_syntax(
// CHECK-NEXT:             (1 as u64) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32)
// CHECK-NEXT:                 | (1 as u64) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32) << (1 as i32)
// CHECK-NEXT:                     << (1 as i32) << (1 as i32) << (1 as i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let error: *const i8 = unsafe {
// CHECK-NEXT:         re_compile_pattern(
// CHECK-NEXT:             c"sl(a|e)te".as_ptr(),
// CHECK-NEXT:             9 as u64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(expression),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_8]] + ((error == std::ptr::null::<i8>()) as i32);
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         re_match(
// CHECK-NEXT:             std::ptr::addr_of_mut!(expression),
// CHECK-NEXT:             c"slate".as_ptr(),
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             std::ptr::null_mut::<re_registers>(),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 5) as i32;
// CHECK-NEXT:     total += [[V_9]];
// CHECK-NEXT:     unsafe { regfree(std::ptr::addr_of_mut!(expression)) };
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         glob(
// CHECK-NEXT:             c"/dev/{null,zero}".as_ptr(),
// CHECK-NEXT:             (1 as i32) << (10 as i32),
// CHECK-NEXT:             None,
// CHECK-NEXT:             std::ptr::addr_of_mut!(paths),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 0) as i32;
// CHECK-NEXT:     total += [[V_10]];
// CHECK-NEXT:     total += (paths.gl_pathc == 2) as i32;
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_11]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 *paths.gl_pathv.offset(0 as isize) as *const i8,
// CHECK-NEXT:                 c"/dev/null".as_ptr(),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_12]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 *paths.gl_pathv.offset(1 as isize) as *const i8,
// CHECK-NEXT:                 c"/dev/zero".as_ptr(),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     unsafe { globfree(std::ptr::addr_of_mut!(paths)) };
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.gnu_runtime_extensions")]
// CHECK-NEXT: extern "C-unwind" fn gnu_runtime_extensions() -> i32 {
// CHECK-NEXT:     let mut random_bytes: [u8; 8] = [0 as u8; 8];
// CHECK-NEXT:     let mut frames: __SlateAlign16<[*mut (); 8]> = __SlateAlign16([0 as *mut (); 8]);
// CHECK-NEXT:     let mut information: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let page_size: i64 = unsafe { sysconf(30 as i32) };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: i32 = ((unsafe { getauxval(6 as u64) }) == (page_size as u64)) as i32;
// CHECK-NEXT:     total += [[V_13]];
// CHECK-NEXT:     let [[V_14:__v[0-9]+]]: i32 = ((unsafe { gettid() })
// CHECK-NEXT:         == ((unsafe { syscall(186 as i64) }) as i32)) as i32;
// CHECK-NEXT:     total += [[V_14]];
// CHECK-NEXT:     let [[V_15:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         getentropy(random_bytes.as_mut_ptr() as *mut (), 8 as u64)
// CHECK-NEXT:     }) == 0) as i32;
// CHECK-NEXT:     total += [[V_15]];
// CHECK-NEXT:     let [[V_16:__v[0-9]+]]: i32 = ((unsafe { arc4random_uniform(1 as u32) }) == 0) as i32;
// CHECK-NEXT:     total += [[V_16]];
// CHECK-NEXT:     let [[V_17:__v[0-9]+]]: i32 = ((unsafe { get_nprocs() }) > 0) as i32;
// CHECK-NEXT:     total += [[V_17]];
// CHECK-NEXT:     let [[V_18:__v[0-9]+]]: i32 = ((unsafe { get_phys_pages() }) > 0) as i32;
// CHECK-NEXT:     total += [[V_18]];
// CHECK-NEXT:     let [[V_19:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         backtrace(frames.0.as_mut_ptr() as *mut *mut (), 8 as i32)
// CHECK-NEXT:     }) > 0) as i32;
// CHECK-NEXT:     total += [[V_19]];
// CHECK-NEXT:     let [[V_20:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         dladdr(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                     *mut (),
// CHECK-NEXT:                 >(Some(gnu_runtime_extensions))
// CHECK-NEXT:             }) as *const (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(information),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != 0) as i32;
// CHECK-NEXT:     total += [[V_20]];
// CHECK-NEXT:     let [[V_21:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_21]] + ((information.dli_fname != std::ptr::null::<i8>()) as i32);
// CHECK-NEXT:     let [[V_22:__v[0-9]+]]: i32 = (((unsafe { *gnu_get_libc_version().offset(0 as isize) }) as i32)
// CHECK-NEXT:         != 0) as i32;
// CHECK-NEXT:     total += [[V_22]];
// CHECK-NEXT:     let [[V_23:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_23]]
// CHECK-NEXT:         + (((unsafe { program_invocation_name }) != std::ptr::null_mut::<i8>()) as i32);
// CHECK-NEXT:     let [[V_24:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_24]]
// CHECK-NEXT:         + (((unsafe { program_invocation_short_name }) != std::ptr::null_mut::<i8>())
// CHECK-NEXT:             as i32);
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", gnu_environment_extensions(), gnu_time_extensions(),
// CHECK-NEXT:         gnu_pattern_extensions() + gnu_runtime_extensions()
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
