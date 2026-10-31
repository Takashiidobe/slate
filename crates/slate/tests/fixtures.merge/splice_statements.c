#include <stdio.h>

struct state {
    int head;
#ifdef ALPHA
    int extra;
#endif
    int tail;
};

int added_statement(int value) {
    value *= 2;
#ifdef ALPHA
    value += 1;
#endif
    value *= 3;
    value += 7;
    value *= 5;
    return value;
}

int repeated_text(int value) {
#ifdef BETA
    value += 1;
#endif
    value *= 2;
    value += 1;
    value *= 2;
#ifdef ALPHA
    value += 1;
#endif
    return value;
}

int nested_in_loop(int count) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += i;
#ifdef ALPHA
        if (i == 2) {
            total += 100;
        }
#endif
#ifdef _WIN32
        total += 1000;
#endif
        total *= 2;
    }
    return total;
}

int gated_local(int value) {
    value += 3;
#ifdef BETA
    int doubled = value * 2;
    value = doubled + 1;
#endif
    value -= 1;
    return value;
}

int different_tail(int value) {
    value += 1;
    value *= 3;
#ifdef ALPHA
    return value + 2;
#else
    return value - 2;
#endif
}

int shifted_names(int value, int limit) {
    int result = 0;
#ifdef ALPHA
    for (int i = 0; i < limit; i++) {
        if (i == value) {
            goto found;
        }
    }
    result += 50;
#endif
    for (int i = 0; i < limit; i++) {
        if (i * 2 == value) {
            goto found;
        }
        result += i;
    }
    result += 7;
    return result;
found:
    result += 1000;
    return result;
}

int switched(int mode) {
    int out = 1;
    switch (mode) {
    case 0:
        out += 1;
        break;
#ifdef BETA
    case 1:
        out += 2;
        break;
#endif
    default:
        out += 3;
        break;
    }
    out *= 2;
    return out;
}

int gamma_value(void) {
    int value = 1;
#if GAMMA + 0 == 3
    value += 3;
#endif
    value += 1;
    return value;
}

int field_use(void) {
    struct state s;
    s.head = 1;
    s.tail = 2;
#ifdef ALPHA
    s.extra = 4;
    s.tail += s.extra;
#endif
    return s.head + s.tail + (int)sizeof(s);
}

int main(void) {
    printf("%d %d %d\n", added_statement(1), repeated_text(1), nested_in_loop(4));
    printf("%d %d\n", gated_local(1), different_tail(1));
    printf("%d %d\n", shifted_names(3, 5), shifted_names(99, 5));
    printf("%d %d %d\n", switched(0), switched(1), switched(2));
    printf("%d %d\n", gamma_value(), field_use());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 6 compiles
// CHECK-NEXT:   splice_statements.c: [alpha,beta,gamma] 6 compiled
// CHECK-NEXT: target windows-x86_64: 6 compiles
// CHECK-NEXT:   splice_statements.c: [alpha,beta,gamma] 6 compiled
// CHECK-NEXT: total: 12 compiles across 2 targets
// CHECK-NEXT: unit splice_statements: 24 items, 12 gated, 0 duplicated functions
// CHECK-NEXT:   spliced record:state: 2 variants
// CHECK-NEXT:   spliced fn:added_statement: 2 variants
// CHECK-NEXT:   spliced fn:repeated_text: 4 variants
// CHECK-NEXT:   spliced fn:nested_in_loop: 4 variants
// CHECK-NEXT:   spliced fn:gated_local: 2 variants
// CHECK-NEXT:   spliced fn:different_tail: 2 variants
// CHECK-NEXT:   spliced fn:shifted_names: 2 variants
// CHECK-NEXT:   spliced fn:switched: 2 variants
// CHECK-NEXT:   spliced fn:gamma_value: 2 variants
// CHECK-NEXT:   spliced fn:field_use: 2 variants
// CHECK-NEXT:   spliced fn:__slate_main: 2 variants
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct state {
// CHECK-NEXT:     head: i32,
// CHECK-NEXT:     #[cfg(feature = "alpha")]
// CHECK-NEXT:     extra: i32,
// CHECK-NEXT:     tail: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn added_statement(mut value: i32) -> i32 {
// CHECK-NEXT:     value *= 2;
// CHECK-NEXT:     #[cfg(feature = "alpha")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value *= 3;
// CHECK-NEXT:     value += 7;
// CHECK-NEXT:     value *= 5;
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn repeated_text(mut value: i32) -> i32 {
// CHECK-NEXT:     #[cfg(not(feature = "beta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value *= 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     value += 1;
// CHECK-NEXT:     value *= 2;
// CHECK-NEXT:     #[cfg(any(feature = "alpha", feature = "beta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(feature = "beta")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value *= 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(feature = "alpha", feature = "beta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn nested_in_loop(count: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         total += i;
// CHECK-NEXT:         #[cfg(feature = "alpha")]
// CHECK-NEXT:         {
// CHECK-NEXT:             if i == 2 {
// CHECK-NEXT:                 total += 100;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT:         {
// CHECK-NEXT:             total += 1000;
// CHECK-NEXT:         }
// CHECK-NEXT:         total *= 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn gated_local(mut value: i32) -> i32 {
// CHECK-NEXT:     value += 3;
// CHECK-NEXT:     #[cfg(feature = "beta")]
// CHECK-NEXT:     let doubled: i32 = value * 2;
// CHECK-NEXT:     #[cfg(feature = "beta")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value = doubled + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     value -= 1;
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn different_tail(mut value: i32) -> i32 {
// CHECK-NEXT:     value += 1;
// CHECK-NEXT:     value *= 3;
// CHECK-NEXT:     #[cfg(not(feature = "alpha"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         value - 2
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(feature = "alpha")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value + 2
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn shifted_names(value: i32, limit: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     #[cfg(feature = "alpha")]
// CHECK-NEXT:     let mut [[I_0:i_[0-9]+]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut result: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         result = 0;
// CHECK-NEXT:         #[cfg(not(feature = "alpha"))]
// CHECK-NEXT:         {
// CHECK-NEXT:             i = 0;
// CHECK-NEXT:             while i < limit {
// CHECK-NEXT:                 if i * 2 == value {
// CHECK-NEXT:                     result += 1000;
// CHECK-NEXT:                     return result;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     result += i;
// CHECK-NEXT:                     i += 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             result += 7;
// CHECK-NEXT:         }
// CHECK-NEXT:         #[cfg(feature = "alpha")]
// CHECK-NEXT:         {
// CHECK-NEXT:             [[I_0]] = 0;
// CHECK-NEXT:             '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:                 while [[I_0]] < limit {
// CHECK-NEXT:                     if [[I_0]] == value {
// CHECK-NEXT:                         break '[[JOIN_0]];
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         [[I_0]] += 1;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 result += 50;
// CHECK-NEXT:                 i = 0;
// CHECK-NEXT:                 while i < limit {
// CHECK-NEXT:                     if i * 2 == value {
// CHECK-NEXT:                         break '[[JOIN_0]];
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         result += i;
// CHECK-NEXT:                         i += 1;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 result += 7;
// CHECK-NEXT:                 return result;
// CHECK-NEXT:             }
// CHECK-NEXT:             result += 1000;
// CHECK-NEXT:         }
// CHECK-NEXT:         return result;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn switched(mode: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 1;
// CHECK-NEXT:     #[cfg(not(feature = "beta"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         match mode {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 out += 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 out += 3;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(feature = "beta")]
// CHECK-NEXT:     {
// CHECK-NEXT:         match mode {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 out += 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 out += 2;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 out += 3;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out *= 2;
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn gamma_value() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 1;
// CHECK-NEXT:     #[cfg(feature = "gamma")]
// CHECK-NEXT:     {
// CHECK-NEXT:         value += 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     value += 1;
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C-unwind" fn field_use() -> i32 {
// CHECK-NEXT:     let mut s: state = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s.head = 1;
// CHECK-NEXT:     s.tail = 2;
// CHECK-NEXT:     #[cfg(not(feature = "alpha"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         s.head + s.tail + 8
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(feature = "alpha")]
// CHECK-NEXT:     {
// CHECK-NEXT:         s.extra = 4;
// CHECK-NEXT:         s.tail += s.extra;
// CHECK-NEXT:         { s.head + s.tail + 12 }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {}",
// CHECK-NEXT:             added_statement(1i32),
// CHECK-NEXT:             repeated_text(1i32),
// CHECK-NEXT:             nested_in_loop(4i32)
// CHECK-NEXT:         );
// CHECK-NEXT:         println!("{} {}", gated_local(1i32), different_tail(1i32));
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}",
// CHECK-NEXT:             shifted_names(3i32, 5i32),
// CHECK-NEXT:             shifted_names(99i32, 5i32)
// CHECK-NEXT:         );
// CHECK-NEXT:         println!("{} {} {}", switched(0i32), switched(1i32), switched(2i32));
// CHECK-NEXT:         println!("{} {}", gamma_value(), field_use());
// CHECK-NEXT:     }
// CHECK-NEXT:     #[cfg(all(target_arch = "x86_64", windows, target_env = "msvc"))]
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 added_statement(1i32),
// CHECK-NEXT:                 repeated_text(1i32),
// CHECK-NEXT:                 nested_in_loop(4i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { printf(c"%d %d\n".as_ptr(), gated_local(1i32), different_tail(1i32)) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %d\n".as_ptr(),
// CHECK-NEXT:                 shifted_names(3i32, 5i32),
// CHECK-NEXT:                 shifted_names(99i32, 5i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:                 switched(0i32),
// CHECK-NEXT:                 switched(1i32),
// CHECK-NEXT:                 switched(2i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { printf(c"%d %d\n".as_ptr(), gamma_value(), field_use()) };
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
