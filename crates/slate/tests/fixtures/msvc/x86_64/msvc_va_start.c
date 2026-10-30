/* { dg-options "/Zc:preprocessor" } */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct big {
  long long a, b, c;
};

static long long sum(int n, ...) {
  va_list ap;
  va_start(ap, n);
  long long total = 0;
  for (int i = 0; i < n; i++)
    total += va_arg(ap, int);
  va_end(ap);
  return total;
}

static double mixed(int n, ...) {
  va_list ap, copy;
  va_start(ap, n);
  va_copy(copy, ap);
  double total = 0;
  for (int i = 0; i < n; i++) {
    total += va_arg(ap, double);
    struct big b = va_arg(ap, struct big);
    total += (double)(b.a + b.b + b.c);
  }
  total += va_arg(copy, double);
  va_end(copy);
  va_end(ap);
  return total;
}

static int forward(char *buffer, size_t size, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int written = vsnprintf(buffer, size, format, ap);
  va_end(ap);
  return written;
}

int main(void) {
  struct big one = {1, 2, 3};
  struct big two = {10, 20, 30};
  if (sum(4, 1, 2, 3, 4) != 10)
    abort();
  if (mixed(2, 0.5, one, 1.25, two) != 0.5 + 6 + 1.25 + 60 + 0.5)
    abort();
  char buffer[64];
  if (forward(buffer, sizeof buffer, "%d-%s-%lld", 7, "x", 9000000000LL) != 14)
    abort();
  printf("%s %d %.3f %c %lld\n", buffer, -5, 2.5, 'q', -9000000000LL);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor" }
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
// CHECK-NEXT:     fn __stdio_common_vsprintf(
// CHECK-NEXT:         [[ARG_0]]: u64,
// CHECK-NEXT:         [[ARG_1]]: *mut i8,
// CHECK-NEXT:         [[ARG_2]]: u64,
// CHECK-NEXT:         [[ARG_3]]: *const i8,
// CHECK-NEXT:         [[ARG_4]]: *mut __crt_locale_pointers,
// CHECK-NEXT:         [[ARG_5:arg[0-9]+]]: *mut i8,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_locale_pointers {
// CHECK-NEXT:     locinfo: *mut __crt_locale_data,
// CHECK-NEXT:     mbcinfo: *mut __crt_multibyte_data,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_locale_data {}
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __crt_multibyte_data {}
// CHECK-EMPTY:
// CHECK-NEXT: fn __local_stdio_printf_options() -> *mut u64 {
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(_OptionsStorage) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut _OptionsStorage: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _iobuf {
// CHECK-NEXT:     _Placeholder: *mut (),
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-NEXT: fn vsnprintf(_Buffer: *mut i8, _BufferCount: u64, _Format: *const i8, _ArgList: *mut i8) -> i32 {
// CHECK-NEXT:     let _Result: i32 = unsafe {
// CHECK-NEXT:         __stdio_common_vsprintf(
// CHECK-NEXT:             *__local_stdio_printf_options() | 2u64,
// CHECK-NEXT:             _Buffer,
// CHECK-NEXT:             _BufferCount,
// CHECK-NEXT:             _Format,
// CHECK-NEXT:             std::ptr::null_mut::<__crt_locale_pointers>(),
// CHECK-NEXT:             _ArgList,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if _Result < 0 { -1 } else { _Result }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct big {
// CHECK-NEXT:     a: i64,
// CHECK-NEXT:     b: i64,
// CHECK-NEXT:     c: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn sum(n: i32, mut __va_args: ...) -> i64 {
// CHECK-NEXT:     let mut ap: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *std::ptr::addr_of_mut!(ap) =
// CHECK-NEXT:             core::mem::transmute::<core::ffi::VaList<'_>, *mut i8>(__va_args.clone());
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = if false || 0u64 != 0 {
// CHECK-NEXT:             let [[V_1:__v[0-9]+]]: *mut i8 = ap;
// CHECK-NEXT:             let [[V_2:__v[0-9]+]]: *mut i8 = unsafe { [[V_1]].add(8) };
// CHECK-NEXT:             ap = [[V_2]];
// CHECK-NEXT:             unsafe { *(*([[V_2]].sub(8) as *mut *mut i32)) }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let [[V_3:__v[0-9]+]]: *mut i8 = ap;
// CHECK-NEXT:             let [[V_4:__v[0-9]+]]: *mut i8 = unsafe { [[V_3]].add(8) };
// CHECK-NEXT:             ap = [[V_4]];
// CHECK-NEXT:             unsafe { *([[V_4]].sub(8) as *mut i32) }
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: i32 = [[V_0]];
// CHECK-NEXT:         total += [[V_5]] as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     ap = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn mixed(n: i32, mut __va_args: ...) -> f64 {
// CHECK-NEXT:     let mut ap: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut copy: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *std::ptr::addr_of_mut!(ap) =
// CHECK-NEXT:             core::mem::transmute::<core::ffi::VaList<'_>, *mut i8>(__va_args.clone());
// CHECK-NEXT:     }
// CHECK-NEXT:     copy = ap;
// CHECK-NEXT:     let mut total: f64 = 0i32 as f64;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let [[V_0]]: f64 = if false || 0u64 != 0 {
// CHECK-NEXT:             let [[V_1]]: *mut i8 = ap;
// CHECK-NEXT:             let [[V_2]]: *mut i8 = unsafe { [[V_1]].add(8) };
// CHECK-NEXT:             ap = [[V_2]];
// CHECK-NEXT:             unsafe { *(*([[V_2]].sub(8) as *mut *mut f64)) }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let [[V_3]]: *mut i8 = ap;
// CHECK-NEXT:             let [[V_4]]: *mut i8 = unsafe { [[V_3]].add(8) };
// CHECK-NEXT:             ap = [[V_4]];
// CHECK-NEXT:             unsafe { *([[V_4]].sub(8) as *mut f64) }
// CHECK-NEXT:         };
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:         let [[V_6:__v[0-9]+]]: big = if true || 16u64 != 0 {
// CHECK-NEXT:             let [[V_7:__v[0-9]+]]: *mut i8 = ap;
// CHECK-NEXT:             let [[V_8:__v[0-9]+]]: *mut i8 = unsafe { [[V_7]].add(8) };
// CHECK-NEXT:             ap = [[V_8]];
// CHECK-NEXT:             unsafe { *(*([[V_8]].sub(8) as *mut *mut big)) }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let [[V_9:__v[0-9]+]]: *mut i8 = ap;
// CHECK-NEXT:             let [[V_10:__v[0-9]+]]: *mut i8 = unsafe { [[V_9]].add(8) };
// CHECK-NEXT:             ap = [[V_10]];
// CHECK-NEXT:             unsafe { *([[V_10]].sub(8) as *mut big) }
// CHECK-NEXT:         };
// CHECK-NEXT:         let b: big = [[V_6]];
// CHECK-NEXT:         total += (b.a + b.b + b.c) as f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: f64 = if false || 0u64 != 0 {
// CHECK-NEXT:         let [[V_12:__v[0-9]+]]: *mut i8 = copy;
// CHECK-NEXT:         let [[V_13:__v[0-9]+]]: *mut i8 = unsafe { [[V_12]].add(8) };
// CHECK-NEXT:         copy = [[V_13]];
// CHECK-NEXT:         unsafe { *(*([[V_13]].sub(8) as *mut *mut f64)) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let [[V_14:__v[0-9]+]]: *mut i8 = copy;
// CHECK-NEXT:         let [[V_15:__v[0-9]+]]: *mut i8 = unsafe { [[V_14]].add(8) };
// CHECK-NEXT:         copy = [[V_15]];
// CHECK-NEXT:         unsafe { *([[V_15]].sub(8) as *mut f64) }
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_16:__v[0-9]+]]: f64 = [[V_11]];
// CHECK-NEXT:     total += [[V_16]];
// CHECK-NEXT:     copy = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     ap = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn forward(
// CHECK-NEXT:     buffer: *mut i8,
// CHECK-NEXT:     size: u64,
// CHECK-NEXT:     format: *const i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let mut ap: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *std::ptr::addr_of_mut!(ap) =
// CHECK-NEXT:             core::mem::transmute::<core::ffi::VaList<'_>, *mut i8>(__va_args.clone());
// CHECK-NEXT:     }
// CHECK-NEXT:     let written: i32 = vsnprintf(buffer, size, format, ap);
// CHECK-NEXT:     ap = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     written
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let one: big = big { a: 1, b: 2, c: 3 };
// CHECK-NEXT:     let two: big = big {
// CHECK-NEXT:         a: 10,
// CHECK-NEXT:         b: 20,
// CHECK-NEXT:         c: 30,
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe { sum(4i32, 1i32, 2i32, 3i32, 4i32) }) != 10 {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { mixed(2i32, 0.5f64, one, 1.25f64, two) })
// CHECK-NEXT:         != 0.5f64 + (6i32 as f64) + 1.25f64 + (60i32 as f64) + 0.5f64
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         forward(
// CHECK-NEXT:             buffer.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             64u64,
// CHECK-NEXT:             c"%d-%s-%lld".as_ptr(),
// CHECK-NEXT:             7i32,
// CHECK-NEXT:             c"x".as_ptr() as *mut i8,
// CHECK-NEXT:             9000000000i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != 14
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %d %.3f %c %lld\n".as_ptr(),
// CHECK-NEXT:             buffer.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             -5i32,
// CHECK-NEXT:             2.5f64,
// CHECK-NEXT:             113i32,
// CHECK-NEXT:             -9000000000i64,
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
