#![no_std]
#![allow(non_camel_case_types, non_upper_case_globals)]

pub use libc::*;

#[cfg(target_vendor = "apple")]
pub use mach2 as mach;

#[cfg(windows)]
pub use windows_sys as windows;

unsafe extern "C" {
    pub fn putc(c: c_int, stream: *mut FILE) -> c_int;
    pub fn getc(stream: *mut FILE) -> c_int;
    #[cfg_attr(target_os = "linux", link_name = "__errno_location")]
    #[cfg_attr(any(target_os = "macos", target_os = "freebsd"), link_name = "__error")]
    #[cfg_attr(target_os = "android", link_name = "__errno")]
    #[cfg_attr(windows, link_name = "_errno")]
    pub fn errno_location() -> *mut c_int;
}

#[inline]
pub unsafe fn __sputc(c: c_int, stream: *mut FILE) -> c_int {
    unsafe { putc(c, stream) }
}

#[cfg(target_os = "linux")]
mod streams {
    use super::FILE;

    mod symbols {
        use super::FILE;

        unsafe extern "C" {
            pub static mut stdin: *mut FILE;
            pub static mut stdout: *mut FILE;
            pub static mut stderr: *mut FILE;
        }
    }

    #[inline]
    pub fn stdin() -> *mut FILE {
        unsafe { symbols::stdin }
    }

    #[inline]
    pub fn stdout() -> *mut FILE {
        unsafe { symbols::stdout }
    }

    #[inline]
    pub fn stderr() -> *mut FILE {
        unsafe { symbols::stderr }
    }
}

#[cfg(any(target_os = "macos", target_os = "freebsd"))]
mod streams {
    use super::FILE;

    unsafe extern "C" {
        static mut __stdinp: *mut FILE;
        static mut __stdoutp: *mut FILE;
        static mut __stderrp: *mut FILE;
    }

    #[inline]
    pub fn stdin() -> *mut FILE {
        unsafe { __stdinp }
    }

    #[inline]
    pub fn stdout() -> *mut FILE {
        unsafe { __stdoutp }
    }

    #[inline]
    pub fn stderr() -> *mut FILE {
        unsafe { __stderrp }
    }
}

#[cfg(target_os = "android")]
mod streams {
    use super::FILE;

    #[cfg(target_pointer_width = "64")]
    const FILE_SIZE: usize = 152;
    #[cfg(target_pointer_width = "32")]
    const FILE_SIZE: usize = 84;

    unsafe extern "C" {
        static mut __sF: [u8; 0];
    }

    #[inline]
    fn stream(index: usize) -> *mut FILE {
        unsafe { (&raw mut __sF).cast::<u8>().add(index * FILE_SIZE).cast() }
    }

    #[inline]
    pub fn stdin() -> *mut FILE {
        stream(0)
    }

    #[inline]
    pub fn stdout() -> *mut FILE {
        stream(1)
    }

    #[inline]
    pub fn stderr() -> *mut FILE {
        stream(2)
    }
}

#[cfg(windows)]
mod streams {
    use super::FILE;

    unsafe extern "C" {
        fn __acrt_iob_func(index: u32) -> *mut FILE;
    }

    #[inline]
    pub fn stdin() -> *mut FILE {
        unsafe { __acrt_iob_func(0) }
    }

    #[inline]
    pub fn stdout() -> *mut FILE {
        unsafe { __acrt_iob_func(1) }
    }

    #[inline]
    pub fn stderr() -> *mut FILE {
        unsafe { __acrt_iob_func(2) }
    }
}

pub use streams::{stderr, stdin, stdout};

#[cfg(target_pointer_width = "64")]
pub type size_t = u64;
#[cfg(target_pointer_width = "32")]
pub type size_t = u32;

#[cfg(target_pointer_width = "64")]
pub type uintptr_t = u64;
#[cfg(target_pointer_width = "32")]
pub type uintptr_t = u32;

#[cfg(target_pointer_width = "64")]
pub type ssize_t = i64;
#[cfg(target_pointer_width = "32")]
pub type ssize_t = i32;

#[cfg(target_pointer_width = "64")]
pub type ptrdiff_t = i64;
#[cfg(target_pointer_width = "32")]
pub type ptrdiff_t = i32;

#[cfg(target_pointer_width = "64")]
pub type intptr_t = i64;
#[cfg(target_pointer_width = "32")]
pub type intptr_t = i32;

#[cfg(not(all(target_os = "linux", target_env = "gnu", target_pointer_width = "32")))]
pub use libc::off_t;
#[cfg(all(target_os = "linux", target_env = "gnu", target_pointer_width = "32"))]
pub type off_t = c_long;

#[cfg(all(target_os = "linux", target_env = "gnu", target_pointer_width = "32"))]
unsafe extern "C" {
    pub fn lseek(fd: c_int, offset: off_t, whence: c_int) -> off_t;
    pub fn pread(fd: c_int, buf: *mut c_void, count: usize, offset: off_t) -> isize;
    pub fn pwrite(fd: c_int, buf: *const c_void, count: usize, offset: off_t) -> isize;
    pub fn mmap(
        addr: *mut c_void,
        len: usize,
        prot: c_int,
        flags: c_int,
        fd: c_int,
        offset: off_t,
    ) -> *mut c_void;
    pub fn truncate(path: *const c_char, length: off_t) -> c_int;
    pub fn ftruncate(fd: c_int, length: off_t) -> c_int;
    pub fn fseeko(stream: *mut FILE, offset: off_t, whence: c_int) -> c_int;
    pub fn ftello(stream: *mut FILE) -> off_t;
    pub fn lockf(fd: c_int, cmd: c_int, len: off_t) -> c_int;
    pub fn posix_fadvise(fd: c_int, offset: off_t, len: off_t, advise: c_int) -> c_int;
    pub fn posix_fallocate(fd: c_int, offset: off_t, len: off_t) -> c_int;
    pub fn fallocate(fd: c_int, mode: c_int, offset: off_t, len: off_t) -> c_int;
    pub fn readahead(fd: c_int, offset: off_t, count: usize) -> isize;
    pub fn preadv(fd: c_int, iov: *const iovec, iovcnt: c_int, offset: off_t) -> isize;
    pub fn pwritev(fd: c_int, iov: *const iovec, iovcnt: c_int, offset: off_t) -> isize;
    pub fn sync_file_range(fd: c_int, offset: off_t, nbytes: off_t, flags: c_uint) -> c_int;
    pub fn sendfile(out_fd: c_int, in_fd: c_int, offset: *mut off_t, count: usize) -> isize;
}

#[cfg(windows)]
pub type wchar_t = u16;
#[cfg(all(
    not(windows),
    any(target_arch = "x86_64", target_arch = "x86", target_os = "macos")
))]
pub type wchar_t = i32;
#[cfg(all(
    not(windows),
    not(any(target_arch = "x86_64", target_arch = "x86", target_os = "macos"))
))]
pub type wchar_t = u32;

pub type intmax_t = i64;
pub type uintmax_t = u64;
pub type off64_t = i64;
pub type pid_t = i32;
pub type uid_t = u32;
pub type gid_t = u32;

#[cfg(windows)]
pub type socklen_t = i32;
#[cfg(not(windows))]
pub type socklen_t = u32;

#[cfg(windows)]
pub type wint_t = u16;
#[cfg(any(target_os = "macos", target_os = "freebsd"))]
pub type wint_t = i32;
#[cfg(not(any(windows, target_os = "macos", target_os = "freebsd")))]
pub type wint_t = u32;

pub use libc::time_t;

#[cfg(target_os = "macos")]
pub type clock_t = u64;
#[cfg(all(
    not(target_os = "macos"),
    any(windows, target_os = "freebsd", target_pointer_width = "32")
))]
pub type clock_t = i32;
#[cfg(not(any(
    target_os = "macos",
    windows,
    target_os = "freebsd",
    target_pointer_width = "32"
)))]
pub type clock_t = i64;

#[cfg(any(target_os = "macos", target_os = "freebsd"))]
pub type mode_t = u16;
#[cfg(not(any(target_os = "macos", target_os = "freebsd")))]
pub type mode_t = u32;

#[cfg(target_os = "macos")]
pub type dev_t = i32;
#[cfg(any(windows, all(target_os = "android", target_pointer_width = "32")))]
pub type dev_t = u32;
#[cfg(not(any(
    target_os = "macos",
    windows,
    all(target_os = "android", target_pointer_width = "32")
)))]
pub type dev_t = u64;

#[cfg(windows)]
pub type ino_t = u16;
#[cfg(all(
    not(windows),
    any(
        all(target_os = "macos", target_arch = "x86_64"),
        all(
            target_pointer_width = "32",
            any(target_env = "gnu", target_os = "android")
        )
    )
))]
pub type ino_t = u32;
#[cfg(not(any(
    windows,
    all(target_os = "macos", target_arch = "x86_64"),
    all(
        target_pointer_width = "32",
        any(target_env = "gnu", target_os = "android")
    )
)))]
pub type ino_t = u64;

#[cfg(target_os = "macos")]
pub type nlink_t = u16;
#[cfg(any(
    target_os = "freebsd",
    all(target_os = "linux", target_arch = "x86_64")
))]
pub type nlink_t = u64;
#[cfg(not(any(
    target_os = "macos",
    target_os = "freebsd",
    all(target_os = "linux", target_arch = "x86_64")
)))]
pub type nlink_t = u32;
