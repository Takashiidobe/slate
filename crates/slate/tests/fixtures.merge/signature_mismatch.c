// merge-sources: units/signature_mismatch_helper.c
// merge-expect-exit: 1
#include <stdio.h>

struct pair {
    int first;
};

extern int widen(int);
extern struct pair make_pair(int);
extern int table_size;

int main(void) {
    printf("%d %d %d\n", widen(1), make_pair(2).first, table_size);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 7 compiles
// CHECK-NEXT:   signature_mismatch.c: [] 1 compiled
// CHECK-NEXT:   units/signature_mismatch_helper.c: [alpha,beta,gamma] 6 compiled
// CHECK-NEXT: target windows-x86_64: 7 compiles
// CHECK-NEXT:   signature_mismatch.c: [] 1 compiled
// CHECK-NEXT:   units/signature_mismatch_helper.c: [alpha,beta,gamma] 6 compiled
// CHECK-NEXT: total: 14 compiles across 2 targets
// CHECK-NEXT: unit signature_mismatch: 20 items, 14 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:__slate_main: 2 variants
// CHECK-NEXT: unit signature_mismatch_helper: 11 items, 10 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:make_pair: 2 variants
// CHECK-NEXT:   hoisted fn:widen: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 6 shared symbols, 4 mismatches
// CHECK-NEXT:   linux-x86_64 [beta]: make_pair: signature_mismatch `extern "C" fn(i32) -> struct{i32}` vs signature_mismatch_helper `extern "C" fn(i32) -> struct{i32, i64}`
// CHECK-NEXT:   linux-x86_64 [gamma]: table_size: signature_mismatch `i32` vs signature_mismatch_helper `i64`
// CHECK-NEXT:   linux-x86_64 [alpha]: widen: signature_mismatch `extern "C" fn(i32) -> i32` vs signature_mismatch_helper `extern "C" fn(i64) -> i64`
// CHECK-NEXT:   windows-x86_64 [beta]: make_pair: signature_mismatch `extern "C" fn(i32) -> struct{i32}` vs signature_mismatch_helper `extern "C" fn(i32) -> struct{i32, i32}`
// CHECK-NEXT: error: 4 signature mismatches
// CHECK-NEXT: exit: 1
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/signature_mismatch_helper.c
// CHECK-NEXT: //!
// CHECK-NEXT: //! merge-expect-exit: 1
// CHECK-NEXT: #[path = "signature_mismatch_helper.rs"]
// CHECK-NEXT: mod __slate_unit_signature_mismatch_helper;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut table_size: i32;
// CHECK-NEXT:     fn widen([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn make_pair([[ARG_0]]: i32) -> pair;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __va_start([[ARG_0]]: *mut *mut i8, ...);
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
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
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
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn __local_stdio_printf_options() -> *mut u64 {
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
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn _vfprintf_l(
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
// CHECK-NEXT: #[unsafe(no_mangle)]
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         unsafe { widen(1i32) },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = make_pair(2i32);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             })
// CHECK-NEXT:             .first
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe { table_size }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:             widen(1i32),
// CHECK-NEXT:             (*{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = make_pair(2i32);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             })
// CHECK-NEXT:             .first,
// CHECK-NEXT:             table_size,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
// CHECK-NEXT: file signature_mismatch_helper.rs
// CHECK-NEXT: #[cfg(not(feature = "beta"))]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     feature = "beta"
// CHECK-NEXT: ))]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT:     second: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc", feature = "beta"))]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT:     second: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(any(
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc"),
// CHECK-NEXT:     all(
// CHECK-NEXT:         target_arch = "x86_64",
// CHECK-NEXT:         target_os = "linux",
// CHECK-NEXT:         target_env = "gnu",
// CHECK-NEXT:         not(feature = "alpha")
// CHECK-NEXT:     )
// CHECK-NEXT: ))]
// CHECK-NEXT: type __SlateHoist_widen = i32;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     feature = "alpha"
// CHECK-NEXT: ))]
// CHECK-NEXT: type __SlateHoist_widen = i64;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn widen(x: __SlateHoist_widen) -> __SlateHoist_widen {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "beta"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn make_pair(x: i32) -> pair {
// CHECK-NEXT:     let mut pair: pair = pair { first: 0 };
// CHECK-NEXT:     pair.first = x;
// CHECK-NEXT:     return pair;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "beta")]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn make_pair(x: i32) -> pair {
// CHECK-NEXT:     let mut pair: pair = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].first = 0i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     pair.first = x;
// CHECK-NEXT:     return pair;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "gamma"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table_size: i32 = 4;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     feature = "gamma"
// CHECK-NEXT: ))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table_size: i64 = 3;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     windows,
// CHECK-NEXT:     target_env = "msvc",
// CHECK-NEXT:     feature = "gamma"
// CHECK-NEXT: ))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table_size: i32 = 3;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
