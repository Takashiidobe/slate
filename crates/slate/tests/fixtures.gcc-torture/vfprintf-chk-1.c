/* { dg-skip-if "requires io" { freestanding } }  */

#ifndef test
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

volatile int should_optimize;

int __attribute__((noinline)) __vfprintf_chk(FILE *f, int flag, const char *fmt,
                                             va_list ap) {
#ifdef __OPTIMIZE__
  if (should_optimize)
    abort();
#endif
  should_optimize = 1;
  return vfprintf(f, fmt, ap);
}

void inner(int x, ...) {
  va_list ap, ap2;
  va_start(ap, x);
  va_start(ap2, x);

  switch (x) {
#define test(n, ret, opt, fmt, args)                                           \
  case n:                                                                      \
    should_optimize = opt;                                                     \
    __vfprintf_chk(stdout, 1, fmt, ap);                                        \
    if (!should_optimize)                                                      \
      abort();                                                                 \
    should_optimize = 0;                                                       \
    if (__vfprintf_chk(stdout, 1, fmt, ap2) != ret)                            \
      abort();                                                                 \
    if (!should_optimize)                                                      \
      abort();                                                                 \
    break;
#include "vfprintf-chk-1.c"
#undef test
  default:
    abort();
  }

  va_end(ap);
  va_end(ap2);
}

int main(void) {
#define test(n, ret, opt, fmt, args) inner args;
#include "vfprintf-chk-1.c"
#undef test
  return 0;
}

#else
test(0, 5, 1, "hello", (0));
test(1, 6, 1, "hello\n", (1));
test(2, 1, 1, "a", (2));
test(3, 0, 1, "", (3));
test(4, 5, 0, "%s", (4, "hello"));
test(5, 6, 0, "%s", (5, "hello\n"));
test(6, 1, 0, "%s", (6, "a"));
test(7, 0, 0, "%s", (7, ""));
test(8, 1, 0, "%c", (8, 'x'));
test(9, 7, 0, "%s\n", (9, "hello\n"));
test(10, 2, 0, "%d\n", (10, 0));
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-skip-if "requires io" { freestanding } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stdout: *mut _IO_FILE;
// CHECK-NEXT:     fn vfprintf(__s: *mut _IO_FILE, __format: *const i8, __arg: core::ffi::VaList<'_>) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_FILE {
// CHECK-NEXT:     _flags: i32,
// CHECK-NEXT:     _IO_read_ptr: *mut i8,
// CHECK-NEXT:     _IO_read_end: *mut i8,
// CHECK-NEXT:     _IO_read_base: *mut i8,
// CHECK-NEXT:     _IO_write_base: *mut i8,
// CHECK-NEXT:     _IO_write_ptr: *mut i8,
// CHECK-NEXT:     _IO_write_end: *mut i8,
// CHECK-NEXT:     _IO_buf_base: *mut i8,
// CHECK-NEXT:     _IO_buf_end: *mut i8,
// CHECK-NEXT:     _IO_save_base: *mut i8,
// CHECK-NEXT:     _IO_backup_base: *mut i8,
// CHECK-NEXT:     _IO_save_end: *mut i8,
// CHECK-NEXT:     _markers: *mut _IO_marker,
// CHECK-NEXT:     _chain: *mut _IO_FILE,
// CHECK-NEXT:     _fileno: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits5U0,
// CHECK-NEXT:     _short_backupbuf: [i8; 1],
// CHECK-NEXT:     _old_offset: i64,
// CHECK-NEXT:     _cur_column: u16,
// CHECK-NEXT:     _vtable_offset: i8,
// CHECK-NEXT:     _shortbuf: [i8; 1],
// CHECK-NEXT:     _lock: *mut (),
// CHECK-NEXT:     _offset: i64,
// CHECK-NEXT:     _codecvt: *mut _IO_codecvt,
// CHECK-NEXT:     _wide_data: *mut _IO_wide_data,
// CHECK-NEXT:     _freeres_list: *mut _IO_FILE,
// CHECK-NEXT:     _freeres_buf: *mut (),
// CHECK-NEXT:     _prevchain: *mut *mut _IO_FILE,
// CHECK-NEXT:     _mode: i32,
// CHECK-NEXT:     _unused3: i32,
// CHECK-NEXT:     _total_written: u64,
// CHECK-NEXT:     _unused2: [i8; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_marker {}
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_codecvt {}
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_wide_data {}
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits5U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut should_optimize: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __vfprintf_chk(
// CHECK-NEXT:     mut f: *mut _IO_FILE,
// CHECK-NEXT:     mut flag: i32,
// CHECK-NEXT:     mut fmt: *const i8,
// CHECK-NEXT:     mut ap: core::ffi::VaList<'_>,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     let __slate_stdout_writer_tail = unsafe { vfprintf(f, fmt, ap.clone()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     __slate_stdout_writer_tail
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn inner(mut x: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ap2: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     ap2 = __va_args.clone();
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: i64 = match x {
// CHECK-NEXT:         0 => 0,
// CHECK-NEXT:         1 => 1,
// CHECK-NEXT:         2 => 2,
// CHECK-NEXT:         3 => 3,
// CHECK-NEXT:         4 => 4,
// CHECK-NEXT:         5 => 5,
// CHECK-NEXT:         6 => 6,
// CHECK-NEXT:         7 => 7,
// CHECK-NEXT:         8 => 8,
// CHECK-NEXT:         9 => 9,
// CHECK-NEXT:         10 => 10,
// CHECK-NEXT:         _ => 11,
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         match [[T_0]] {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 // { dg-skip-if "requires io" { freestanding } }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"hello".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"hello".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (5 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 1;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"hello\n".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"hello\n".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (6 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 2;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"a".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"a".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (1 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 3;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (0 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 4;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (5 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 5;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             5 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (6 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 6;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             6 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (1 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 7;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             7 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (0 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 8;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             8 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%c".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%c".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (1 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 9;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             9 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s\n".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%s\n".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (7 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 10;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             10 => {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%d\n".as_ptr() as *const i8,
// CHECK-NEXT:                     ap.clone(),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 if __vfprintf_chk(
// CHECK-NEXT:                     unsafe { stdout },
// CHECK-NEXT:                     1 as i32,
// CHECK-NEXT:                     c"%d\n".as_ptr() as *const i8,
// CHECK-NEXT:                     ap2.clone(),
// CHECK-NEXT:                 ) != (2 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) })
// CHECK-NEXT:                     != (0 as i32))
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 11;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             11 => {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // { dg-skip-if "requires io" { freestanding } }
// CHECK-NEXT:     unsafe { inner(0 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(1 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(2 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(3 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(4 as i32, c"hello".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(5 as i32, c"hello\n".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(6 as i32, c"a".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(7 as i32, c"".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(8 as i32, 120 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(9 as i32, c"hello\n".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(10 as i32, 0 as i32) };
// CHECK-NEXT:     {}
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
