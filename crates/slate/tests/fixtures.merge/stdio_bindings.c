// merge-sources: units/stdio_bindings_layout.c
// merge-extra-targets: android-x86_64 x86_64-linux-android clang
#include <stdio.h>

extern int stream_layout(void);

#define LOG(x) { fprintf x; }

static void report(FILE *stream, int value) {
    LOG((stream, "value %d\n", value));
}

int main(void) {
    FILE *saved = stdout;
    report(stderr, 1);
    report(stdout, 2);
    fputs("three\n", stdout);
    putc('4', stdout);
    fputc('\n', stdout);
    fflush(saved);
#ifdef __GLIBC__
    stderr = stdout;
#endif
    fprintf(stderr, "reassigned %d\n", 5);
    fprintf(stdout, "layout %d\n", stream_layout());
    FILE *file = fopen("/nonexistent/slate", "r");
    printf("missing %d\n", file == NULL);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   stdio_bindings.c: [] 1 compiled
// CHECK-NEXT:   units/stdio_bindings_layout.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   stdio_bindings.c: [] 1 compiled
// CHECK-NEXT:   units/stdio_bindings_layout.c: [] 1 compiled
// CHECK-NEXT: target android-x86_64: 2 compiles
// CHECK-NEXT:   stdio_bindings.c: [] 1 compiled
// CHECK-NEXT:   units/stdio_bindings_layout.c: [] 1 compiled
// CHECK-NEXT: total: 6 compiles across 3 targets
// CHECK-NEXT: unit stdio_bindings: 10 items, 3 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:__slate_main: 3 variants
// CHECK-NEXT: unit stdio_bindings_layout: 16 items, 15 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:stream_layout: 3 variants
// CHECK-NEXT: macros: 1 function-like, 1 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro LOG: 3 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 3 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/stdio_bindings_layout.c
// CHECK-NEXT: //!
// CHECK-NEXT: //! merge-extra-targets: android-x86_64 x86_64-linux-android clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "android")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "stdio_bindings_layout.rs"]
// CHECK-NEXT: mod __slate_unit_stdio_bindings_layout;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! LOG {
// CHECK-NEXT:     ($x__0:expr, $x__1:expr, $x__2:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::fprintf($x__0, ($x__1) as *const c_char, $x__2) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! LOG {
// CHECK-NEXT:     ($x__0:expr, $x__1:expr, $x__2:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::fprintf($x__0, ($x__1) as *const c_char, $x__2) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stderr: *mut slate_support::FILE;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush([[ARG_0:arg[0-9]+]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT:     fn stream_layout() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-EMPTY:
// CHECK-NEXT: fn report(stream: *mut slate_support::FILE, value: i32) {
// CHECK-NEXT:     LOG!(stream, c"value %d\n".as_ptr() as *mut c_char, value);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let saved: *mut slate_support::FILE = slate_support::stdout();
// CHECK-NEXT:     report(slate_support::stderr(), 1i32);
// CHECK-NEXT:     report(slate_support::stdout(), 2i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::fputs(c"three\n".as_ptr(), slate_support::stdout()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::putc(52i32, slate_support::stdout()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::fputc(10i32, slate_support::stdout()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { fflush(saved) };
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             stderr = slate_support::stdout();
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::fprintf(slate_support::stderr(), c"reassigned %d\n".as_ptr(), 5i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::fprintf(slate_support::stderr(), c"reassigned %d\n".as_ptr(), 5i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::fprintf(
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"layout %d\n".as_ptr(),
// CHECK-NEXT:                 stream_layout(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::fprintf(
// CHECK-NEXT:                 slate_support::stdout(),
// CHECK-NEXT:                 c"layout %d\n".as_ptr(),
// CHECK-NEXT:                 stream_layout(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let file: *mut slate_support::FILE =
// CHECK-NEXT:         unsafe { slate_support::fopen(c"/nonexistent/slate".as_ptr(), c"r".as_ptr()) };
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("missing {}", file.is_null() as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { slate_support::printf(c"missing %d\n".as_ptr(), file.is_null() as i32) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NEXT: file stdio_bindings_layout.rs
// CHECK-NEXT: #[cfg(target_os = "android")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut __sF: [__sFILE; 0];
// CHECK-NEXT:     fn fputc([[ARG_0]]: i32, [[ARG_1:arg[0-9]+]]: *mut __sFILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __acrt_iob_func([[ARG_0]]: u32) -> *mut _iobuf;
// CHECK-NEXT:     fn fputc([[ARG_0]]: i32, [[ARG_1]]: *mut _iobuf) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stdout: *mut _IO_FILE;
// CHECK-NEXT:     fn fputc([[ARG_0]]: i32, [[ARG_1]]: *mut _IO_FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _iobuf {
// CHECK-NEXT:     _Placeholder: *mut (),
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_os = "android")]
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __sFILE {
// CHECK-NEXT:     __private: [c_char; 152],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(target_env = "gnu")]
// CHECK-NEXT: use core::ffi::c_long;
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
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
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
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn stream_layout() -> i32 {
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     let write: Option<unsafe extern "C-unwind" fn(i32, *mut _IO_FILE) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut _IO_FILE) -> i32>,
// CHECK-NEXT:         >(fputc as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(target_env = "gnu")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { write.unwrap()(119i32, stdout) };
// CHECK-NEXT:         unsafe { write.unwrap()(10i32, stdout) };
// CHECK-NEXT:         { unsafe { (*stdout)._fileno } }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let write: Option<unsafe extern "C-unwind" fn(i32, *mut _iobuf) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32, *mut _iobuf) -> i32>>(
// CHECK-NEXT:             fputc as *const (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { write.unwrap()(119i32, __acrt_iob_func(1u32)) };
// CHECK-NEXT:         unsafe { write.unwrap()(10i32, __acrt_iob_func(1u32)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     let write: Option<unsafe extern "C-unwind" fn(i32, *mut __sFILE) -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(i32, *mut __sFILE) -> i32>,
// CHECK-NEXT:         >(fputc as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     #[cfg(target_os = "android")]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             write.unwrap()(
// CHECK-NEXT:                 119i32,
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(1),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { write.unwrap()(10i32, (std::ptr::addr_of_mut!(__sF) as *mut __sFILE).add(1)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(target_env = "gnu"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         1
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
