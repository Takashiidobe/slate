#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat info   = {0};
  int         result = stat("/dev/null", &info);
  printf("%d %lld\n", result, (long long)info.st_size);
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
// COMMON-LOWERING-NEXT: struct stat {
// COMMON-LOWERING-NEXT:     st_dev: u64,
// COMMON-LOWERING-NEXT:     st_ino: u64,
// COMMON-LOWERING-NEXT:     st_nlink: u64,
// COMMON-LOWERING-NEXT:     st_mode: u32,
// COMMON-LOWERING-NEXT:     st_uid: u32,
// COMMON-LOWERING-NEXT:     st_gid: u32,
// COMMON-LOWERING-NEXT:     st_rdev: u64,
// COMMON-LOWERING-NEXT:     st_size: i64,
// COMMON-LOWERING-NEXT:     st_blksize: i64,
// COMMON-LOWERING-NEXT:     st_blocks: i64,
// COMMON-LOWERING-NEXT:     st_atim: libc::timespec,
// COMMON-LOWERING-NEXT:     st_mtim: libc::timespec,
// COMMON-LOWERING-NEXT:     st_ctim: libc::timespec,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn stat(_0: *const core::ffi::c_char, _1: *mut stat) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut info: stat = stat {
// COMMON-LOWERING-NEXT:         st_dev: 0,
// COMMON-LOWERING-NEXT:         st_ino: 0,
// COMMON-LOWERING-NEXT:         st_nlink: 0,
// COMMON-LOWERING-NEXT:         st_mode: 0,
// COMMON-LOWERING-NEXT:         st_uid: 0,
// COMMON-LOWERING-NEXT:         st_gid: 0,
// COMMON-LOWERING-NEXT:         st_rdev: 0,
// COMMON-LOWERING-NEXT:         st_size: 0,
// COMMON-LOWERING-NEXT:         st_blksize: 0,
// COMMON-LOWERING-NEXT:         st_blocks: 0,
// COMMON-LOWERING-NEXT:         st_atim: libc::timespec {
// COMMON-LOWERING-NEXT:             tv_sec: 0,
// COMMON-LOWERING-NEXT:             tv_nsec: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         st_mtim: libc::timespec {
// COMMON-LOWERING-NEXT:             tv_sec: 0,
// COMMON-LOWERING-NEXT:             tv_nsec: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         st_ctim: libc::timespec {
// COMMON-LOWERING-NEXT:             tv_sec: 0,
// COMMON-LOWERING-NEXT:             tv_nsec: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: stat = stat {
// COMMON-LOWERING-NEXT:         st_dev: 0,
// COMMON-LOWERING-NEXT:         st_ino: 0,
// COMMON-LOWERING-NEXT:         st_nlink: 0,
// COMMON-LOWERING-NEXT:         st_mode: 0,
// COMMON-LOWERING-NEXT:         st_uid: 0,
// COMMON-LOWERING-NEXT:         st_gid: 0,
// COMMON-LOWERING-NEXT:         st_rdev: 0,
// COMMON-LOWERING-NEXT:         st_size: 0,
// COMMON-LOWERING-NEXT:         st_blksize: 0,
// COMMON-LOWERING-NEXT:         st_blocks: 0,
// COMMON-LOWERING-NEXT:         st_atim: libc::timespec {
// COMMON-LOWERING-NEXT:             tv_sec: 0,
// COMMON-LOWERING-NEXT:             tv_nsec: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         st_mtim: libc::timespec {
// COMMON-LOWERING-NEXT:             tv_sec: 0,
// COMMON-LOWERING-NEXT:             tv_nsec: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         st_ctim: libc::timespec {
// COMMON-LOWERING-NEXT:             tv_sec: 0,
// COMMON-LOWERING-NEXT:             tv_nsec: 0,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     info = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         stat(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(info) as *mut stat,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = info.st_size;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     __pad0: u32,
// LOWERING-X86_64-GNU-NEXT:     __unused: [i64; 3],
// LOWERING-X86_64-GNU-NEXT:         __pad0: 0,
// LOWERING-X86_64-GNU-NEXT:         __unused: [0; 3],
// LOWERING-X86_64-GNU-NEXT:         __pad0: 0,
// LOWERING-X86_64-GNU-NEXT:         __unused: [0; 3],
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"/dev/null\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %lld\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     __pad: u64,
// LOWERING-AARCH64-GNU-NEXT:     __pad2: i32,
// LOWERING-AARCH64-GNU-NEXT:     __unused: [u32; 2],
// LOWERING-AARCH64-GNU-NEXT:         __pad: 0,
// LOWERING-AARCH64-GNU-NEXT:         __pad2: 0,
// LOWERING-AARCH64-GNU-NEXT:         __unused: [0; 2],
// LOWERING-AARCH64-GNU-NEXT:         __pad: 0,
// LOWERING-AARCH64-GNU-NEXT:         __pad2: 0,
// LOWERING-AARCH64-GNU-NEXT:         __unused: [0; 2],
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"/dev/null\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %lld\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: struct stat {
// COMMON-REWRITES-NEXT:     st_dev: u64,
// COMMON-REWRITES-NEXT:     st_ino: u64,
// COMMON-REWRITES-NEXT:     st_nlink: u64,
// COMMON-REWRITES-NEXT:     st_mode: u32,
// COMMON-REWRITES-NEXT:     st_uid: u32,
// COMMON-REWRITES-NEXT:     st_gid: u32,
// COMMON-REWRITES-NEXT:     st_rdev: u64,
// COMMON-REWRITES-NEXT:     st_size: i64,
// COMMON-REWRITES-NEXT:     st_blksize: i64,
// COMMON-REWRITES-NEXT:     st_blocks: i64,
// COMMON-REWRITES-NEXT:     st_atim: libc::timespec,
// COMMON-REWRITES-NEXT:     st_mtim: libc::timespec,
// COMMON-REWRITES-NEXT:     st_ctim: libc::timespec,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn stat(_0: *const core::ffi::c_char, _1: *mut stat) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut info: stat = stat {
// COMMON-REWRITES-NEXT:         st_dev: 0,
// COMMON-REWRITES-NEXT:         st_ino: 0,
// COMMON-REWRITES-NEXT:         st_nlink: 0,
// COMMON-REWRITES-NEXT:         st_mode: 0,
// COMMON-REWRITES-NEXT:         st_uid: 0,
// COMMON-REWRITES-NEXT:         st_gid: 0,
// COMMON-REWRITES-NEXT:         st_rdev: 0,
// COMMON-REWRITES-NEXT:         st_size: 0,
// COMMON-REWRITES-NEXT:         st_blksize: 0,
// COMMON-REWRITES-NEXT:         st_blocks: 0,
// COMMON-REWRITES-NEXT:         st_atim: libc::timespec {
// COMMON-REWRITES-NEXT:             tv_sec: 0,
// COMMON-REWRITES-NEXT:             tv_nsec: 0,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         st_mtim: libc::timespec {
// COMMON-REWRITES-NEXT:             tv_sec: 0,
// COMMON-REWRITES-NEXT:             tv_nsec: 0,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         st_ctim: libc::timespec {
// COMMON-REWRITES-NEXT:             tv_sec: 0,
// COMMON-REWRITES-NEXT:             tv_nsec: 0,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = info.st_size;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %lld\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 stat(
// COMMON-REWRITES-NEXT:                     c"/dev/null".as_ptr(),
// COMMON-REWRITES-NEXT:                     std::ptr::addr_of_mut!(info) as *mut stat,
// COMMON-REWRITES-NEXT:                 )
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     __pad0: u32,
// REWRITES-X86_64-GNU-NEXT:     __unused: [i64; 3],
// REWRITES-X86_64-GNU-NEXT:         __pad0: 0,
// REWRITES-X86_64-GNU-NEXT:         __unused: [0; 3],
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     __pad: u64,
// REWRITES-AARCH64-GNU-NEXT:     __pad2: i32,
// REWRITES-AARCH64-GNU-NEXT:     __unused: [u32; 2],
// REWRITES-AARCH64-GNU-NEXT:         __pad: 0,
// REWRITES-AARCH64-GNU-NEXT:         __pad2: 0,
// REWRITES-AARCH64-GNU-NEXT:         __unused: [0; 2],
// SLATE-FILECHECK-END rewrites-aarch64-gnu
