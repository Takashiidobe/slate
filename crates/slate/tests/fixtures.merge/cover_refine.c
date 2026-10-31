// merge-features: alpha beta gamma delta epsilon
#include <stdio.h>

struct state {
    int head;
#ifdef ALPHA
    long a;
#endif
#ifdef BETA
    long b;
#endif
};

int state_size(void) { return (int)sizeof(struct state); }

int nested(void) {
    int value = 0;
#ifdef GAMMA
    value += 1;
#ifdef DELTA
    value += 2;
#endif
#ifdef EPSILON
    value += 4;
#endif
#endif
    return value;
}

int joint(void) {
    int value = 0;
#if defined(ALPHA) && defined(GAMMA) && defined(EPSILON)
    value += 8;
#endif
#if defined(BETA) || (defined(DELTA) && defined(EPSILON))
    value += 16;
#endif
    return value;
}

#if defined(ALPHA) && defined(DELTA)
int shape(int x) { return x + 1; }
#elif defined(BETA)
int shape(int x) {
    for (int i = 0; i < 2; i++) x += i;
    return x;
}
#else
int shape(int x) {
    if (x > 0) return x * 3;
    return x;
}
#endif

int main(void) {
    printf("%d %d %d %d\n", state_size() > 8, nested(), joint(), shape(2));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 21 compiles
// CHECK-NEXT:   cover_refine.c: [alpha,beta,gamma,delta,epsilon] 27 compiled
// CHECK-NEXT:     6 assignments preprocess like a compiled one
// CHECK-NEXT: target windows-x86_64: 21 compiles
// CHECK-NEXT:   cover_refine.c: [alpha,beta,gamma,delta,epsilon] 27 compiled
// CHECK-NEXT:     6 assignments preprocess like a compiled one
// CHECK-NEXT: total: 42 compiles across 2 targets
// CHECK-NEXT: unit cover_refine: 24 items, 17 gated, 0 duplicated functions
// CHECK-NEXT:   hoisted fn:state_size: 5 variants
// CHECK-NEXT:   spliced fn:nested: 5 variants
// CHECK-NEXT:   spliced fn:joint: 4 variants
// CHECK-NEXT:   spliced fn:shape: 3 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
// CHECK-NEXT: projection: 64 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! merge-features: alpha beta gamma delta epsilon
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
// CHECK-NEXT: #[cfg(all(not(feature = "alpha"), not(feature = "beta")))]
// CHECK-NEXT: const __SLATE_HOIST_state_size: i32 = 4;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     any(
// CHECK-NEXT:         all(feature = "alpha", not(feature = "beta")),
// CHECK-NEXT:         all(not(feature = "alpha"), feature = "beta")
// CHECK-NEXT:     )
// CHECK-NEXT: ))]
// CHECK-NEXT: const __SLATE_HOIST_state_size: i32 = 16;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     target_os = "linux",
// CHECK-NEXT:     target_env = "gnu",
// CHECK-NEXT:     feature = "alpha",
// CHECK-NEXT:     feature = "beta"
// CHECK-NEXT: ))]
// CHECK-NEXT: const __SLATE_HOIST_state_size: i32 = 24;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     windows,
// CHECK-NEXT:     target_env = "msvc",
// CHECK-NEXT:     any(
// CHECK-NEXT:         all(feature = "alpha", not(feature = "beta")),
// CHECK-NEXT:         all(not(feature = "alpha"), feature = "beta")
// CHECK-NEXT:     )
// CHECK-NEXT: ))]
// CHECK-NEXT: const __SLATE_HOIST_state_size: i32 = 8;
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(all(
// CHECK-NEXT:     target_arch = "x86_64",
// CHECK-NEXT:     windows,
// CHECK-NEXT:     target_env = "msvc",
// CHECK-NEXT:     feature = "alpha",
// CHECK-NEXT:     feature = "beta"
// CHECK-NEXT: ))]
// CHECK-NEXT: const __SLATE_HOIST_state_size: i32 = 12;
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn state_size() -> i32 {
// CHECK-NEXT:     __SLATE_HOIST_state_size
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn nested() -> i32 {
// CHECK-NEXT:     #[cfg(feature = "gamma")]
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     #[cfg(feature = "gamma")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(feature = "gamma", feature = "delta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(feature = "gamma", feature = "epsilon"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(not(feature = "gamma"))]
// CHECK-NEXT:     let value: i32 = 0;
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn joint() -> i32 {
// CHECK-NEXT:     #[cfg(any(
// CHECK-NEXT:         feature = "beta",
// CHECK-NEXT:         all(feature = "delta", feature = "epsilon"),
// CHECK-NEXT:         all(feature = "alpha", feature = "gamma", feature = "epsilon")
// CHECK-NEXT:     ))]
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     #[cfg(all(feature = "alpha", feature = "gamma", feature = "epsilon"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 8;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(feature = "beta", all(feature = "delta", feature = "epsilon")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 16;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(
// CHECK-NEXT:         all(not(feature = "beta"), not(feature = "epsilon")),
// CHECK-NEXT:         all(not(feature = "alpha"), not(feature = "beta"), not(feature = "delta")),
// CHECK-NEXT:         all(not(feature = "beta"), not(feature = "gamma"), not(feature = "delta"))
// CHECK-NEXT:     ))]
// CHECK-NEXT:     let value: i32 = 0;
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn shape(mut x: i32) -> i32 {
// CHECK-NEXT:     #[cfg(any(
// CHECK-NEXT:         all(not(feature = "alpha"), not(feature = "beta")),
// CHECK-NEXT:         all(not(feature = "beta"), not(feature = "delta"))
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         if x > 0 {
// CHECK-NEXT:             return x * 3;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(
// CHECK-NEXT:         all(not(feature = "alpha"), feature = "beta"),
// CHECK-NEXT:         all(feature = "beta", not(feature = "delta"))
// CHECK-NEXT:     ))]
// CHECK-NEXT:     {
// CHECK-NEXT:         for i in 0i32..2i32 {
// CHECK-NEXT:             x += i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(any(not(feature = "alpha"), not(feature = "delta")))]
// CHECK-NEXT:     {
// CHECK-NEXT:         x
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(feature = "alpha", feature = "delta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         x + 1
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {}",
// CHECK-NEXT:             (state_size() > 8) as i32,
// CHECK-NEXT:             nested(),
// CHECK-NEXT:             joint(),
// CHECK-NEXT:             shape(2i32)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:                 (state_size() > 8) as i32,
// CHECK-NEXT:                 nested(),
// CHECK-NEXT:                 joint(),
// CHECK-NEXT:                 shape(2i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
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
