#include <stdio.h>

#ifdef ALPHA
static int only_alpha(void) { return 7; }
#endif

#ifdef BETA
int table[2] = {1, 2};
#else
int table[3] = {1, 2, 3};
#endif

int main(void) {
#ifdef ALPHA
    printf("%d\n", only_alpha());
#endif
    printf("%d\n", (int)(sizeof(table) / sizeof(table[0])));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 4 compiles
// CHECK-NEXT:   presence.c: [alpha,beta] 4 compiled
// CHECK-NEXT: target windows-x86_64: 4 compiles
// CHECK-NEXT:   presence.c: [alpha,beta] 4 compiled
// CHECK-NEXT: total: 8 compiles across 2 targets
// CHECK-NEXT: unit presence: 17 items, 15 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 8 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __va_start([[ARG_0:arg[0-9]+]]: *mut *mut i8, ...);
// CHECK-NEXT:     fn __acrt_iob_func([[ARG_0]]: u32) -> *mut _iobuf;
// CHECK-NEXT:     fn __stdio_common_vfprintf(
// CHECK-NEXT:         [[ARG_0]]: u64,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *mut _iobuf,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: *const i8,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *mut __crt_locale_pointers,
// CHECK-NEXT:         [[ARG_4:arg[0-9]+]]: *mut i8,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_locale_pointers {
// CHECK-NEXT:     locinfo: *mut __crt_locale_data,
// CHECK-NEXT:     mbcinfo: *mut __crt_multibyte_data,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_locale_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_multibyte_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: fn __local_stdio_printf_options() -> *mut u64 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(_OptionsStorage) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: static mut _OptionsStorage: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _iobuf {
// CHECK-NEXT:     _Placeholder: *mut (),
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: fn _vfprintf_l(
// CHECK-NEXT:     _Stream: *mut _iobuf,
// CHECK-NEXT:     _Format: *const i8,
// CHECK-NEXT:     _Locale: *mut __crt_locale_pointers,
// CHECK-NEXT:     _ArgList: *mut i8,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __stdio_common_vfprintf(
// CHECK-NEXT:             *__local_stdio_printf_options(),
// CHECK-NEXT:             _Stream,
// CHECK-NEXT:             _Format,
// CHECK-NEXT:             _Locale,
// CHECK-NEXT:             _ArgList,
// CHECK-NEXT:         )
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: unsafe extern "C-unwind" fn printf(_Format: *const i8, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut _Result: i32 = 0;
// CHECK-NEXT:     let mut _ArgList: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *std::ptr::addr_of_mut!(_ArgList) =
// CHECK-NEXT:             core::mem::transmute::<core::ffi::VaList<'_>, *mut i8>(__va_args.clone());
// CHECK-NEXT:     }
// CHECK-NEXT:     _Result = _vfprintf_l(
// CHECK-NEXT:         unsafe { __acrt_iob_func(1u32) },
// CHECK-NEXT:         _Format,
// CHECK-NEXT:         std::ptr::null_mut::<__crt_locale_pointers>(),
// CHECK-NEXT:         _ArgList,
// CHECK-NEXT:     );
// CHECK-NEXT:     _ArgList = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     _Result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "alpha")]
// CHECK-NEXT: fn only_alpha() -> i32 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "beta"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table: [i32; 3] = [1, 2, 3];
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "beta")]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table: [i32; 2] = [1, 2];
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(all(
// CHECK-NEXT:         target_arch = "x86_64",
// CHECK-NEXT:         target_os = "linux",
// CHECK-NEXT:         target_env = "gnu",
// CHECK-NEXT:         feature = "alpha"
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", only_alpha());
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(
// CHECK-NEXT:         target_arch = "x86_64",
// CHECK-NEXT:         target_os = "linux",
// CHECK-NEXT:         target_env = "gnu",
// CHECK-NEXT:         not(feature = "beta")
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", 3i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(
// CHECK-NEXT:         target_arch = "x86_64",
// CHECK-NEXT:         target_os = "linux",
// CHECK-NEXT:         target_env = "gnu",
// CHECK-NEXT:         feature = "beta"
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", 2i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(
// CHECK-NEXT:         target_arch = "x86_64",
// CHECK-NEXT:         windows,
// CHECK-NEXT:         target_env = "msvc",
// CHECK-NEXT:         feature = "alpha"
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), only_alpha()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(
// CHECK-NEXT:         target_arch = "x86_64",
// CHECK-NEXT:         windows,
// CHECK-NEXT:         target_env = "msvc",
// CHECK-NEXT:         not(feature = "beta")
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), 3i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc", feature = "beta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), 2i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
