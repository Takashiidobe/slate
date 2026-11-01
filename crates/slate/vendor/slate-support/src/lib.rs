#![no_std]
#![allow(non_camel_case_types, non_upper_case_globals)]

pub use libc::*;

unsafe extern "C" {
    pub fn putc(c: c_int, stream: *mut FILE) -> c_int;
    pub fn getc(stream: *mut FILE) -> c_int;
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

#[cfg(any(
    windows,
    all(
        target_pointer_width = "32",
        any(target_env = "gnu", target_os = "android")
    )
))]
pub type off_t = i32;
#[cfg(not(any(
    windows,
    all(
        target_pointer_width = "32",
        any(target_env = "gnu", target_os = "android")
    )
)))]
pub type off_t = i64;

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
