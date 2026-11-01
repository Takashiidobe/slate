// merge-extra-targets: android-x86_64 x86_64-linux-android clang
#include <stdlib.h>
#include <stdio.h>

static int verbose = 1;

#define TRACE(x) { if (verbose > 0) fprintf x; }
#define CHECK(c, m) { if (!(c)) fputs(m, stderr); }

static void report(int value) {
    fprintf(stderr, "value %d\n", value);
}

int main(void) {
    printf("one %d\n", 1);
    printf("two %s\n", "x");
    TRACE((stderr, "traced %d\n", 2));
    CHECK(verbose == 0, "checked\n");
    fputs("three\n", stdout);
    fprintf(stdout, "four %s\n", "x");
    printf("five %d\n", 5);
    fwrite("written\n", 1, 8, stderr);
    fputc('6', stdout);
    fputc('\n', stderr);
    printf("seven %d\n", 7);
    printf("eight");
    report(8);
    printf(" nine\n");
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   flush_streams.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   flush_streams.c: [] 1 compiled
// CHECK-NEXT: target android-x86_64: 1 compiles
// CHECK-NEXT:   flush_streams.c: [] 1 compiled
// CHECK-NEXT: total: 3 compiles across 3 targets
// CHECK-NEXT: unit flush_streams: 47 items, 38 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:report: 3 variants
// CHECK-NEXT:   spliced fn:__slate_main: 3 variants
// CHECK-NEXT: projection: 18 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-extra-targets: android-x86_64 x86_64-linux-android clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "android")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! TRACE {
// CHECK-NEXT:     ($($x:expr),*) => {
// CHECK-NEXT:         if (unsafe { verbose }) > 0 {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { fprintf($($x),*) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: macro_rules! CHECK {
// CHECK-NEXT:     ($c:expr, $m:expr) => {
// CHECK-NEXT:         if !$c {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { fputs($m, stderr) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! CHECK {
// CHECK-NEXT:     ($c:expr, $m:expr) => {
// CHECK-NEXT:         if !$c {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { fputs($m, __acrt_iob_func(2u32)) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "android")]
// CHECK-NEXT: macro_rules! CHECK {
// CHECK-NEXT:     ($c:expr, $m:expr) => {
// CHECK-NEXT:         if !$c {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { fputs($m, (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(2)) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "android")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut __sF: [__sFILE; 0];
// CHECK-NEXT:     fn fprintf([[ARG_0:arg[0-9]+]]: *mut __sFILE, [[ARG_1:arg[0-9]+]]: *const c_char, ...) -> i32;
// CHECK-NEXT:     fn fputc([[ARG_0]]: i32, [[ARG_1]]: *mut __sFILE) -> i32;
// CHECK-NEXT:     fn fputs([[ARG_0]]: *const c_char, [[ARG_1]]: *mut __sFILE) -> i32;
// CHECK-NEXT:     fn fwrite([[ARG_0]]: *const (), [[ARG_1]]: size_t, [[ARG_2:arg[0-9]+]]: size_t, [[ARG_3:arg[0-9]+]]: *mut __sFILE) -> size_t;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __va_start([[ARG_0]]: *mut *mut c_char, ...);
// CHECK-NEXT:     fn __acrt_iob_func([[ARG_0]]: u32) -> *mut _iobuf;
// CHECK-NEXT:     fn fputc([[ARG_0]]: i32, [[ARG_1]]: *mut _iobuf) -> i32;
// CHECK-NEXT:     fn fputs([[ARG_0]]: *const c_char, [[ARG_1]]: *mut _iobuf) -> i32;
// CHECK-NEXT:     fn fwrite([[ARG_0]]: *const (), [[ARG_1]]: size_t, [[ARG_2]]: size_t, [[ARG_3]]: *mut _iobuf) -> size_t;
// CHECK-NEXT:     fn __stdio_common_vfprintf(
// CHECK-NEXT:         [[ARG_0]]: u64,
// CHECK-NEXT:         [[ARG_1]]: *mut _iobuf,
// CHECK-NEXT:         [[ARG_2]]: *const c_char,
// CHECK-NEXT:         [[ARG_3]]: *mut __crt_locale_pointers,
// CHECK-NEXT:         [[ARG_4:arg[0-9]+]]: *mut c_char,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stdout: *mut _IO_FILE;
// CHECK-NEXT:     static mut stderr: *mut _IO_FILE;
// CHECK-NEXT:     fn fprintf([[ARG_0]]: *mut _IO_FILE, [[ARG_1]]: *const c_char, ...) -> i32;
// CHECK-NEXT:     fn fputc([[ARG_0]]: i32, [[ARG_1]]: *mut _IO_FILE) -> i32;
// CHECK-NEXT:     fn fputs([[ARG_0]]: *const c_char, [[ARG_1]]: *mut _IO_FILE) -> i32;
// CHECK-NEXT:     fn fwrite([[ARG_0]]: *const (), [[ARG_1]]: size_t, [[ARG_2]]: size_t, [[ARG_3]]: *mut _IO_FILE) -> size_t;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0]]: *const c_char, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "android")]
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __sFILE {
// CHECK-NEXT:     __private: [c_char; 152],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_locale_pointers {
// CHECK-NEXT:     locinfo: *mut __crt_locale_data,
// CHECK-NEXT:     mbcinfo: *mut __crt_multibyte_data,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_locale_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_multibyte_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn __local_stdio_printf_options() -> *mut u64 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(_OptionsStorage) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: static mut _OptionsStorage: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _iobuf {
// CHECK-NEXT:     _Placeholder: *mut (),
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn _vfprintf_l(
// CHECK-NEXT:     _Stream: *mut _iobuf,
// CHECK-NEXT:     _Format: *const c_char,
// CHECK-NEXT:     _Locale: *mut __crt_locale_pointers,
// CHECK-NEXT:     _ArgList: *mut c_char,
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
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C-unwind" fn fprintf(
// CHECK-NEXT:     _Stream: *mut _iobuf,
// CHECK-NEXT:     _Format: *const c_char,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let mut _Result: i32 = 0;
// CHECK-NEXT:     let mut _ArgList: *mut c_char = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *std::ptr::addr_of_mut!(_ArgList) =
// CHECK-NEXT:             core::mem::transmute::<core::ffi::VaList<'_>, *mut i8>(__va_args.clone());
// CHECK-NEXT:     }
// CHECK-NEXT:     _Result = _vfprintf_l(
// CHECK-NEXT:         _Stream,
// CHECK-NEXT:         _Format,
// CHECK-NEXT:         std::ptr::null_mut::<__crt_locale_pointers>(),
// CHECK-NEXT:         _ArgList,
// CHECK-NEXT:     );
// CHECK-NEXT:     _ArgList = std::ptr::null_mut::<c_char>();
// CHECK-NEXT:     _Result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C-unwind" fn printf(_Format: *const c_char, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut _Result: i32 = 0;
// CHECK-NEXT:     let mut _ArgList: *mut c_char = unsafe { std::mem::zeroed() };
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
// CHECK-NEXT:     _ArgList = std::ptr::null_mut::<c_char>();
// CHECK-NEXT:     _Result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_FILE {
// CHECK-NEXT:     _flags: i32,
// CHECK-NEXT:     _IO_read_ptr: *mut c_char,
// CHECK-NEXT:     _IO_read_end: *mut c_char,
// CHECK-NEXT:     _IO_read_base: *mut c_char,
// CHECK-NEXT:     _IO_write_base: *mut c_char,
// CHECK-NEXT:     _IO_write_ptr: *mut c_char,
// CHECK-NEXT:     _IO_write_end: *mut c_char,
// CHECK-NEXT:     _IO_buf_base: *mut c_char,
// CHECK-NEXT:     _IO_buf_end: *mut c_char,
// CHECK-NEXT:     _IO_save_base: *mut c_char,
// CHECK-NEXT:     _IO_backup_base: *mut c_char,
// CHECK-NEXT:     _IO_save_end: *mut c_char,
// CHECK-NEXT:     _markers: *mut _IO_marker,
// CHECK-NEXT:     _chain: *mut _IO_FILE,
// CHECK-NEXT:     _fileno: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT:     _short_backupbuf: [c_char; 1],
// CHECK-NEXT:     _old_offset: c_long,
// CHECK-NEXT:     _cur_column: u16,
// CHECK-NEXT:     _vtable_offset: i8,
// CHECK-NEXT:     _shortbuf: [c_char; 1],
// CHECK-NEXT:     _lock: *mut (),
// CHECK-NEXT:     _offset: c_long,
// CHECK-NEXT:     _codecvt: *mut _IO_codecvt,
// CHECK-NEXT:     _wide_data: *mut _IO_wide_data,
// CHECK-NEXT:     _freeres_list: *mut _IO_FILE,
// CHECK-NEXT:     _freeres_buf: *mut (),
// CHECK-NEXT:     _prevchain: *mut *mut _IO_FILE,
// CHECK-NEXT:     _mode: i32,
// CHECK-NEXT:     _unused3: i32,
// CHECK-NEXT:     _total_written: u64,
// CHECK-NEXT:     _unused2: [c_char; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_marker {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_codecvt {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_wide_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut verbose: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: fn report(value: i32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fprintf(stderr, c"value %d\n".as_ptr(), value) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fprintf(__acrt_iob_func(2u32), c"value %d\n".as_ptr(), value) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             fprintf(
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(2),
// CHECK-NEXT:                 c"value %d\n".as_ptr(),
// CHECK-NEXT:                 value,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"one %d\n".as_ptr(), 1i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"two %s\n".as_ptr(), c"x".as_ptr() as *mut c_char) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("one {}", 1i32);
// CHECK-NEXT:         unsafe { printf(c"two %s\n".as_ptr(), c"x".as_ptr() as *mut c_char) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         TRACE!(
// CHECK-NEXT:             unsafe { __acrt_iob_func(2u32) },
// CHECK-NEXT:             c"traced %d\n".as_ptr(),
// CHECK-NEXT:             2i32
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         TRACE!(unsafe { stderr }, c"traced %d\n".as_ptr(), 2i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     {
// CHECK-NEXT:         TRACE!(
// CHECK-NEXT:             unsafe { (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(2) },
// CHECK-NEXT:             c"traced %d\n".as_ptr(),
// CHECK-NEXT:             2i32
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     CHECK!((unsafe { verbose }) == 0, c"checked\n".as_ptr());
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fputs(c"three\n".as_ptr(), __acrt_iob_func(1u32)) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             fprintf(
// CHECK-NEXT:                 __acrt_iob_func(1u32),
// CHECK-NEXT:                 c"four %s\n".as_ptr(),
// CHECK-NEXT:                 c"x".as_ptr() as *mut c_char,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"five %d\n".as_ptr(), 5i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fputs(c"three\n".as_ptr(), stdout) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             fputs(
// CHECK-NEXT:                 c"three\n".as_ptr(),
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(1),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             fwrite(
// CHECK-NEXT:                 b"written\n\0".as_ptr() as *const (),
// CHECK-NEXT:                 1u64,
// CHECK-NEXT:                 8u64,
// CHECK-NEXT:                 __acrt_iob_func(2u32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fprintf(stdout, c"four %s\n".as_ptr(), c"x".as_ptr() as *mut c_char) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             fprintf(
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(1),
// CHECK-NEXT:                 c"four %s\n".as_ptr(),
// CHECK-NEXT:                 c"x".as_ptr() as *mut c_char,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fputc(54i32, __acrt_iob_func(1u32)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("five {}", 5i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             fwrite(
// CHECK-NEXT:                 b"written\n\0".as_ptr() as *const (),
// CHECK-NEXT:                 (1 as c_ulong),
// CHECK-NEXT:                 (8 as c_ulong),
// CHECK-NEXT:                 stderr,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             fwrite(
// CHECK-NEXT:                 b"written\n\0".as_ptr() as *const (),
// CHECK-NEXT:                 (1 as c_ulong),
// CHECK-NEXT:                 (8 as c_ulong),
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(2),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fputc(10i32, __acrt_iob_func(2u32)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fputc(54i32, stdout) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fputc(54i32, (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(1)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"seven %d\n".as_ptr(), 7i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fputc(10i32, stderr) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { fputc(10i32, (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(2)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("seven {}", 7i32);
// CHECK-NEXT:         print!("eight");
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"eight".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     report(8i32);
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c" nine\n".as_ptr()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(" nine");
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
