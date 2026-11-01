#![no_std]
#![allow(non_camel_case_types)]

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
