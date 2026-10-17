#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat info = {0};
  if (stat("/dev/null", &info) != 0)
    return 1;
  printf("%lld\n", (long long)info.st_mtime);
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn stat(__file: *const i8, __buf: *mut stat) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct stat {
// CHECK-NEXT:     st_dev: u64,
// CHECK-NEXT:     st_ino: u64,
// CHECK-NEXT:     st_nlink: u64,
// CHECK-NEXT:     st_mode: u32,
// CHECK-NEXT:     st_uid: u32,
// CHECK-NEXT:     st_gid: u32,
// CHECK-NEXT:     __pad0: i32,
// CHECK-NEXT:     st_rdev: u64,
// CHECK-NEXT:     st_size: i64,
// CHECK-NEXT:     st_blksize: i64,
// CHECK-NEXT:     st_blocks: i64,
// CHECK-NEXT:     st_atime: i64,
// CHECK-NEXT:     st_atimensec: u64,
// CHECK-NEXT:     st_mtime: i64,
// CHECK-NEXT:     st_mtimensec: u64,
// CHECK-NEXT:     st_ctime: i64,
// CHECK-NEXT:     st_ctimensec: u64,
// CHECK-NEXT:     __glibc_reserved: [i64; 3],
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut info: stat = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: stat = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].st_dev = 0 as u64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe { stat(c"/dev/null".as_ptr(), std::ptr::addr_of_mut!(info)) }) != 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), info.st_mtime) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
