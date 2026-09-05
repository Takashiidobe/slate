#include <dirent.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <utime.h>

#if defined(__SLATE_ARCH_X86_64)
_Static_assert(sizeof(struct stat) == 144, "stat size");
_Static_assert(offsetof(struct stat, st_nlink) == 16, "st_nlink offset");
_Static_assert(sizeof(((struct stat *)0)->st_nlink) == 8, "st_nlink width");
_Static_assert(offsetof(struct stat, st_mode) == 24, "st_mode offset");
_Static_assert(offsetof(struct stat, st_blksize) == 56, "st_blksize offset");
_Static_assert(sizeof(((struct stat *)0)->st_blksize) == 8, "st_blksize width");
_Static_assert(offsetof(struct stat, st_blocks) == 64, "st_blocks offset");
_Static_assert(sizeof(struct stat64) == 144, "stat64 size");
#else
_Static_assert(sizeof(struct stat) == 128, "stat size");
_Static_assert(offsetof(struct stat, st_mode) == 16, "st_mode offset");
_Static_assert(offsetof(struct stat, st_nlink) == 20, "st_nlink offset");
_Static_assert(sizeof(((struct stat *)0)->st_nlink) == 4, "st_nlink width");
_Static_assert(offsetof(struct stat, st_blksize) == 56, "st_blksize offset");
_Static_assert(sizeof(((struct stat *)0)->st_blksize) == 4, "st_blksize width");
_Static_assert(offsetof(struct stat, st_blocks) == 64, "st_blocks offset");
_Static_assert(sizeof(((struct stat *)0)->st_blocks) == 8, "st_blocks width");
_Static_assert(sizeof(struct stat64) == 128, "stat64 size");
#endif

_Static_assert(sizeof(struct dirent) == 280, "dirent size");
_Static_assert(sizeof(struct dirent64) == 280, "dirent64 size");
_Static_assert(sizeof(struct statvfs) == 112, "statvfs size");
_Static_assert(sizeof(blksize_t) == 8, "public blksize_t width");
_Static_assert(sizeof(blkcnt_t) == 8, "public blkcnt_t width");
_Static_assert((blksize_t)-1 > 0, "public blksize_t is unsigned");
_Static_assert(AT_FDCWD == -100, "AT_FDCWD");
_Static_assert(O_CREAT == 0100, "O_CREAT");
#if defined(__SLATE_ARCH_X86_64)
_Static_assert(O_DIRECTORY == 0200000, "O_DIRECTORY");
#else
_Static_assert(O_DIRECTORY == 040000, "O_DIRECTORY");
#endif
_Static_assert(F_DUPFD_CLOEXEC == 1030, "F_DUPFD_CLOEXEC");

int main(void) {
  struct stat    s;
  struct dirent  d;
  struct statvfs v;
  struct utimbuf u;
  (void)s;
  (void)d;
  (void)v;
  (void)u;
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
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
// LOWERING-NEXT: struct dirent {
// LOWERING-NEXT:     d_ino: u64,
// LOWERING-NEXT:     d_off: i64,
// LOWERING-NEXT:     d_reclen: u16,
// LOWERING-NEXT:     d_type: u8,
// LOWERING-BIONIC-AARCH64-NEXT:     d_name: [u8; 256],
// LOWERING-BIONIC-X86_64-NEXT:     d_name: [i8; 256],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct stat {
// LOWERING-NEXT:     st_dev: u64,
// LOWERING-NEXT:     st_ino: u64,
// LOWERING-BIONIC-X86_64-NEXT:     st_nlink: u64,
// LOWERING-NEXT:     st_mode: u32,
// LOWERING-BIONIC-AARCH64-NEXT:     st_nlink: u32,
// LOWERING-NEXT:     st_uid: u32,
// LOWERING-NEXT:     st_gid: u32,
// LOWERING-BIONIC-X86_64-NEXT:     __pad0: u32,
// LOWERING-NEXT:     st_rdev: u64,
// LOWERING-BIONIC-AARCH64-NEXT:     __pad1: u64,
// LOWERING-NEXT:     st_size: i64,
// LOWERING-BIONIC-AARCH64-NEXT:     st_blksize: i32,
// LOWERING-BIONIC-AARCH64-NEXT:     __pad2: i32,
// LOWERING-BIONIC-X86_64-NEXT:     st_blksize: i64,
// LOWERING-NEXT:     st_blocks: i64,
// LOWERING-NEXT:     st_atim: libc::timespec,
// LOWERING-NEXT:     st_mtim: libc::timespec,
// LOWERING-NEXT:     st_ctim: libc::timespec,
// LOWERING-BIONIC-AARCH64-NEXT:     __unused4: u32,
// LOWERING-BIONIC-AARCH64-NEXT:     __unused5: u32,
// LOWERING-BIONIC-X86_64-NEXT:     __pad3: [i64; 3],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct statvfs {
// LOWERING-NEXT:     f_bsize: u64,
// LOWERING-NEXT:     f_frsize: u64,
// LOWERING-NEXT:     f_blocks: u64,
// LOWERING-NEXT:     f_bfree: u64,
// LOWERING-NEXT:     f_bavail: u64,
// LOWERING-NEXT:     f_files: u64,
// LOWERING-NEXT:     f_ffree: u64,
// LOWERING-NEXT:     f_favail: u64,
// LOWERING-NEXT:     f_fsid: u64,
// LOWERING-NEXT:     f_flag: u64,
// LOWERING-NEXT:     f_namemax: u64,
// LOWERING-NEXT:     __f_reserved: [u32; 6],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct utimbuf {
// LOWERING-NEXT:     actime: i64,
// LOWERING-NEXT:     modtime: i64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut s: stat = stat {
// LOWERING-NEXT:         st_dev: 0,
// LOWERING-NEXT:         st_ino: 0,
// LOWERING-BIONIC-X86_64-NEXT:         st_nlink: 0,
// LOWERING-NEXT:         st_mode: 0,
// LOWERING-BIONIC-AARCH64-NEXT:         st_nlink: 0,
// LOWERING-NEXT:         st_uid: 0,
// LOWERING-NEXT:         st_gid: 0,
// LOWERING-BIONIC-X86_64-NEXT:         __pad0: 0,
// LOWERING-NEXT:         st_rdev: 0,
// LOWERING-BIONIC-AARCH64-NEXT:         __pad1: 0,
// LOWERING-NEXT:         st_size: 0,
// LOWERING-NEXT:         st_blksize: 0,
// LOWERING-BIONIC-AARCH64-NEXT:         __pad2: 0,
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
// LOWERING-BIONIC-AARCH64-NEXT:         __unused4: 0,
// LOWERING-BIONIC-AARCH64-NEXT:         __unused5: 0,
// LOWERING-BIONIC-X86_64-NEXT:         __pad3: [0; 3],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut d: dirent = dirent {
// LOWERING-NEXT:         d_ino: 0,
// LOWERING-NEXT:         d_off: 0,
// LOWERING-NEXT:         d_reclen: 0,
// LOWERING-NEXT:         d_type: 0,
// LOWERING-NEXT:         d_name: [0; 256],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut v: statvfs = statvfs {
// LOWERING-NEXT:         f_bsize: 0,
// LOWERING-NEXT:         f_frsize: 0,
// LOWERING-NEXT:         f_blocks: 0,
// LOWERING-NEXT:         f_bfree: 0,
// LOWERING-NEXT:         f_bavail: 0,
// LOWERING-NEXT:         f_files: 0,
// LOWERING-NEXT:         f_ffree: 0,
// LOWERING-NEXT:         f_favail: 0,
// LOWERING-NEXT:         f_fsid: 0,
// LOWERING-NEXT:         f_flag: 0,
// LOWERING-NEXT:         f_namemax: 0,
// LOWERING-NEXT:         __f_reserved: [0; 6],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut u: utimbuf = utimbuf {
// LOWERING-NEXT:         actime: 0,
// LOWERING-NEXT:         modtime: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
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
// REWRITES-NEXT: struct dirent {
// REWRITES-NEXT:     d_ino: u64,
// REWRITES-NEXT:     d_off: i64,
// REWRITES-NEXT:     d_reclen: u16,
// REWRITES-NEXT:     d_type: u8,
// REWRITES-BIONIC-AARCH64-NEXT:     d_name: [u8; 256],
// REWRITES-BIONIC-X86_64-NEXT:     d_name: [i8; 256],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct stat {
// REWRITES-NEXT:     st_dev: u64,
// REWRITES-NEXT:     st_ino: u64,
// REWRITES-BIONIC-X86_64-NEXT:     st_nlink: u64,
// REWRITES-NEXT:     st_mode: u32,
// REWRITES-BIONIC-AARCH64-NEXT:     st_nlink: u32,
// REWRITES-NEXT:     st_uid: u32,
// REWRITES-NEXT:     st_gid: u32,
// REWRITES-BIONIC-X86_64-NEXT:     __pad0: u32,
// REWRITES-NEXT:     st_rdev: u64,
// REWRITES-BIONIC-AARCH64-NEXT:     __pad1: u64,
// REWRITES-NEXT:     st_size: i64,
// REWRITES-BIONIC-AARCH64-NEXT:     st_blksize: i32,
// REWRITES-BIONIC-AARCH64-NEXT:     __pad2: i32,
// REWRITES-BIONIC-X86_64-NEXT:     st_blksize: i64,
// REWRITES-NEXT:     st_blocks: i64,
// REWRITES-NEXT:     st_atim: libc::timespec,
// REWRITES-NEXT:     st_mtim: libc::timespec,
// REWRITES-NEXT:     st_ctim: libc::timespec,
// REWRITES-BIONIC-AARCH64-NEXT:     __unused4: u32,
// REWRITES-BIONIC-AARCH64-NEXT:     __unused5: u32,
// REWRITES-BIONIC-X86_64-NEXT:     __pad3: [i64; 3],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct statvfs {
// REWRITES-NEXT:     f_bsize: u64,
// REWRITES-NEXT:     f_frsize: u64,
// REWRITES-NEXT:     f_blocks: u64,
// REWRITES-NEXT:     f_bfree: u64,
// REWRITES-NEXT:     f_bavail: u64,
// REWRITES-NEXT:     f_files: u64,
// REWRITES-NEXT:     f_ffree: u64,
// REWRITES-NEXT:     f_favail: u64,
// REWRITES-NEXT:     f_fsid: u64,
// REWRITES-NEXT:     f_flag: u64,
// REWRITES-NEXT:     f_namemax: u64,
// REWRITES-NEXT:     __f_reserved: [u32; 6],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct utimbuf {
// REWRITES-NEXT:     actime: i64,
// REWRITES-NEXT:     modtime: i64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
