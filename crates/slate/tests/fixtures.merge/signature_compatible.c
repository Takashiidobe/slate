// merge-sources: units/signature_compatible_helper.c
#include <stdio.h>

#ifdef ALPHA
typedef long value_t;
#else
typedef int value_t;
#endif

struct opaque;
typedef struct {
    int q;
} *handle;

extern value_t scale(value_t);
extern long native_long(long);
extern int unprototyped();
extern int table[];
extern int big_unsized[];
extern int big_sized[32];
extern const char *label(void);
extern int read_opaque(struct opaque *);
extern struct opaque *make_opaque(void);
extern int read_handle(handle);
extern void quit(int) __attribute__((noreturn));

int main(void) {
    printf("%ld %ld %d %d %d %d %s %d %d\n", (long)scale(2), native_long(3), unprototyped(4), table[1],
           big_unsized[2], big_sized[3],
           label(), read_opaque(make_opaque()), read_handle(0));
    quit(0);
    return 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 6 compiles
// CHECK-NEXT:   signature_compatible.c: [alpha] 2 compiled
// CHECK-NEXT:   units/signature_compatible_helper.c: [alpha,beta] 4 compiled
// CHECK-NEXT: target windows-x86_64: 6 compiles
// CHECK-NEXT:   signature_compatible.c: [alpha] 2 compiled
// CHECK-NEXT:   units/signature_compatible_helper.c: [alpha,beta] 4 compiled
// CHECK-NEXT: total: 12 compiles across 2 targets
// CHECK-NEXT: unit signature_compatible: 33 items, 20 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:__slate_main: 3 variants
// CHECK-NEXT: unit signature_compatible_helper: 21 items, 8 gated, 1 duplicated functions
// CHECK-NEXT:   duplicated fn:native_long: 2 variants
// CHECK-NEXT:   hoisted fn:scale: 2 variants
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 22 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-sources: units/signature_compatible_helper.c
// CHECK-NEXT: #[path = "signature_compatible_helper.rs"]
// CHECK-NEXT: mod __slate_unit_signature_compatible_helper;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut table: [i32; 0];
// CHECK-NEXT:     static mut big_unsized: [i32; 0];
// CHECK-NEXT:     static mut big_sized: [i32; 32];
// CHECK-NEXT:     fn unprototyped(...) -> i32;
// CHECK-NEXT:     fn label() -> *const i8;
// CHECK-NEXT:     fn read_opaque([[ARG_0:arg[0-9]+]]: *mut opaque) -> i32;
// CHECK-NEXT:     fn make_opaque() -> *mut opaque;
// CHECK-NEXT:     fn read_handle([[ARG_0]]: *mut __SlateRecord_signature_compatible_c_13) -> i32;
// CHECK-NEXT:     fn quit([[ARG_0]]: i32);
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
// CHECK-NEXT:     fn native_long([[ARG_0]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn native_long([[ARG_0]]: i64) -> i64;
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
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
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn scale([[ARG_0]]: i32) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     feature = "alpha"
// CHECK-NEXT: ))]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn scale([[ARG_0]]: i64) -> i64;
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct opaque {}
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
// CHECK-NEXT: struct __SlateRecord_signature_compatible_c_13 {
// CHECK-NEXT:     q: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     not(feature = "alpha")
// CHECK-NEXT: ))]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld %ld %d %d %d %d %s %d %d\n".as_ptr(),
// CHECK-NEXT:             scale(2i32) as i64,
// CHECK-NEXT:             native_long(3i64),
// CHECK-NEXT:             unprototyped(4i32),
// CHECK-NEXT:             *(std::ptr::addr_of_mut!(table) as *mut i32).add(1),
// CHECK-NEXT:             *(std::ptr::addr_of_mut!(big_unsized) as *mut i32).add(2),
// CHECK-NEXT:             big_sized[3],
// CHECK-NEXT:             label(),
// CHECK-NEXT:             read_opaque(make_opaque()),
// CHECK-NEXT:             read_handle(std::ptr::null_mut::<__SlateRecord_signature_compatible_c_13>()),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { quit(0i32) };
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     feature = "alpha"
// CHECK-NEXT: ))]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld %ld %d %d %d %d %s %d %d\n".as_ptr(),
// CHECK-NEXT:             scale(2i64),
// CHECK-NEXT:             native_long(3i64),
// CHECK-NEXT:             unprototyped(4i32),
// CHECK-NEXT:             *(std::ptr::addr_of_mut!(table) as *mut i32).add(1),
// CHECK-NEXT:             *(std::ptr::addr_of_mut!(big_unsized) as *mut i32).add(2),
// CHECK-NEXT:             big_sized[3],
// CHECK-NEXT:             label(),
// CHECK-NEXT:             read_opaque(make_opaque()),
// CHECK-NEXT:             read_handle(std::ptr::null_mut::<__SlateRecord_signature_compatible_c_13>()),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { quit(0i32) };
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld %ld %d %d %d %d %s %d %d\n".as_ptr(),
// CHECK-NEXT:             scale(2i32),
// CHECK-NEXT:             native_long(3i32),
// CHECK-NEXT:             unprototyped(4i32),
// CHECK-NEXT:             *(std::ptr::addr_of_mut!(table) as *mut i32).add(1),
// CHECK-NEXT:             *(std::ptr::addr_of_mut!(big_unsized) as *mut i32).add(2),
// CHECK-NEXT:             big_sized[3],
// CHECK-NEXT:             label(),
// CHECK-NEXT:             read_opaque(make_opaque()),
// CHECK-NEXT:             read_handle(std::ptr::null_mut::<__SlateRecord_signature_compatible_c_13>()),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { quit(0i32) };
// CHECK-NEXT:     1
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
// CHECK-NEXT: file signature_compatible_helper.rs
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[ARG_0]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "beta"))]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct opaque {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "beta")]
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct opaque {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     extra: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_signature_compatible_helper_c_17 {
// CHECK-NEXT:     q: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
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
// CHECK-NEXT: type __SlateHoist_scale = i32;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     feature = "alpha"
// CHECK-NEXT: ))]
// CHECK-NEXT: type __SlateHoist_scale = i64;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn scale(x: __SlateHoist_scale) -> __SlateHoist_scale {
// CHECK-NEXT:     x * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn native_long(x: i64) -> i64 {
// CHECK-NEXT:     x + 8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn native_long(x: i32) -> i32 {
// CHECK-NEXT:     x + 4
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn unprototyped(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut table: [i32; 3] = [1, 2, 3];
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut big_unsized: __SlateAlign16<[i32; 32]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [i32; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = 1i32;
// CHECK-NEXT:     [[T_0]][1] = 2i32;
// CHECK-NEXT:     [[T_0]][2] = 3i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut big_sized: __SlateAlign16<[i32; 32]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0]]: [i32; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = 1i32;
// CHECK-NEXT:     [[T_0]][1] = 2i32;
// CHECK-NEXT:     [[T_0]][2] = 3i32;
// CHECK-NEXT:     [[T_0]][3] = 4i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn label() -> *const i8 {
// CHECK-NEXT:     c"ok".as_ptr()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(feature = "beta"))]
// CHECK-NEXT: static mut shared: opaque = opaque { value: 7i32 };
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(feature = "beta")]
// CHECK-NEXT: static mut shared: opaque = {
// CHECK-NEXT:     let mut [[T_0]]: opaque = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].value = 7i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn make_opaque() -> *mut opaque {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(shared) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn read_opaque(opaque: *mut opaque) -> i32 {
// CHECK-NEXT:     unsafe { (*opaque).value }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn read_handle(h: *mut __SlateRecord_signature_compatible_helper_c_17) -> i32 {
// CHECK-NEXT:     if !h.is_null() { unsafe { (*h).q } } else { 9 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn quit(code: i32) {
// CHECK-NEXT:     unsafe { std::process::exit(code) };
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
