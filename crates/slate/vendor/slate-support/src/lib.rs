#![no_std]
#![allow(non_camel_case_types, non_upper_case_globals)]

pub use libc::*;

#[cfg(target_vendor = "apple")]
pub use mach2 as mach;

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

#[cfg(all(windows, any(target_arch = "x86", target_arch = "x86_64")))]
mod string_intrinsics {
    #[inline]
    unsafe fn movs<T: Copy>(dst: *mut T, src: *const T, n: usize) {
        if (dst as usize).wrapping_sub(src as usize) >= n * size_of::<T>() {
            unsafe { core::ptr::copy(src, dst, n) };
        } else {
            for i in 0..n {
                unsafe { dst.add(i).write(src.add(i).read()) };
            }
        }
    }

    #[inline]
    unsafe fn stos<T: Copy>(dst: *mut T, value: T, n: usize) {
        for i in 0..n {
            unsafe { dst.add(i).write(value) };
        }
    }

    #[inline]
    pub unsafe fn __movsb(dst: *mut u8, src: *const u8, n: usize) {
        unsafe { movs(dst, src, n) }
    }

    #[inline]
    pub unsafe fn __movsw(dst: *mut u16, src: *const u16, n: usize) {
        unsafe { movs(dst, src, n) }
    }

    #[inline]
    pub unsafe fn __movsd(dst: *mut u32, src: *const u32, n: usize) {
        unsafe { movs(dst, src, n) }
    }

    #[cfg(target_arch = "x86_64")]
    #[inline]
    pub unsafe fn __movsq(dst: *mut u64, src: *const u64, n: usize) {
        unsafe { movs(dst, src, n) }
    }

    #[inline]
    pub unsafe fn __stosb(dst: *mut u8, value: u8, n: usize) {
        unsafe { core::ptr::write_bytes(dst, value, n) }
    }

    #[inline]
    pub unsafe fn __stosw(dst: *mut u16, value: u16, n: usize) {
        unsafe { stos(dst, value, n) }
    }

    #[inline]
    pub unsafe fn __stosd(dst: *mut u32, value: u32, n: usize) {
        unsafe { stos(dst, value, n) }
    }

    #[cfg(target_arch = "x86_64")]
    #[inline]
    pub unsafe fn __stosq(dst: *mut u64, value: u64, n: usize) {
        unsafe { stos(dst, value, n) }
    }
}

#[cfg(all(windows, any(target_arch = "x86", target_arch = "x86_64")))]
pub use string_intrinsics::*;

#[cfg(all(windows, any(target_arch = "x86", target_arch = "x86_64")))]
mod machine_intrinsics {
    #[cfg(target_arch = "x86")]
    use core::arch::x86 as arch;
    #[cfg(target_arch = "x86_64")]
    use core::arch::x86_64 as arch;

    #[inline]
    pub unsafe fn __cpuidex(regs: *mut i32, leaf: i32, subleaf: i32) {
        let result = arch::__cpuid_count(leaf as u32, subleaf as u32);
        let values = [result.eax, result.ebx, result.ecx, result.edx];
        for (index, value) in values.into_iter().enumerate() {
            unsafe { regs.add(index).write(value as i32) };
        }
    }

    #[inline]
    pub unsafe fn __cpuid(regs: *mut i32, leaf: i32) {
        unsafe { __cpuidex(regs, leaf, 0) }
    }

    #[inline]
    pub unsafe fn _addcarry_u32(carry: u8, a: u32, b: u32, out: *mut u32) -> u8 {
        let sum = a as u64 + b as u64 + (carry != 0) as u64;
        unsafe { out.write(sum as u32) };
        (sum >> 32) as u8
    }

    #[inline]
    pub unsafe fn _subborrow_u32(borrow: u8, a: u32, b: u32, out: *mut u32) -> u8 {
        let diff = a as i64 - b as i64 - (borrow != 0) as i64;
        unsafe { out.write(diff as u32) };
        (diff < 0) as u8
    }

    #[cfg(target_arch = "x86_64")]
    #[inline]
    pub unsafe fn _addcarry_u64(carry: u8, a: u64, b: u64, out: *mut u64) -> u8 {
        let sum = a as u128 + b as u128 + (carry != 0) as u128;
        unsafe { out.write(sum as u64) };
        (sum >> 64) as u8
    }

    #[cfg(target_arch = "x86_64")]
    #[inline]
    pub unsafe fn _subborrow_u64(borrow: u8, a: u64, b: u64, out: *mut u64) -> u8 {
        let diff = a as i128 - b as i128 - (borrow != 0) as i128;
        unsafe { out.write(diff as u64) };
        (diff < 0) as u8
    }

    #[cfg(target_arch = "x86_64")]
    #[inline]
    pub unsafe fn _umul128(a: u64, b: u64, high: *mut u64) -> u64 {
        let product = a as u128 * b as u128;
        unsafe { high.write((product >> 64) as u64) };
        product as u64
    }
}

#[cfg(all(windows, any(target_arch = "x86", target_arch = "x86_64")))]
pub use machine_intrinsics::*;

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
