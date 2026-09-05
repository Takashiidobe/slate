#include <dirent.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>

_Static_assert(sizeof(dev_t) == 4, "dev_t");
_Static_assert(sizeof(ino_t) == 8, "ino_t");
_Static_assert(sizeof(mode_t) == 2, "mode_t");
_Static_assert(sizeof(nlink_t) == 2, "nlink_t");
_Static_assert(sizeof(blksize_t) == 4, "blksize_t");
_Static_assert(sizeof(blkcnt_t) == 8, "blkcnt_t");
_Static_assert(sizeof(off_t) == 8, "off_t");

_Static_assert(sizeof(struct stat) == 144, "struct stat");
_Static_assert(offsetof(struct stat, st_dev) == 0, "stat st_dev");
_Static_assert(offsetof(struct stat, st_mode) == 4, "stat st_mode");
_Static_assert(offsetof(struct stat, st_nlink) == 6, "stat st_nlink");
_Static_assert(offsetof(struct stat, st_ino) == 8, "stat st_ino");
_Static_assert(offsetof(struct stat, st_atimespec) == 32, "stat st_atimespec");
_Static_assert(offsetof(struct stat, st_birthtimespec) == 80,
               "stat st_birthtimespec");
_Static_assert(offsetof(struct stat, st_size) == 96, "stat st_size");
_Static_assert(offsetof(struct stat, st_qspare) == 128, "stat st_qspare");

_Static_assert(sizeof(struct dirent) == 1048, "struct dirent");
_Static_assert(offsetof(struct dirent, d_ino) == 0, "dirent d_ino");
_Static_assert(offsetof(struct dirent, d_seekoff) == 8, "dirent d_seekoff");
_Static_assert(offsetof(struct dirent, d_reclen) == 16, "dirent d_reclen");
_Static_assert(offsetof(struct dirent, d_namlen) == 18, "dirent d_namlen");
_Static_assert(offsetof(struct dirent, d_type) == 20, "dirent d_type");
_Static_assert(offsetof(struct dirent, d_name) == 21, "dirent d_name");

_Static_assert(sizeof(struct statvfs) == 64, "struct statvfs");
_Static_assert(offsetof(struct statvfs, f_fsid) == 40, "statvfs f_fsid");

_Static_assert(O_RDONLY == 0x0000, "O_RDONLY");
_Static_assert(O_WRONLY == 0x0001, "O_WRONLY");
_Static_assert(O_NONBLOCK == 0x0004, "O_NONBLOCK");
_Static_assert(O_APPEND == 0x0008, "O_APPEND");
_Static_assert(O_CREAT == 0x0200, "O_CREAT");
_Static_assert(O_TRUNC == 0x0400, "O_TRUNC");
_Static_assert(O_EXCL == 0x0800, "O_EXCL");
_Static_assert(O_CLOEXEC == 0x01000000, "O_CLOEXEC");
_Static_assert(F_DUPFD_CLOEXEC == 67, "F_DUPFD_CLOEXEC");
_Static_assert(AT_FDCWD == -2, "AT_FDCWD");

_Static_assert(__builtin_types_compatible_p(__typeof__(&stat),
                                            int (*)(const char *__restrict,
                                                    struct stat *__restrict)),
               "stat signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&readdir),
                                            struct dirent *(*)(DIR *)),
               "readdir signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&open),
                                            int (*)(const char *, int, ...)),
               "open signature");

int is_directory(mode_t mode) { return S_ISDIR(mode); }

int stat_file(const char *path, struct stat *out) { return stat(path, out); }

struct dirent *next_entry(DIR *dir) { return readdir(dir); }

int open_read_only(const char *path) { return open(path, O_RDONLY); }

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS: #![feature(c_variadic)]
// LOWERING-MACOS-NEXT: #![allow(
// LOWERING-MACOS-NEXT:     dead_code,
// LOWERING-MACOS-NEXT:     unused,
// LOWERING-MACOS-NEXT:     non_camel_case_types,
// LOWERING-MACOS-NEXT:     non_snake_case,
// LOWERING-MACOS-NEXT:     non_upper_case_globals,
// LOWERING-MACOS-NEXT:     arithmetic_overflow,
// LOWERING-MACOS-NEXT:     unconditional_panic,
// LOWERING-MACOS-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-MACOS-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-MACOS-NEXT:     unused_comparisons
// LOWERING-MACOS-NEXT: )]
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[repr(C)]
// LOWERING-MACOS-NEXT: #[derive(Clone, Copy)]
// LOWERING-MACOS-NEXT: struct __dirstream {
// LOWERING-MACOS-NEXT:     __slate_empty: [u8; 0],
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[repr(C)]
// LOWERING-MACOS-NEXT: #[derive(Clone, Copy)]
// LOWERING-MACOS-NEXT: struct dirent {
// LOWERING-MACOS-NEXT:     d_ino: u64,
// LOWERING-MACOS-NEXT:     d_seekoff: u64,
// LOWERING-MACOS-NEXT:     d_reclen: u16,
// LOWERING-MACOS-NEXT:     d_namlen: u16,
// LOWERING-MACOS-NEXT:     d_type: u8,
// LOWERING-MACOS-NEXT:     d_name: [i8; 1024],
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: #[repr(C)]
// LOWERING-MACOS-NEXT: #[derive(Clone, Copy)]
// LOWERING-MACOS-NEXT: struct stat {
// LOWERING-MACOS-NEXT:     st_dev: i32,
// LOWERING-MACOS-NEXT:     st_mode: u16,
// LOWERING-MACOS-NEXT:     st_nlink: u16,
// LOWERING-MACOS-NEXT:     st_ino: u64,
// LOWERING-MACOS-NEXT:     st_uid: u32,
// LOWERING-MACOS-NEXT:     st_gid: u32,
// LOWERING-MACOS-NEXT:     st_rdev: i32,
// LOWERING-MACOS-NEXT:     st_atimespec: libc::timespec,
// LOWERING-MACOS-NEXT:     st_mtimespec: libc::timespec,
// LOWERING-MACOS-NEXT:     st_ctimespec: libc::timespec,
// LOWERING-MACOS-NEXT:     st_birthtimespec: libc::timespec,
// LOWERING-MACOS-NEXT:     st_size: i64,
// LOWERING-MACOS-NEXT:     st_blocks: i64,
// LOWERING-MACOS-NEXT:     st_blksize: i32,
// LOWERING-MACOS-NEXT:     st_flags: u32,
// LOWERING-MACOS-NEXT:     st_gen: u32,
// LOWERING-MACOS-NEXT:     st_lspare: i32,
// LOWERING-MACOS-NEXT:     st_qspare: [i64; 2],
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: unsafe extern "C" {
// LOWERING-MACOS-NEXT:     fn stat(_0: *const core::ffi::c_char, _1: *mut stat) -> i32;
// LOWERING-MACOS-NEXT:     fn readdir(_0: *mut __dirstream) -> *mut dirent;
// LOWERING-MACOS-NEXT:     fn open(_0: *const core::ffi::c_char, _1: i32, ...) -> i32;
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn is_directory({{arg[0-9]+}}: u16) -> i32 {
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} as i32;
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = 61440;
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} & {{__v[0-9]+}};
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = 16384;
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-MACOS-NEXT:     return {{__v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn stat_file({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut stat) -> i32 {
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { stat({{arg[0-9]+}} as *const core::ffi::c_char, {{arg[0-9]+}} as *mut stat) };
// LOWERING-MACOS-NEXT:     return {{__v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn next_entry({{arg[0-9]+}}: *mut __dirstream) -> *mut dirent {
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: *mut dirent = unsafe { readdir({{arg[0-9]+}} as *mut __dirstream) };
// LOWERING-MACOS-NEXT:     return {{__v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn open_read_only({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { open({{arg[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) };
// LOWERING-MACOS-NEXT:     return {{__v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// SLATE-FILECHECK-END lowering-macos

// SLATE-FILECHECK-BEGIN rewrites-macos
// REWRITES-MACOS: #![feature(c_variadic)]
// REWRITES-MACOS-NEXT: #![allow(
// REWRITES-MACOS-NEXT:     dead_code,
// REWRITES-MACOS-NEXT:     unused,
// REWRITES-MACOS-NEXT:     non_camel_case_types,
// REWRITES-MACOS-NEXT:     non_snake_case,
// REWRITES-MACOS-NEXT:     non_upper_case_globals,
// REWRITES-MACOS-NEXT:     arithmetic_overflow,
// REWRITES-MACOS-NEXT:     unconditional_panic,
// REWRITES-MACOS-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-MACOS-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-MACOS-NEXT:     unused_comparisons
// REWRITES-MACOS-NEXT: )]
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: #[repr(C)]
// REWRITES-MACOS-NEXT: #[derive(Clone, Copy)]
// REWRITES-MACOS-NEXT: struct __dirstream {
// REWRITES-MACOS-NEXT:     __slate_empty: [u8; 0],
// REWRITES-MACOS-NEXT: }
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: #[repr(C)]
// REWRITES-MACOS-NEXT: #[derive(Clone, Copy)]
// REWRITES-MACOS-NEXT: struct dirent {
// REWRITES-MACOS-NEXT:     d_ino: u64,
// REWRITES-MACOS-NEXT:     d_seekoff: u64,
// REWRITES-MACOS-NEXT:     d_reclen: u16,
// REWRITES-MACOS-NEXT:     d_namlen: u16,
// REWRITES-MACOS-NEXT:     d_type: u8,
// REWRITES-MACOS-NEXT:     d_name: [i8; 1024],
// REWRITES-MACOS-NEXT: }
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: #[repr(C)]
// REWRITES-MACOS-NEXT: #[derive(Clone, Copy)]
// REWRITES-MACOS-NEXT: struct stat {
// REWRITES-MACOS-NEXT:     st_dev: i32,
// REWRITES-MACOS-NEXT:     st_mode: u16,
// REWRITES-MACOS-NEXT:     st_nlink: u16,
// REWRITES-MACOS-NEXT:     st_ino: u64,
// REWRITES-MACOS-NEXT:     st_uid: u32,
// REWRITES-MACOS-NEXT:     st_gid: u32,
// REWRITES-MACOS-NEXT:     st_rdev: i32,
// REWRITES-MACOS-NEXT:     st_atimespec: libc::timespec,
// REWRITES-MACOS-NEXT:     st_mtimespec: libc::timespec,
// REWRITES-MACOS-NEXT:     st_ctimespec: libc::timespec,
// REWRITES-MACOS-NEXT:     st_birthtimespec: libc::timespec,
// REWRITES-MACOS-NEXT:     st_size: i64,
// REWRITES-MACOS-NEXT:     st_blocks: i64,
// REWRITES-MACOS-NEXT:     st_blksize: i32,
// REWRITES-MACOS-NEXT:     st_flags: u32,
// REWRITES-MACOS-NEXT:     st_gen: u32,
// REWRITES-MACOS-NEXT:     st_lspare: i32,
// REWRITES-MACOS-NEXT:     st_qspare: [i64; 2],
// REWRITES-MACOS-NEXT: }
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: unsafe extern "C" {
// REWRITES-MACOS-NEXT:     fn stat(_0: *const core::ffi::c_char, _1: *mut stat) -> i32;
// REWRITES-MACOS-NEXT:     fn readdir(_0: *mut __dirstream) -> *mut dirent;
// REWRITES-MACOS-NEXT:     fn open(_0: *const core::ffi::c_char, _1: i32, ...) -> i32;
// REWRITES-MACOS-NEXT: }
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: fn is_directory({{arg[0-9]+}}: u16) -> i32 {
// REWRITES-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-MACOS-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-MACOS-NEXT:     if ({{arg[0-9]+}} as i32) & 61440 == 16384 {
// REWRITES-MACOS-NEXT:         {{__v[0-9]+}}
// REWRITES-MACOS-NEXT:     } else {
// REWRITES-MACOS-NEXT:         {{__v[0-9]+}}
// REWRITES-MACOS-NEXT:     }
// REWRITES-MACOS-NEXT: }
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: fn stat_file({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut stat) -> i32 {
// REWRITES-MACOS-NEXT:     unsafe { stat({{arg[0-9]+}} as *const core::ffi::c_char, {{arg[0-9]+}} as *mut stat) }
// REWRITES-MACOS-NEXT: }
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: fn next_entry({{arg[0-9]+}}: *mut __dirstream) -> *mut dirent {
// REWRITES-MACOS-NEXT:     unsafe { readdir({{arg[0-9]+}} as *mut __dirstream) }
// REWRITES-MACOS-NEXT: }
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: fn open_read_only({{arg[0-9]+}}: *mut i8) -> i32 {
// REWRITES-MACOS-NEXT:     unsafe { open({{arg[0-9]+}} as *const core::ffi::c_char, 0 as i32) }
// REWRITES-MACOS-NEXT: }
// SLATE-FILECHECK-END rewrites-macos
