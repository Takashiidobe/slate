/* { dg-skip-if "requires io" { freestanding } }  */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

volatile int should_optimize;

int __attribute__((noinline)) __fprintf_chk(FILE *f, int flag, const char *fmt,
                                            ...) {
  va_list ap;
  int     ret;
#ifdef __OPTIMIZE__
  if (should_optimize)
    abort();
#endif
  should_optimize = 1;
  va_start(ap, fmt);
  ret = vfprintf(f, fmt, ap);
  va_end(ap);
  return ret;
}

int main(void) {
#define test(ret, opt, args...)                                                \
  should_optimize = opt;                                                       \
  __fprintf_chk(stdout, 1, args);                                              \
  if (!should_optimize)                                                        \
    abort();                                                                   \
  should_optimize = 0;                                                         \
  if (__fprintf_chk(stdout, 1, args) != ret)                                   \
    abort();                                                                   \
  if (!should_optimize)                                                        \
    abort();
  test(5, 1, "hello");
  test(6, 1, "hello\n");
  test(1, 1, "a");
  test(0, 1, "");
  test(5, 1, "%s", "hello");
  test(6, 1, "%s", "hello\n");
  test(1, 1, "%s", "a");
  test(0, 1, "%s", "");
  test(1, 1, "%c", 'x');
  test(7, 0, "%s\n", "hello\n");
  test(2, 0, "%d\n", 0);
  return 0;
}

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
// CHECK-NEXT: unsafe extern "C-unwind" fn __fprintf_chk(
// CHECK-NEXT:     mut f: *mut _IO_FILE,
// CHECK-NEXT:     mut flag: i32,
// CHECK-NEXT:     mut fmt: *const i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ret: i32 = 0 as i32;
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     ret = unsafe { vfprintf(f, fmt, ap.clone()) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     ret
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe { __fprintf_chk(unsafe { stdout }, 1 as i32, c"hello".as_ptr() as *const i8) };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe { __fprintf_chk(unsafe { stdout }, 1 as i32, c"hello".as_ptr() as *const i8) })
// CHECK-NEXT:         != (5 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"hello\n".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"hello\n".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (6 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe { __fprintf_chk(unsafe { stdout }, 1 as i32, c"a".as_ptr() as *const i8) };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe { __fprintf_chk(unsafe { stdout }, 1 as i32, c"a".as_ptr() as *const i8) })
// CHECK-NEXT:         != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe { __fprintf_chk(unsafe { stdout }, 1 as i32, c"".as_ptr() as *const i8) };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe { __fprintf_chk(unsafe { stdout }, 1 as i32, c"".as_ptr() as *const i8) })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"hello".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"hello".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (5 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"hello\n".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"hello\n".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (6 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"a".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"a".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s".as_ptr() as *const i8,
// CHECK-NEXT:             c"".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%c".as_ptr() as *const i8,
// CHECK-NEXT:             120 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%c".as_ptr() as *const i8,
// CHECK-NEXT:             120 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s\n".as_ptr() as *const i8,
// CHECK-NEXT:             c"hello\n".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%s\n".as_ptr() as *const i8,
// CHECK-NEXT:             c"hello\n".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (7 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%d\n".as_ptr() as *const i8,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __fprintf_chk(
// CHECK-NEXT:             unsafe { stdout },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"%d\n".as_ptr() as *const i8,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (2 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
