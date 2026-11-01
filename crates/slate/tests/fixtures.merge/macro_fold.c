// merge-sources: units/macro_fold_helper.c
#include <stdio.h>
#include <stdlib.h>

extern int helper(int);

static int verbose = 1;
static int bumps;

static void report(const char *message) {
    printf("error: %s\n", message);
}

static int next_id(int *state) {
    *state += 1;
    return *state;
}

#define LOG(x) { fprintf x; }
#define DIFFERS(a) { printf("main %d\n", a); }
#define ASSERT(cond, msg) { if (!(cond)) report(msg); }
#define NOTE(c, x) { if (verbose > 0 && (c)) fprintf x; }
#define EARLY_LIST(x, c) { if (c) fprintf x; }
#define TWICE(a) { printf("%d %d\n", a, a); }
#define DROPS(a, b) { printf("%d\n", a); }
#define STORE(p, v) { *(p)++ = (v); }
#define LOCALS(a) { int t = (a); printf("%d\n", t + 1); }
#define SHADOWS(a) { int t = 5; printf("%d\n", (a) + t); }
#define READS_LOCAL(a) { printf("%d\n", (a) + count); }
#define WRITES_LOCAL(a) { count += (a); }
#define RETURNS(a) { if ((a) < 0) return -1; }
#define COMMA_RETURN(a) return (bumps = (a), (a))
#define NAME(a) { printf("%s %d\n", #a, a); }
#define PASTE(a) { printf("%d\n", a##_value); }
#define VARIADIC(...) { printf(__VA_ARGS__); }
#define OPEN(c) if (c) {
#define CLOSE }
#define DECLARE(a) int a = 7;
#define SPLIT(a) int split = (a); printf("%d\n", split); printf("%d\n", (a));
#define print(a) { printf("%d\n", a); }
#define type(a) { printf("%d\n", a); }
#define MOVED_CONST(a) { printf("%d\n", (a) + 1); }
#define BUMPED(a) { printf("%d\n", a); bumps += 1; }

static int comma_return(int a) {
    COMMA_RETURN(a);
}

static int early(int a) {
    RETURNS(a);
    return a * 2;
}

static int with_goto(int a, int b) {
    int i = 0;
    LOG((stdout, "goto entry\n"));
again:
    ASSERT(a == b, "goto mismatch");
    LOG((stdout, "goto %d\n", i));
    NOTE(i > 0, (stdout, "goto note %d\n", i));
    i++;
    if (i < 2) goto again;
    return i;
}

static int with_switch(int mode, int count) {
    switch (mode) {
    case 0:
        LOG((stdout, "case zero %d\n", count));
        count++;
    case 1:
        ASSERT(count != 0, "count was zero");
        LOG((stdout, "case one %d\n", count));
        break;
    default:
        LOG((stdout, "default\n"));
    }
    return count;
}

int main(void) {
    int count = 2;
    int state = 0;
    int t = 40;
    int a_value = 9;
    char buffer[4] = {0};
    char *cursor = buffer;

    LOG((stdout, "start %d\n", count));
    LOG((stdout, "no args\n"));
    DIFFERS(count);
    ASSERT(count == 3, "count is not three");
    ASSERT(next_id(&state) == 1, "side effect");
    NOTE(count > 1, (stdout, "note %d\n", count));
    EARLY_LIST((stdout, "early list\n"), count);
    TWICE(count + 1);
    DROPS(count, next_id(&state));
    STORE(cursor, 'x');
    STORE(cursor, 'y');
    LOCALS(count);
    SHADOWS(count);
    SHADOWS(t);
    READS_LOCAL(1);
    WRITES_LOCAL(1);
    NAME(count);
    PASTE(a);
    VARIADIC("%d\n", count);
    OPEN(count > 0)
        printf("opened\n");
    CLOSE
    DECLARE(declared);
    printf("%d\n", declared);
    SPLIT(count);
    print(count);
    type(count);
    MOVED_CONST(1);
    MOVED_CONST(count);
    BUMPED(count);
    BUMPED(count + 1);
    for (int i = 0; i < 2; i++) {
        LOG((stdout, "loop %d\n", i));
    }
    printf("%s %d %d %d %d\n", buffer, state, bumps, count, helper(count));
    printf("%d %d %d\n", early(-1), early(4), comma_return(6));
    printf("%d %d\n", with_goto(1, 1), with_switch(0, 0));
    printf("%d\n", with_switch(1, 5) + with_switch(9, 5));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles
// CHECK-NEXT:   macro_fold.c: [] 1 compiled
// CHECK-NEXT:   units/macro_fold_helper.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 2 compiles
// CHECK-NEXT:   macro_fold.c: [] 1 compiled
// CHECK-NEXT:   units/macro_fold_helper.c: [] 1 compiled
// CHECK-NEXT: total: 4 compiles across 2 targets
// CHECK-NEXT: unit macro_fold: 52 items, 35 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:report: 2 variants
// CHECK-NEXT:   spliced fn:with_goto: 2 variants
// CHECK-NEXT:   spliced fn:with_switch: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: unit macro_fold_helper: 29 items, 25 gated, 0 duplicated functions
// CHECK-NEXT:   spliced fn:helper: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 10 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file __slate_macros.rs
// CHECK-NEXT: macro_rules! LOG {
// CHECK-NEXT:     ($($x:expr),*) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { fprintf($($x),*) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: file macro_fold_helper.rs
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! ONLY_HERE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", $a * 3);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! ONLY_HERE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), $a * 3) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! DIFFERS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("helper {}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! DIFFERS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"helper %d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __va_start([[ARG_0:arg[0-9]+]]: *mut *mut c_char, ...);
// CHECK-NEXT:     fn __acrt_iob_func([[ARG_0]]: u32) -> *mut _iobuf;
// CHECK-NEXT:     fn __stdio_common_vfprintf(
// CHECK-NEXT:         [[ARG_0]]: u64,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *mut _iobuf,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: *const c_char,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *mut __crt_locale_pointers,
// CHECK-NEXT:         [[ARG_4:arg[0-9]+]]: *mut c_char,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stdout: *mut _IO_FILE;
// CHECK-NEXT:     fn fprintf([[ARG_0]]: *mut _IO_FILE, [[ARG_1]]: *const c_char, ...) -> i32;
// CHECK-NEXT:     fn printf([[ARG_0]]: *const c_char, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: type c_char = i8;
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
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: type c_long = i64;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
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
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_marker {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_codecvt {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_wide_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn helper(value: i32) -> i32 {
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         LOG!(unsafe { stdout }, c"helper %d\n".as_ptr(), value);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         LOG!(
// CHECK-NEXT:             unsafe { __acrt_iob_func(1u32) },
// CHECK-NEXT:             c"helper %d\n".as_ptr(),
// CHECK-NEXT:             value
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     ONLY_HERE!(value);
// CHECK-NEXT:     DIFFERS!(value);
// CHECK-NEXT:     value + 1
// CHECK-NEXT: }
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/macro_fold_helper.c
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[macro_use]
// CHECK-NEXT: mod __slate_macros;
// CHECK-EMPTY:
// CHECK-NEXT: #[path = "macro_fold_helper.rs"]
// CHECK-NEXT: mod __slate_unit_macro_fold_helper;
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ASSERT {
// CHECK-NEXT:     ($cond:expr, $msg:expr) => {
// CHECK-NEXT:         if !$cond {
// CHECK-NEXT:             report($msg);
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! NOTE {
// CHECK-NEXT:     ($c:expr, $($x:expr),*) => {
// CHECK-NEXT:         if (unsafe { verbose }) > 0 && $c {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe { fprintf($($x),*) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! DIFFERS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("main {}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! DIFFERS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"main %d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! TWICE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{} {}", $a, $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! TWICE {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%d %d\n".as_ptr(), $a, $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! DROPS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", $a);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! DROPS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! LOCALS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let t: i32 = $a;
// CHECK-NEXT:         println!("{}", t + 1);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! LOCALS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let t: i32 = $a;
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), t + 1) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! SHADOWS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let t: i32 = 5;
// CHECK-NEXT:         println!("{}", $a + t);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! SHADOWS {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let t: i32 = 5;
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), $a + t) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! MOVED_CONST {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", $a + 1);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! MOVED_CONST {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), $a + 1) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: macro_rules! BUMPED {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         println!("{}", $a);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bumps = bumps + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: macro_rules! BUMPED {
// CHECK-NEXT:     ($a:expr) => {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), $a) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             bumps = bumps + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __va_start([[ARG_0]]: *mut *mut c_char, ...);
// CHECK-NEXT:     fn __acrt_iob_func([[ARG_0]]: u32) -> *mut _iobuf;
// CHECK-NEXT:     fn __stdio_common_vfprintf(
// CHECK-NEXT:         [[ARG_0]]: u64,
// CHECK-NEXT:         [[ARG_1]]: *mut _iobuf,
// CHECK-NEXT:         [[ARG_2]]: *const c_char,
// CHECK-NEXT:         [[ARG_3]]: *mut __crt_locale_pointers,
// CHECK-NEXT:         [[ARG_4]]: *mut c_char,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stdout: *mut _IO_FILE;
// CHECK-NEXT:     fn fprintf([[ARG_0]]: *mut _IO_FILE, [[ARG_1]]: *const c_char, ...) -> i32;
// CHECK-NEXT:     fn printf([[ARG_0]]: *const c_char, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn helper([[ARG_0]]: i32) -> i32;
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: type c_char = i8;
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
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: type c_long = i64;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
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
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_marker {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_codecvt {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_wide_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut verbose: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: static mut bumps: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn report(message: *const c_char) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"error: %s\n".as_ptr(), message) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"error: %s\n".as_ptr(), message) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn next_id(state: &mut i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *state = *state + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *(state as *mut i32) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn comma_return(a: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         bumps = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn early(a: i32) -> i32 {
// CHECK-NEXT:     if a < 0 {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     a * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn with_goto(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             LOG!(unsafe { stdout }, c"goto entry\n".as_ptr());
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             LOG!(__acrt_iob_func(1u32), c"goto entry\n".as_ptr());
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             ASSERT!(a == b, c"goto mismatch".as_ptr());
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(unsafe { stdout }, c"goto %d\n".as_ptr(), i);
// CHECK-NEXT:                 NOTE!(i > 0, unsafe { stdout }, c"goto note %d\n".as_ptr(), i);
// CHECK-NEXT:             }
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(__acrt_iob_func(1u32), c"goto %d\n".as_ptr(), i);
// CHECK-NEXT:                 NOTE!(i > 0, __acrt_iob_func(1u32), c"goto note %d\n".as_ptr(), i);
// CHECK-NEXT:             }
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:             if i >= 2 {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return i;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn with_switch(mode: i32, mut count: i32) -> i32 {
// CHECK-NEXT:     match mode {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(unsafe { stdout }, c"case zero %d\n".as_ptr(), count);
// CHECK-NEXT:             }
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(
// CHECK-NEXT:                     unsafe { __acrt_iob_func(1u32) },
// CHECK-NEXT:                     c"case zero %d\n".as_ptr(),
// CHECK-NEXT:                     count
// CHECK-NEXT:                 );
// CHECK-NEXT:             }
// CHECK-NEXT:             count += 1;
// CHECK-NEXT:             ASSERT!(count != 0, c"count was zero".as_ptr());
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(unsafe { stdout }, c"case one %d\n".as_ptr(), count);
// CHECK-NEXT:             }
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(
// CHECK-NEXT:                     unsafe { __acrt_iob_func(1u32) },
// CHECK-NEXT:                     c"case one %d\n".as_ptr(),
// CHECK-NEXT:                     count
// CHECK-NEXT:                 );
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             ASSERT!(count != 0, c"count was zero".as_ptr());
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(unsafe { stdout }, c"case one %d\n".as_ptr(), count);
// CHECK-NEXT:             }
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(
// CHECK-NEXT:                     unsafe { __acrt_iob_func(1u32) },
// CHECK-NEXT:                     c"case one %d\n".as_ptr(),
// CHECK-NEXT:                     count
// CHECK-NEXT:                 );
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(unsafe { stdout }, c"default\n".as_ptr());
// CHECK-NEXT:             }
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 LOG!(unsafe { __acrt_iob_func(1u32) }, c"default\n".as_ptr());
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut count: i32 = 2;
// CHECK-NEXT:     let mut state: i32 = 0;
// CHECK-NEXT:     let a_value: i32 = 9;
// CHECK-NEXT:     let mut buffer: [c_char; 4] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = (0 as c_char);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut cursor: *mut c_char = buffer.as_mut_ptr() as *mut c_char;
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         LOG!(
// CHECK-NEXT:             unsafe { __acrt_iob_func(1u32) },
// CHECK-NEXT:             c"start %d\n".as_ptr(),
// CHECK-NEXT:             count
// CHECK-NEXT:         );
// CHECK-NEXT:         LOG!(unsafe { __acrt_iob_func(1u32) }, c"no args\n".as_ptr());
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         LOG!(unsafe { stdout }, c"start %d\n".as_ptr(), count);
// CHECK-NEXT:         LOG!(unsafe { stdout }, c"no args\n".as_ptr());
// CHECK-NEXT:     }
// CHECK-NEXT:     DIFFERS!(count);
// CHECK-NEXT:     ASSERT!(count == 3, c"count is not three".as_ptr());
// CHECK-NEXT:     ASSERT!(
// CHECK-NEXT:         next_id(unsafe { &mut (*std::ptr::addr_of_mut!(state)) }) == 1,
// CHECK-NEXT:         c"side effect".as_ptr()
// CHECK-NEXT:     );
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         NOTE!(
// CHECK-NEXT:             count > 1,
// CHECK-NEXT:             unsafe { __acrt_iob_func(1u32) },
// CHECK-NEXT:             c"note %d\n".as_ptr(),
// CHECK-NEXT:             count
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         NOTE!(count > 1, unsafe { stdout }, c"note %d\n".as_ptr(), count);
// CHECK-NEXT:     }
// CHECK-NEXT:     if count != 0 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { fprintf(__acrt_iob_func(1u32), c"early list\n".as_ptr()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { fprintf(stdout, c"early list\n".as_ptr()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     TWICE!(count + 1);
// CHECK-NEXT:     DROPS!(count);
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut c_char = cursor;
// CHECK-NEXT:     cursor = unsafe { [[V_0]].add(1) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = 120;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut c_char = cursor;
// CHECK-NEXT:     cursor = unsafe { [[V_1]].add(1) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_1]] = 121;
// CHECK-NEXT:     }
// CHECK-NEXT:     LOCALS!(count);
// CHECK-NEXT:     SHADOWS!(count);
// CHECK-NEXT:     let t: i32 = 5;
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), t + t) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), 1 + count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", t + t);
// CHECK-NEXT:         println!("{}", 1 + count);
// CHECK-NEXT:     }
// CHECK-NEXT:     count += 1;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%s %d\n".as_ptr(), c"count".as_ptr() as *mut c_char, count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), a_value) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%s %d\n".as_ptr(), c"count".as_ptr() as *mut c_char, count) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", a_value);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), count) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), count) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if count > 0 {
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { printf(c"opened\n".as_ptr()) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             println!("opened");
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let declared: i32 = 7;
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), declared) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", declared);
// CHECK-NEXT:     }
// CHECK-NEXT:     let split: i32 = count;
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), split) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), count) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", split);
// CHECK-NEXT:         println!("{}", count);
// CHECK-NEXT:         println!("{}", count);
// CHECK-NEXT:         println!("{}", count);
// CHECK-NEXT:     }
// CHECK-NEXT:     MOVED_CONST!(1i32);
// CHECK-NEXT:     MOVED_CONST!(count);
// CHECK-NEXT:     BUMPED!(count);
// CHECK-NEXT:     BUMPED!(count + 1);
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             LOG!(unsafe { __acrt_iob_func(1u32) }, c"loop %d\n".as_ptr(), i);
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             LOG!(unsafe { stdout }, c"loop %d\n".as_ptr(), i);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 buffer.as_mut_ptr() as *mut c_char,
// CHECK-NEXT:                 state,
// CHECK-NEXT:                 bumps,
// CHECK-NEXT:                 count,
// CHECK-NEXT:                 helper(count),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 early(-1i32),
// CHECK-NEXT:                 early(4i32),
// CHECK-NEXT:                 comma_return(6i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 with_goto(1i32, 1i32),
// CHECK-NEXT:                 with_switch(0i32, 0i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d\n".as_ptr(),
// CHECK-NEXT:                 with_switch(1i32, 5i32) + with_switch(9i32, 5i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 buffer.as_mut_ptr() as *mut c_char,
// CHECK-NEXT:                 state,
// CHECK-NEXT:                 bumps,
// CHECK-NEXT:                 count,
// CHECK-NEXT:                 helper(count),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{} {} {}", early(-1i32), early(4i32), comma_return(6i32));
// CHECK-NEXT:         println!("{} {}", with_goto(1i32, 1i32), with_switch(0i32, 0i32));
// CHECK-NEXT:         println!("{}", with_switch(1i32, 5i32) + with_switch(9i32, 5i32));
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
