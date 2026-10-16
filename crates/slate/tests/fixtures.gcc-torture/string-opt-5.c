/* Copyright (C) 2000  Free Software Foundation.

   Ensure builtin strlen, strcmp, strchr, strrchr and strncpy
   perform correctly.

   Written by Jakub Jelinek, 11/7/2000.  */

extern void          abort(void);
extern __SIZE_TYPE__ strlen(const char *);
extern int           strcmp(const char *, const char *);
extern char         *strchr(const char *, int);
extern char         *strrchr(const char *, int);
extern char         *strncpy(char *, const char *, __SIZE_TYPE__);
extern void         *memset(void *, int, __SIZE_TYPE__);
extern int           memcmp(const void *, const void *, __SIZE_TYPE__);

int   x   = 6;
int   y   = 1;
char *bar = "hi world";
char  buf[64];

int main() {
  const char *const foo = "hello world";
  char              dst[64];

  if (strlen(bar) != 8)
    abort();
  if (strlen(bar + (++x & 2)) != 6)
    abort();
  if (x != 7)
    abort();
  if (strlen(foo + (x++, 6)) != 5)
    abort();
  if (x != 8)
    abort();
  if (strlen(foo + (++x & 1)) != 10)
    abort();
  if (x != 9)
    abort();
  if (strcmp(foo + (x -= 6), "lo world"))
    abort();
  if (x != 3)
    abort();
  if (strcmp(foo, bar) >= 0)
    abort();
  if (strcmp(foo, bar + (x++ & 1)) >= 0)
    abort();
  if (x != 4)
    abort();
  if (strchr(foo + (x++ & 7), 'l') != foo + 9)
    abort();
  if (x != 5)
    abort();
  if (strchr(bar, 'o') != bar + 4)
    abort();
  if (strchr(bar, '\0') != bar + 8)
    abort();
  if (strrchr(bar, 'x'))
    abort();
  if (strrchr(bar, 'o') != bar + 4)
    abort();
  if (strcmp(foo + (x++ & 1), "ello world" + (--y & 1)))
    abort();
  if (x != 6 || y != 0)
    abort();
  dst[5] = ' ';
  dst[6] = '\0';
  x      = 5;
  y      = 1;
  if (strncpy(dst + 1, foo + (x++ & 3), 4) != dst + 1 || x != 6 ||
      strcmp(dst + 1, "ello "))
    abort();
  memset(dst, ' ', sizeof dst);
  if (strncpy(dst + (++x & 1), (y++ & 3) + "foo", 10) != dst + 1 || x != 7 ||
      y != 2 || memcmp(dst, " oo\0\0\0\0\0\0\0\0 ", 12))
    abort();
  memset(dst, ' ', sizeof dst);
  if (strncpy(dst, "hello", 8) != dst || memcmp(dst, "hello\0\0\0 ", 9))
    abort();
  x = '!';
  memset(buf, ' ', sizeof buf);
  if (memset(buf, x++, ++y) != buf || x != '!' + 1 || y != 3 ||
      memcmp(buf, "!!!", 3))
    abort();
  if (memset(buf + y++, '-', 8) != buf + 3 || y != 4 ||
      memcmp(buf, "!!!--------", 11))
    abort();
  x = 10;
  if (memset(buf + ++x, 0, y++) != buf + 11 || x != 11 || y != 5 ||
      memcmp(buf + 8, "---\0\0\0", 7))
    abort();
  if (memset(buf + (x += 4), 0, 6) != buf + 15 || x != 15 ||
      memcmp(buf + 10, "-\0\0\0\0\0\0\0\0\0", 11))
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2000  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Ensure builtin strlen, strcmp, strchr, strrchr and strncpy
// CHECK-NEXT: //! perform correctly.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Jakub Jelinek, 11/7/2000.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn strlen([[V_0:__v[0-9]+]]: *const i8) -> u64;
// CHECK-NEXT:     fn strcmp([[V_1:__v[0-9]+]]: *const i8, [[V_2:__v[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     fn strchr([[V_3:__v[0-9]+]]: *const i8, [[V_4:__v[0-9]+]]: i32) -> *mut i8;
// CHECK-NEXT:     fn strrchr([[V_5:__v[0-9]+]]: *const i8, [[V_6:__v[0-9]+]]: i32) -> *mut i8;
// CHECK-NEXT:     fn strncpy([[V_7:__v[0-9]+]]: *mut i8, [[V_8:__v[0-9]+]]: *const i8, [[V_9:__v[0-9]+]]: u64) -> *mut i8;
// CHECK-NEXT:     fn memset([[V_10:__v[0-9]+]]: *mut (), [[V_11:__v[0-9]+]]: i32, [[V_12:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn memcmp([[V_13:__v[0-9]+]]: *const (), [[V_14:__v[0-9]+]]: *const (), [[V_15:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i32 = 6 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut y: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut bar: *mut i8 = b"hi world\0".as_ptr() as *mut i8;
// CHECK-EMPTY:
// CHECK-NEXT: static mut buf: __SlateAlign16<[i8; 64]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut foo: *const i8 = c"hello world".as_ptr() as *const i8;
// CHECK-NEXT:     let mut dst: __SlateAlign16<[i8; 64]> = __SlateAlign16([0 as i8; 64]);
// CHECK-NEXT:     if (unsafe { strlen((unsafe { bar }) as *const i8) }) != (((8 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: i32 = [[V_16]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_17]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strlen((unsafe { unsafe { bar }.offset(([[V_17]] & (2 as i32)) as isize) }) as *const i8)
// CHECK-NEXT:     }) != (((6 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (7 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_18:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: i32 = [[V_18]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_19]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { strlen(unsafe { foo.offset((6 as i32) as isize) }) })
// CHECK-NEXT:         != (((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (8 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_20:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: i32 = [[V_20]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_21]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { strlen(unsafe { foo.offset(([[V_21]] & (1 as i32)) as isize) }) })
// CHECK-NEXT:         != (((10 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_23:__v[0-9]+]]: i32 = [[V_22]] - (6 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_23]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             unsafe { foo.offset([[V_23]] as isize) },
// CHECK-NEXT:             c"lo world".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { strcmp(foo, (unsafe { bar }) as *const i8) }) >= (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_25:__v[0-9]+]]: i32 = [[V_24]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_25]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             foo,
// CHECK-NEXT:             (unsafe { unsafe { bar }.offset(([[V_24]] & (1 as i32)) as isize) }) as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) >= (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (4 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_26:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_27:__v[0-9]+]]: i32 = [[V_26]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_27]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strchr(
// CHECK-NEXT:             unsafe { foo.offset(([[V_26]] & (7 as i32)) as isize) },
// CHECK-NEXT:             108 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != ((unsafe { foo.offset((9 as i32) as isize) }) as *mut i8)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { strchr((unsafe { bar }) as *const i8, 111 as i32) })
// CHECK-NEXT:         != unsafe { unsafe { bar }.offset((4 as i32) as isize) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { strchr((unsafe { bar }) as *const i8, 0 as i32) })
// CHECK-NEXT:         != unsafe { unsafe { bar }.offset((8 as i32) as isize) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { strrchr((unsafe { bar }) as *const i8, 120 as i32) }) != std::ptr::null_mut::<i8>()
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { strrchr((unsafe { bar }) as *const i8, 111 as i32) })
// CHECK-NEXT:         != unsafe { unsafe { bar }.offset((4 as i32) as isize) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_28:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_29:__v[0-9]+]]: i32 = [[V_28]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_29]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_30:__v[0-9]+]]: i32 = unsafe { y };
// CHECK-NEXT:     let mut [[V_31:__v[0-9]+]]: i32 = [[V_30]] - (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = [[V_31]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strcmp(
// CHECK-NEXT:             unsafe { foo.offset(([[V_28]] & (1 as i32)) as isize) },
// CHECK-NEXT:             (unsafe { (c"ello world".as_ptr() as *mut i8).offset(([[V_31]] & (1 as i32)) as isize) })
// CHECK-NEXT:                 as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (6 as i32) || (unsafe { y }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (dst.0.as_mut_ptr() as *mut i8).offset((5 as i32) as isize) } = (32 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (dst.0.as_mut_ptr() as *mut i8).offset((6 as i32) as isize) } = (0 as i32) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = 5 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_32:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_33:__v[0-9]+]]: i32 = [[V_32]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_33]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_34:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strncpy(
// CHECK-NEXT:             unsafe { (dst.0.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) },
// CHECK-NEXT:             unsafe { foo.offset(([[V_32]] & (3 as i32)) as isize) },
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != unsafe { (dst.0.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) }
// CHECK-NEXT:         || (unsafe { x }) != (6 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_34]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_34]] = (unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 (unsafe { (dst.0.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) })
// CHECK-NEXT:                     as *const i8,
// CHECK-NEXT:                 c"ello ".as_ptr() as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_34]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { memset(dst.0.as_mut_ptr() as *mut (), 32 as i32, 64 as u64) };
// CHECK-NEXT:     let mut [[V_35:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_36:__v[0-9]+]]: i32 = [[V_35]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_36]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_37:__v[0-9]+]]: i32 = unsafe { y };
// CHECK-NEXT:     let mut [[V_38:__v[0-9]+]]: i32 = [[V_37]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = [[V_38]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_39:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strncpy(
// CHECK-NEXT:             unsafe { (dst.0.as_mut_ptr() as *mut i8).offset(([[V_36]] & (1 as i32)) as isize) },
// CHECK-NEXT:             (unsafe { (c"foo".as_ptr() as *mut i8).offset(([[V_37]] & (3 as i32)) as isize) })
// CHECK-NEXT:                 as *const i8,
// CHECK-NEXT:             ((10 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != unsafe { (dst.0.as_mut_ptr() as *mut i8).offset((1 as i32) as isize) }
// CHECK-NEXT:         || (unsafe { x }) != (7 as i32)
// CHECK-NEXT:         || (unsafe { y }) != (2 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_39]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_39]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 dst.0.as_mut_ptr() as *const (),
// CHECK-NEXT:                 b" oo\0\0\0\0\0\0\0\0 \0".as_ptr() as *const (),
// CHECK-NEXT:                 ((12 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_39]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { memset(dst.0.as_mut_ptr() as *mut (), 32 as i32, 64 as u64) };
// CHECK-NEXT:     let mut [[V_40:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         strncpy(
// CHECK-NEXT:             dst.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             c"hello".as_ptr() as *const i8,
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (dst.0.as_mut_ptr() as *mut i8)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_40]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_40]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 dst.0.as_mut_ptr() as *const (),
// CHECK-NEXT:                 b"hello\0\0\0 \0".as_ptr() as *const (),
// CHECK-NEXT:                 ((9 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_40]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = 33 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             32 as i32,
// CHECK-NEXT:             64 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_41:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_42:__v[0-9]+]]: i32 = [[V_41]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_42]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_43:__v[0-9]+]]: i32 = unsafe { y };
// CHECK-NEXT:     let mut [[V_44:__v[0-9]+]]: i32 = [[V_43]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = [[V_44]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_45:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *mut (),
// CHECK-NEXT:             [[V_41]],
// CHECK-NEXT:             ([[V_44]] as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != ((unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *mut ())
// CHECK-NEXT:         || (unsafe { x }) != (33 as i32) + (1 as i32)
// CHECK-NEXT:         || (unsafe { y }) != (3 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_45]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_45]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *const (),
// CHECK-NEXT:                 b"!!!\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((3 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_45]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_46:__v[0-9]+]]: i32 = unsafe { y };
// CHECK-NEXT:     let mut [[V_47:__v[0-9]+]]: i32 = [[V_46]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = [[V_47]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_48:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset([[V_46]] as isize) })
// CHECK-NEXT:                 as *mut (),
// CHECK-NEXT:             45 as i32,
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != ((unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset((3 as i32) as isize)
// CHECK-NEXT:     }) as *mut ())
// CHECK-NEXT:         || (unsafe { y }) != (4 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_48]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_48]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }) as *const (),
// CHECK-NEXT:                 b"!!!--------\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((11 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_48]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = 10 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_49:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_50:__v[0-9]+]]: i32 = [[V_49]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_50]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_51:__v[0-9]+]]: i32 = unsafe { y };
// CHECK-NEXT:     let mut [[V_52:__v[0-9]+]]: i32 = [[V_51]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = [[V_52]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_53:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset([[V_50]] as isize) })
// CHECK-NEXT:                 as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ([[V_51]] as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != ((unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset((11 as i32) as isize)
// CHECK-NEXT:     }) as *mut ())
// CHECK-NEXT:         || (unsafe { x }) != (11 as i32)
// CHECK-NEXT:         || (unsafe { y }) != (5 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_53]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_53]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset((8 as i32) as isize)
// CHECK-NEXT:                 }) as *const (),
// CHECK-NEXT:                 b"---\0\0\0\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((7 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_53]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_54:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:     let mut [[V_55:__v[0-9]+]]: i32 = [[V_54]] + (4 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_55]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_56:__v[0-9]+]]: bool;
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             (unsafe { unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset([[V_55]] as isize) })
// CHECK-NEXT:                 as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             ((6 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != ((unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset((15 as i32) as isize)
// CHECK-NEXT:     }) as *mut ())
// CHECK-NEXT:         || (unsafe { x }) != (15 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_56]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_56]] = (unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(buf.0) as *mut i8 }.offset((10 as i32) as isize)
// CHECK-NEXT:                 }) as *const (),
// CHECK-NEXT:                 b"-\0\0\0\0\0\0\0\0\0\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((11 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_56]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
