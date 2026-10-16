/* This is a modfied version of va-arg-2.c to test passing a va_list as
   a parameter to another function.  */

#include <stdarg.h>

extern void          abort(void);
extern void          exit(int);
extern __SIZE_TYPE__ strlen(const char *);

int to_hex(unsigned int a) {
  static char hex[] = "0123456789abcdef";

  if (a > 15)
    abort();
  return hex[a];
}

void fap(int i, char *format, va_list ap) {
  if (strlen(format) != 16 - i)
    abort();
  while (*format)
    if (*format++ != to_hex(va_arg(ap, int)))
      abort();
}

void f0(char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(0, format, ap);
  va_end(ap);
}

void f1(int a1, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(1, format, ap);
  va_end(ap);
}

void f2(int a1, int a2, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(2, format, ap);
  va_end(ap);
}

void f3(int a1, int a2, int a3, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(3, format, ap);
  va_end(ap);
}

void f4(int a1, int a2, int a3, int a4, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(4, format, ap);
  va_end(ap);
}

void f5(int a1, int a2, int a3, int a4, int a5, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(5, format, ap);
  va_end(ap);
}

void f6(int a1, int a2, int a3, int a4, int a5, int a6, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(6, format, ap);
  va_end(ap);
}

void f7(int a1, int a2, int a3, int a4, int a5, int a6, int a7, char *format,
        ...) {
  va_list ap;

  va_start(ap, format);
  fap(7, format, ap);
  va_end(ap);
}

void f8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
        char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(8, format, ap);
  va_end(ap);
}

void f9(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
        char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(9, format, ap);
  va_end(ap);
}

void f10(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(10, format, ap);
  va_end(ap);
}

void f11(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(11, format, ap);
  va_end(ap);
}

void f12(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(12, format, ap);
  va_end(ap);
}

void f13(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, int a13, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(13, format, ap);
  va_end(ap);
}

void f14(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, int a13, int a14, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(14, format, ap);
  va_end(ap);
}

void f15(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, int a13, int a14, int a15, char *format,
         ...) {
  va_list ap;

  va_start(ap, format);
  fap(15, format, ap);
  va_end(ap);
}

int main(void) {
  char *f = "0123456789abcdef";

  f0(f + 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f1(0, f + 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f2(0, 1, f + 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f3(0, 1, 2, f + 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f4(0, 1, 2, 3, f + 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f5(0, 1, 2, 3, 4, f + 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f6(0, 1, 2, 3, 4, 5, f + 6, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f7(0, 1, 2, 3, 4, 5, 6, f + 7, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f8(0, 1, 2, 3, 4, 5, 6, 7, f + 8, 8, 9, 10, 11, 12, 13, 14, 15);
  f9(0, 1, 2, 3, 4, 5, 6, 7, 8, f + 9, 9, 10, 11, 12, 13, 14, 15);
  f10(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, f + 10, 10, 11, 12, 13, 14, 15);
  f11(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, f + 11, 11, 12, 13, 14, 15);
  f12(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, f + 12, 12, 13, 14, 15);
  f13(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, f + 13, 13, 14, 15);
  f14(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, f + 14, 14, 15);
  f15(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, f + 15, 15);

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This is a modfied version of va-arg-2.c to test passing a va_list as
// CHECK-NEXT: //! a parameter to another function.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn strlen([[V_1:__v[0-9]+]]: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn to_hex(mut a: u32) -> i32 {
// CHECK-NEXT:     if a > ((15 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { *unsafe { unsafe { std::ptr::addr_of_mut!(hex.0) as *mut i8 }.offset(a as isize) } })
// CHECK-NEXT:         as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut hex: __SlateAlign16<[i8; 17]> = __SlateAlign16([
// CHECK-NEXT:     48 as i8, 49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 54 as i8, 55 as i8, 56 as i8,
// CHECK-NEXT:     57 as i8, 97 as i8, 98 as i8, 99 as i8, 100 as i8, 101 as i8, 102 as i8, 0 as i8,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn fap(mut i: i32, mut format: *mut i8, mut ap: core::ffi::VaList<'_>) {
// CHECK-NEXT:     if (unsafe { strlen(format as *const i8) }) != ((((16 as i32) - i) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { *format }) != (0 as i8) {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut i8 = format;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut i8 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:         format = [[V_3]];
// CHECK-NEXT:         if ((unsafe { *[[V_2]] }) as i32) != to_hex((unsafe { ap.next_arg::<i32>() }) as u32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f0(mut format: *mut i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(0 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f1(mut a1: i32, mut format: *mut i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(1 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f2(mut a1: i32, mut a2: i32, mut format: *mut i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(2 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f3(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(3 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f4(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(4 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f5(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(5 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f6(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(6 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f7(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut a7: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(7 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f8(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut a7: i32,
// CHECK-NEXT:     mut a8: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(8 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f9(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut a7: i32,
// CHECK-NEXT:     mut a8: i32,
// CHECK-NEXT:     mut a9: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(9 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f10(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut a7: i32,
// CHECK-NEXT:     mut a8: i32,
// CHECK-NEXT:     mut a9: i32,
// CHECK-NEXT:     mut a10: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(10 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f11(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut a7: i32,
// CHECK-NEXT:     mut a8: i32,
// CHECK-NEXT:     mut a9: i32,
// CHECK-NEXT:     mut a10: i32,
// CHECK-NEXT:     mut a11: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(11 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f12(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut a7: i32,
// CHECK-NEXT:     mut a8: i32,
// CHECK-NEXT:     mut a9: i32,
// CHECK-NEXT:     mut a10: i32,
// CHECK-NEXT:     mut a11: i32,
// CHECK-NEXT:     mut a12: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(12 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f13(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut a7: i32,
// CHECK-NEXT:     mut a8: i32,
// CHECK-NEXT:     mut a9: i32,
// CHECK-NEXT:     mut a10: i32,
// CHECK-NEXT:     mut a11: i32,
// CHECK-NEXT:     mut a12: i32,
// CHECK-NEXT:     mut a13: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(13 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f14(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut a7: i32,
// CHECK-NEXT:     mut a8: i32,
// CHECK-NEXT:     mut a9: i32,
// CHECK-NEXT:     mut a10: i32,
// CHECK-NEXT:     mut a11: i32,
// CHECK-NEXT:     mut a12: i32,
// CHECK-NEXT:     mut a13: i32,
// CHECK-NEXT:     mut a14: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(14 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn f15(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut a5: i32,
// CHECK-NEXT:     mut a6: i32,
// CHECK-NEXT:     mut a7: i32,
// CHECK-NEXT:     mut a8: i32,
// CHECK-NEXT:     mut a9: i32,
// CHECK-NEXT:     mut a10: i32,
// CHECK-NEXT:     mut a11: i32,
// CHECK-NEXT:     mut a12: i32,
// CHECK-NEXT:     mut a13: i32,
// CHECK-NEXT:     mut a14: i32,
// CHECK-NEXT:     mut a15: i32,
// CHECK-NEXT:     mut format: *mut i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     fap(15 as i32, format, ap.clone());
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: *mut i8 = c"0123456789abcdef".as_ptr() as *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f0(
// CHECK-NEXT:             unsafe { f.offset((0 as i32) as isize) },
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f1(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             unsafe { f.offset((1 as i32) as isize) },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f2(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             unsafe { f.offset((2 as i32) as isize) },
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f3(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             unsafe { f.offset((3 as i32) as isize) },
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f4(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             unsafe { f.offset((4 as i32) as isize) },
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f5(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             unsafe { f.offset((5 as i32) as isize) },
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f6(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             unsafe { f.offset((6 as i32) as isize) },
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f7(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             unsafe { f.offset((7 as i32) as isize) },
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f8(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             unsafe { f.offset((8 as i32) as isize) },
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f9(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             unsafe { f.offset((9 as i32) as isize) },
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f10(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             unsafe { f.offset((10 as i32) as isize) },
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f11(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             unsafe { f.offset((11 as i32) as isize) },
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f12(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             unsafe { f.offset((12 as i32) as isize) },
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f13(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             unsafe { f.offset((13 as i32) as isize) },
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f14(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             unsafe { f.offset((14 as i32) as isize) },
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f15(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2 as i32,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4 as i32,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6 as i32,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8 as i32,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10 as i32,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12 as i32,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14 as i32,
// CHECK-NEXT:             unsafe { f.offset((15 as i32) as isize) },
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
