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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![allow(
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
// COMMON-LOWERING-NEXT: struct dirent {
// COMMON-LOWERING-NEXT:     d_ino: u64,
// COMMON-LOWERING-NEXT:     d_off: i64,
// COMMON-LOWERING-NEXT:     d_reclen: u16,
// COMMON-LOWERING-NEXT:     d_type: u8,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct stat {
// COMMON-LOWERING-NEXT:     st_dev: u64,
// COMMON-LOWERING-NEXT:     st_ino: u64,
// COMMON-LOWERING-NEXT:     st_mode: u32,
// COMMON-LOWERING-NEXT:     st_uid: u32,
// COMMON-LOWERING-NEXT:     st_gid: u32,
// COMMON-LOWERING-NEXT:     st_rdev: u64,
// COMMON-LOWERING-NEXT:     st_size: i64,
// COMMON-LOWERING-NEXT:     st_blocks: i64,
// COMMON-LOWERING-NEXT:     st_atim: libc::timespec,
// COMMON-LOWERING-NEXT:     st_mtim: libc::timespec,
// COMMON-LOWERING-NEXT:     st_ctim: libc::timespec,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct statvfs {
// COMMON-LOWERING-NEXT:     f_bsize: u64,
// COMMON-LOWERING-NEXT:     f_frsize: u64,
// COMMON-LOWERING-NEXT:     f_blocks: u64,
// COMMON-LOWERING-NEXT:     f_bfree: u64,
// COMMON-LOWERING-NEXT:     f_bavail: u64,
// COMMON-LOWERING-NEXT:     f_files: u64,
// COMMON-LOWERING-NEXT:     f_ffree: u64,
// COMMON-LOWERING-NEXT:     f_favail: u64,
// COMMON-LOWERING-NEXT:     f_fsid: u64,
// COMMON-LOWERING-NEXT:     f_flag: u64,
// COMMON-LOWERING-NEXT:     f_namemax: u64,
// COMMON-LOWERING-NEXT:     __f_reserved: [u32; 6],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct utimbuf {
// COMMON-LOWERING-NEXT:     actime: i64,
// COMMON-LOWERING-NEXT:     modtime: i64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut s: stat = stat {
// COMMON-LOWERING-NEXT:         st_dev: 0,
// COMMON-LOWERING-NEXT:         st_ino: 0,
// COMMON-LOWERING-NEXT:         st_mode: 0,
// COMMON-LOWERING-NEXT:         st_nlink: 0,
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
// COMMON-LOWERING-NEXT:     let mut d: dirent = dirent {
// COMMON-LOWERING-NEXT:         d_ino: 0,
// COMMON-LOWERING-NEXT:         d_off: 0,
// COMMON-LOWERING-NEXT:         d_reclen: 0,
// COMMON-LOWERING-NEXT:         d_type: 0,
// COMMON-LOWERING-NEXT:         d_name: [0; 256],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut v: statvfs = statvfs {
// COMMON-LOWERING-NEXT:         f_bsize: 0,
// COMMON-LOWERING-NEXT:         f_frsize: 0,
// COMMON-LOWERING-NEXT:         f_blocks: 0,
// COMMON-LOWERING-NEXT:         f_bfree: 0,
// COMMON-LOWERING-NEXT:         f_bavail: 0,
// COMMON-LOWERING-NEXT:         f_files: 0,
// COMMON-LOWERING-NEXT:         f_ffree: 0,
// COMMON-LOWERING-NEXT:         f_favail: 0,
// COMMON-LOWERING-NEXT:         f_fsid: 0,
// COMMON-LOWERING-NEXT:         f_flag: 0,
// COMMON-LOWERING-NEXT:         f_namemax: 0,
// COMMON-LOWERING-NEXT:         __f_reserved: [0; 6],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut u: utimbuf = utimbuf {
// COMMON-LOWERING-NEXT:         actime: 0,
// COMMON-LOWERING-NEXT:         modtime: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-bionic-aarch64
// LOWERING-BIONIC-AARCH64-NEXT:     d_name: [u8; 256],
// LOWERING-BIONIC-AARCH64-NEXT:     st_nlink: u32,
// LOWERING-BIONIC-AARCH64-NEXT:     __pad1: u64,
// LOWERING-BIONIC-AARCH64-NEXT:     st_blksize: i32,
// LOWERING-BIONIC-AARCH64-NEXT:     __pad2: i32,
// LOWERING-BIONIC-AARCH64-NEXT:     __unused4: u32,
// LOWERING-BIONIC-AARCH64-NEXT:     __unused5: u32,
// LOWERING-BIONIC-AARCH64-NEXT:         __pad1: 0,
// LOWERING-BIONIC-AARCH64-NEXT:         __pad2: 0,
// LOWERING-BIONIC-AARCH64-NEXT:         __unused4: 0,
// LOWERING-BIONIC-AARCH64-NEXT:         __unused5: 0,
// SLATE-FILECHECK-END lowering-bionic-aarch64

// SLATE-FILECHECK-BEGIN lowering-bionic-x86_64
// LOWERING-BIONIC-X86_64-NEXT:     d_name: [i8; 256],
// LOWERING-BIONIC-X86_64-NEXT:     st_nlink: u64,
// LOWERING-BIONIC-X86_64-NEXT:     __pad0: u32,
// LOWERING-BIONIC-X86_64-NEXT:     st_blksize: i64,
// LOWERING-BIONIC-X86_64-NEXT:     __pad3: [i64; 3],
// LOWERING-BIONIC-X86_64-NEXT:         __pad0: 0,
// LOWERING-BIONIC-X86_64-NEXT:         __pad3: [0; 3],
// SLATE-FILECHECK-END lowering-bionic-x86_64

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![allow(
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
// COMMON-REWRITES-NEXT: struct dirent {
// COMMON-REWRITES-NEXT:     d_ino: u64,
// COMMON-REWRITES-NEXT:     d_off: i64,
// COMMON-REWRITES-NEXT:     d_reclen: u16,
// COMMON-REWRITES-NEXT:     d_type: u8,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct stat {
// COMMON-REWRITES-NEXT:     st_dev: u64,
// COMMON-REWRITES-NEXT:     st_ino: u64,
// COMMON-REWRITES-NEXT:     st_mode: u32,
// COMMON-REWRITES-NEXT:     st_uid: u32,
// COMMON-REWRITES-NEXT:     st_gid: u32,
// COMMON-REWRITES-NEXT:     st_rdev: u64,
// COMMON-REWRITES-NEXT:     st_size: i64,
// COMMON-REWRITES-NEXT:     st_blocks: i64,
// COMMON-REWRITES-NEXT:     st_atim: libc::timespec,
// COMMON-REWRITES-NEXT:     st_mtim: libc::timespec,
// COMMON-REWRITES-NEXT:     st_ctim: libc::timespec,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct statvfs {
// COMMON-REWRITES-NEXT:     f_bsize: u64,
// COMMON-REWRITES-NEXT:     f_frsize: u64,
// COMMON-REWRITES-NEXT:     f_blocks: u64,
// COMMON-REWRITES-NEXT:     f_bfree: u64,
// COMMON-REWRITES-NEXT:     f_bavail: u64,
// COMMON-REWRITES-NEXT:     f_files: u64,
// COMMON-REWRITES-NEXT:     f_ffree: u64,
// COMMON-REWRITES-NEXT:     f_favail: u64,
// COMMON-REWRITES-NEXT:     f_fsid: u64,
// COMMON-REWRITES-NEXT:     f_flag: u64,
// COMMON-REWRITES-NEXT:     f_namemax: u64,
// COMMON-REWRITES-NEXT:     __f_reserved: [u32; 6],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct utimbuf {
// COMMON-REWRITES-NEXT:     actime: i64,
// COMMON-REWRITES-NEXT:     modtime: i64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-bionic-aarch64
// REWRITES-BIONIC-AARCH64-NEXT:     d_name: [u8; 256],
// REWRITES-BIONIC-AARCH64-NEXT:     st_nlink: u32,
// REWRITES-BIONIC-AARCH64-NEXT:     __pad1: u64,
// REWRITES-BIONIC-AARCH64-NEXT:     st_blksize: i32,
// REWRITES-BIONIC-AARCH64-NEXT:     __pad2: i32,
// REWRITES-BIONIC-AARCH64-NEXT:     __unused4: u32,
// REWRITES-BIONIC-AARCH64-NEXT:     __unused5: u32,
// SLATE-FILECHECK-END rewrites-bionic-aarch64

// SLATE-FILECHECK-BEGIN rewrites-bionic-x86_64
// REWRITES-BIONIC-X86_64-NEXT:     d_name: [i8; 256],
// REWRITES-BIONIC-X86_64-NEXT:     st_nlink: u64,
// REWRITES-BIONIC-X86_64-NEXT:     __pad0: u32,
// REWRITES-BIONIC-X86_64-NEXT:     st_blksize: i64,
// REWRITES-BIONIC-X86_64-NEXT:     __pad3: [i64; 3],
// SLATE-FILECHECK-END rewrites-bionic-x86_64
