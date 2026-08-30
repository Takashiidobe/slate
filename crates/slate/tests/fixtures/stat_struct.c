#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat info   = {0};
  int         result = stat("/dev/null", &info);
  printf("%d %lld\n", result, (long long)info.st_size);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn stat(_0: *const i8, _1: *mut stat) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut info: stat = stat { st_dev: 0, st_ino: 0, st_nlink: 0, st_mode: 0, st_uid: 0, st_gid: 0, __pad0: 0, st_rdev: 0, st_size: 0, st_blksize: 0, st_blocks: 0, st_atim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, st_mtim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, st_ctim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, __unused: [0; 3] };
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     info = stat { st_dev: 0, st_ino: 0, st_nlink: 0, st_mode: 0, st_uid: 0, st_gid: 0, __pad0: 0, st_rdev: 0, st_size: 0, st_blksize: 0, st_blocks: 0, st_atim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, st_mtim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, st_ctim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, __unused: [0; 3] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"/dev/null\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { stat({{_v[0-9]+}} as *const i8, std::ptr::addr_of_mut!(info) as *mut stat) };
// LOWERING-NEXT:     result = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %lld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = info.st_size;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn stat(_0: *const i8, _1: *mut stat) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut info: stat = stat { st_dev: 0, st_ino: 0, st_nlink: 0, st_mode: 0, st_uid: 0, st_gid: 0, __pad0: 0, st_rdev: 0, st_size: 0, st_blksize: 0, st_blocks: 0, st_atim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, st_mtim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, st_ctim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, __unused: [0; 3] };
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: info = stat { st_dev: 0, st_ino: 0, st_nlink: 0, st_mode: 0, st_uid: 0, st_gid: 0, __pad0: 0, st_rdev: 0, st_size: 0, st_blksize: 0, st_blocks: 0, st_atim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, st_mtim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, st_ctim: libc::timespec { tv_sec: 0, tv_nsec: 0 }, __unused: [0; 3] };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"/dev/null\0".as_ptr() as *mut i8;
// REWRITES-NEXT: result = unsafe { stat({{_v[0-9]+}} as *const i8, std::ptr::addr_of_mut!(info) as *mut stat) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %lld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = info.st_size;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, result, {{_v[0-9]+}}) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
