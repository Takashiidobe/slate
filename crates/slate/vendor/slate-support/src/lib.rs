#![no_std]
#![allow(non_camel_case_types, non_upper_case_globals)]

#[cfg(feature = "hosted")]
mod hosted;
#[cfg(feature = "hosted")]
pub use hosted::*;

#[cfg(target_vendor = "apple")]
pub use mach2 as mach;

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

pub struct Guard<'a>(pub &'a mut dyn FnMut());

impl Drop for Guard<'_> {
    fn drop(&mut self) {
        (self.0)()
    }
}
