#include <stddef.h>
#include <stdio.h>

typedef unsigned long ulg;
typedef unsigned int uInt;

static ulg modulo(ulg value) {
    value %= 65521U;
    return value;
}

static ulg from_uint(unsigned value) { return value; }

static ulg from_uint_cast(uInt value) { return (ulg)value; }

static ulg from_size(size_t value) { return (ulg)value; }

static long from_int(int value) { return value; }

static long long sum_long(long left, long long right) { return left + right; }

static long keeps_arms(long left, unsigned right) { return left + right; }

static unsigned same_spelling(unsigned value) { return (unsigned)value; }

int main(void) {
    printf("%lu\n", modulo(70000));
    printf("%lu\n", from_uint(7));
    printf("%lu\n", from_uint_cast(8));
    printf("%lu\n", from_size(9));
    printf("%ld\n", from_int(-10));
    printf("%lld\n", sum_long(11, 12));
    printf("%ld\n", keeps_arms(13, 14));
    printf("%u\n", same_spelling(15));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   spelled_conversions.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   spelled_conversions.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit spelled_conversions: 16 items, 2 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:keeps_arms: 2 variants
// CHECK-NEXT: macros: 0 function-like, 0 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(unix)]
// CHECK-NEXT: fn keeps_arms(left: c_long, right: u32) -> c_long {
// CHECK-NEXT:     left + (right as c_long)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: fn keeps_arms(left: c_long, right: u32) -> c_long {
// CHECK-NEXT:     (left as c_ulong).wrapping_add(right as c_ulong) as c_long
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
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lu\n".as_ptr(), modulo((70000 as c_ulong))) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lu\n".as_ptr(), from_uint(7u32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lu\n".as_ptr(), from_uint_cast(8u32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lu\n".as_ptr(), from_size((9 as size_t))) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%ld\n".as_ptr(), from_int(-10i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lld\n".as_ptr(), sum_long((11 as c_long), 12i64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%ld\n".as_ptr(), keeps_arms((13 as c_long), 14u32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%u\n".as_ptr(), same_spelling(15u32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn from_int(value: i32) -> c_long {
// CHECK-NEXT:     value as c_long
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn from_size(value: size_t) -> c_ulong {
// CHECK-NEXT:     value as c_ulong
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn from_uint(value: u32) -> c_ulong {
// CHECK-NEXT:     value as c_ulong
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn from_uint_cast(value: u32) -> c_ulong {
// CHECK-NEXT:     value as c_ulong
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn modulo(mut value: c_ulong) -> c_ulong {
// CHECK-NEXT:     value %= 65521;
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn same_spelling(value: u32) -> u32 {
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sum_long(left: c_long, right: i64) -> i64 {
// CHECK-NEXT:     (left as i64) + right
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NEXT: use core::ffi::c_ulong;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
