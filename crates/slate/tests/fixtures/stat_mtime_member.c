#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat info = {0};
  if (stat("/dev/null", &info) != 0)
    return 1;
  printf("%lld\n", (long long)info.st_mtime);
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct stat {
// LOWERING-NEXT:     st_dev: u64,
// LOWERING-NEXT:     st_ino: u64,
// LOWERING-NEXT:     st_nlink: u64,
// LOWERING-NEXT:     st_mode: u32,
// LOWERING-NEXT:     st_uid: u32,
// LOWERING-NEXT:     st_gid: u32,
// LOWERING-NEXT:     __pad0: u32,
// LOWERING-NEXT:     st_rdev: u64,
// LOWERING-NEXT:     st_size: i64,
// LOWERING-NEXT:     st_blksize: i64,
// LOWERING-NEXT:     st_blocks: i64,
// LOWERING-NEXT:     st_atim: libc::timespec,
// LOWERING-NEXT:     st_mtim: libc::timespec,
// LOWERING-NEXT:     st_ctim: libc::timespec,
// LOWERING-NEXT:     __unused: [i64; 3],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn stat(_0: *const core::ffi::c_char, _1: *mut stat) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut info: stat = stat {
// LOWERING-NEXT:         st_dev: 0,
// LOWERING-NEXT:         st_ino: 0,
// LOWERING-NEXT:         st_nlink: 0,
// LOWERING-NEXT:         st_mode: 0,
// LOWERING-NEXT:         st_uid: 0,
// LOWERING-NEXT:         st_gid: 0,
// LOWERING-NEXT:         __pad0: 0,
// LOWERING-NEXT:         st_rdev: 0,
// LOWERING-NEXT:         st_size: 0,
// LOWERING-NEXT:         st_blksize: 0,
// LOWERING-NEXT:         st_blocks: 0,
// LOWERING-NEXT:         st_atim: libc::timespec {
// LOWERING-NEXT:             tv_sec: 0,
// LOWERING-NEXT:             tv_nsec: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         st_mtim: libc::timespec {
// LOWERING-NEXT:             tv_sec: 0,
// LOWERING-NEXT:             tv_nsec: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         st_ctim: libc::timespec {
// LOWERING-NEXT:             tv_sec: 0,
// LOWERING-NEXT:             tv_nsec: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         __unused: [0; 3],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     info = stat {
// LOWERING-NEXT:         st_dev: 0,
// LOWERING-NEXT:         st_ino: 0,
// LOWERING-NEXT:         st_nlink: 0,
// LOWERING-NEXT:         st_mode: 0,
// LOWERING-NEXT:         st_uid: 0,
// LOWERING-NEXT:         st_gid: 0,
// LOWERING-NEXT:         __pad0: 0,
// LOWERING-NEXT:         st_rdev: 0,
// LOWERING-NEXT:         st_size: 0,
// LOWERING-NEXT:         st_blksize: 0,
// LOWERING-NEXT:         st_blocks: 0,
// LOWERING-NEXT:         st_atim: libc::timespec {
// LOWERING-NEXT:             tv_sec: 0,
// LOWERING-NEXT:             tv_nsec: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         st_mtim: libc::timespec {
// LOWERING-NEXT:             tv_sec: 0,
// LOWERING-NEXT:             tv_nsec: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         st_ctim: libc::timespec {
// LOWERING-NEXT:             tv_sec: 0,
// LOWERING-NEXT:             tv_nsec: 0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         __unused: [0; 3],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"/dev/null\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:             stat(
// LOWERING-NEXT:                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 std::ptr::addr_of_mut!(info) as *mut stat,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%lld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = info.st_mtim.tv_sec;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct stat {
// REWRITES-NEXT:     st_dev: u64,
// REWRITES-NEXT:     st_ino: u64,
// REWRITES-NEXT:     st_nlink: u64,
// REWRITES-NEXT:     st_mode: u32,
// REWRITES-NEXT:     st_uid: u32,
// REWRITES-NEXT:     st_gid: u32,
// REWRITES-NEXT:     __pad0: u32,
// REWRITES-NEXT:     st_rdev: u64,
// REWRITES-NEXT:     st_size: i64,
// REWRITES-NEXT:     st_blksize: i64,
// REWRITES-NEXT:     st_blocks: i64,
// REWRITES-NEXT:     st_atim: libc::timespec,
// REWRITES-NEXT:     st_mtim: libc::timespec,
// REWRITES-NEXT:     st_ctim: libc::timespec,
// REWRITES-NEXT:     __unused: [i64; 3],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn stat(_0: *const core::ffi::c_char, _1: *mut stat) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut info: stat = stat {
// REWRITES-NEXT:         st_dev: 0,
// REWRITES-NEXT:         st_ino: 0,
// REWRITES-NEXT:         st_nlink: 0,
// REWRITES-NEXT:         st_mode: 0,
// REWRITES-NEXT:         st_uid: 0,
// REWRITES-NEXT:         st_gid: 0,
// REWRITES-NEXT:         __pad0: 0,
// REWRITES-NEXT:         st_rdev: 0,
// REWRITES-NEXT:         st_size: 0,
// REWRITES-NEXT:         st_blksize: 0,
// REWRITES-NEXT:         st_blocks: 0,
// REWRITES-NEXT:         st_atim: libc::timespec {
// REWRITES-NEXT:             tv_sec: 0,
// REWRITES-NEXT:             tv_nsec: 0,
// REWRITES-NEXT:         },
// REWRITES-NEXT:         st_mtim: libc::timespec {
// REWRITES-NEXT:             tv_sec: 0,
// REWRITES-NEXT:             tv_nsec: 0,
// REWRITES-NEXT:         },
// REWRITES-NEXT:         st_ctim: libc::timespec {
// REWRITES-NEXT:             tv_sec: 0,
// REWRITES-NEXT:             tv_nsec: 0,
// REWRITES-NEXT:         },
// REWRITES-NEXT:         __unused: [0; 3],
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         stat(
// REWRITES-NEXT:             c"/dev/null".as_ptr(),
// REWRITES-NEXT:             std::ptr::addr_of_mut!(info) as *mut stat,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         __retval = 1;
// REWRITES-NEXT:         std::process::exit(__retval as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = info.st_mtim.tv_sec;
// REWRITES-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), {{_v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
