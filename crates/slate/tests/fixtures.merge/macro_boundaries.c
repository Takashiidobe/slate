// merge-extra-targets: linux-i686 i686-unknown-linux-gnu clang
#include <stdio.h>
#include <stdlib.h>

static int verbose = 1;

static void report(const char *message) {
    printf("error: %s\n", message);
}

#define ASSERT(cond, msg) { if (!(cond)) report(msg); }
#define TRACE(x) { if (verbose > 0) printf x; }
#define NOTE(c, x) { if (verbose > 0 && (c)) printf x; }
#define QUIET(x)
#define BUMP(x) { count += (x); }
#define GUARD(x) { if (!(x)) return -1; }

static int next_id(int *state) {
    *state += 1;
    return *state;
}

static int checked(int a, int b) {
    int count = 0;
    int state = 0;
    ASSERT(a == b, "a and b differ");
    ASSERT(a + 1 == b + 1, "still differ");
    ASSERT((1 << 3) - 1 == 7, "folded");
    ASSERT(next_id(&state) == 1, "side effect");
    TRACE(("a %d b %d\n", a, b));
    NOTE(a != 0, ("nonzero %d\n", a));
    QUIET(("dropped %d\n", next_id(&state)));
    BUMP(a);
    GUARD(b >= 0);
    if (a > 100) ASSERT(b > 100, "nested");
    for (int i = 0; i < 2; i++) {
        TRACE(("loop %d\n", i));
    }
    printf("state %d count %d\n", state, count);
    return !(a == b);
}

int main(void) {
    printf("%d\n", checked(3, 3));
    printf("%d\n", checked(3, 4));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_boundaries.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_boundaries.c: [] 1 compiled
// CHECK-NEXT: target linux-i686: 1 compiles
// CHECK-NEXT:   macro_boundaries.c: [] 1 compiled
// CHECK-NEXT: total: 3 compiles across 3 targets
// CHECK-NEXT: unit macro_boundaries: 22 items, 15 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:report: 2 variants
// CHECK-NEXT:   spliced fn:checked: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: projection: 18 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-extra-targets: linux-i686 i686-unknown-linux-gnu clang
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(target_arch = "x86", target_os = "linux", target_env = "gnu")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
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
// CHECK-NEXT:     fn printf([[ARG_0]]: *const c_char, ...) -> i32;
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
// CHECK-NEXT: static mut verbose: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: fn report(message: *const c_char) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"error: %s\n".as_ptr(), message) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn report(message: *const c_char) {
// CHECK-NEXT:     unsafe { printf(c"error: %s\n".as_ptr(), message) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn next_id(state: &mut i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *state = *state + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *(state as *mut i32) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn checked(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut count: i32 = 0;
// CHECK-NEXT:     let mut state: i32 = 0;
// CHECK-NEXT:     if !(a == b) {
// CHECK-NEXT:         report(c"a and b differ".as_ptr());
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(a + 1 == b + 1) {
// CHECK-NEXT:         report(c"still differ".as_ptr());
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(7i32 == 7) {
// CHECK-NEXT:         report(c"folded".as_ptr());
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(next_id(unsafe { &mut (*std::ptr::addr_of_mut!(state)) }) == 1) {
// CHECK-NEXT:         report(c"side effect".as_ptr());
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { verbose }) > 0 {
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { printf(c"a %d b %d\n".as_ptr(), a, b) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { printf(c"a %d b %d\n".as_ptr(), a, b) };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { verbose }) > 0 && a != 0 {
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         #[cfg(unix)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { printf(c"nonzero %d\n".as_ptr(), a) };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(windows)]
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { printf(c"nonzero %d\n".as_ptr(), a) };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     count += a;
// CHECK-NEXT:     if b < 0 {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if a > 100 {
// CHECK-NEXT:         if !(b > 100) {
// CHECK-NEXT:             report(c"nested".as_ptr());
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         if (unsafe { verbose }) > 0 {
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             #[cfg(unix)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { printf(c"loop %d\n".as_ptr(), i) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             }
// CHECK-NEXT:             #[cfg(windows)]
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { printf(c"loop %d\n".as_ptr(), i) };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("state {} count {}", state, count);
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"state %d count %d\n".as_ptr(), state, count) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (a != b) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(unix)]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("{}", checked(3i32, 3i32));
// CHECK-NEXT:         println!("{}", checked(3i32, 4i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(windows)]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), checked(3i32, 3i32)) };
// CHECK-NEXT:         unsafe { printf(c"%d\n".as_ptr(), checked(3i32, 4i32)) };
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
