/* { dg-skip-if "requires io" { freestanding } }  */
/* { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } } */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

volatile int should_optimize;

int __attribute__((noinline)) __printf_chk(int flag, const char *fmt, ...) {
  va_list ap;
  int     ret;
#ifdef __OPTIMIZE__
  if (should_optimize)
    abort();
#endif
  should_optimize = 1;
  va_start(ap, fmt);
  ret = vprintf(fmt, ap);
  va_end(ap);
  return ret;
}

int main(void) {
#define test(ret, opt, args...)                                                \
  should_optimize = opt;                                                       \
  __printf_chk(1, args);                                                       \
  if (!should_optimize)                                                        \
    abort();                                                                   \
  should_optimize = 0;                                                         \
  if (__printf_chk(1, args) != ret)                                            \
    abort();                                                                   \
  if (!should_optimize)                                                        \
    abort();
  test(5, 0, "hello");
  test(6, 1, "hello\n");
  test(1, 1, "a");
  test(0, 1, "");
  test(5, 0, "%s", "hello");
  test(6, 1, "%s", "hello\n");
  test(1, 1, "%s", "a");
  test(0, 1, "%s", "");
  test(1, 1, "%c", 'x');
  test(7, 1, "%s\n", "hello\n");
  test(2, 0, "%d\n", 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-skip-if "requires io" { freestanding } }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn vprintf(__format: *const i8, __arg: core::ffi::VaList<'_>) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut should_optimize: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn __printf_chk(
// CHECK-NEXT:     mut flag: i32,
// CHECK-NEXT:     mut fmt: *const i8,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ret: i32 = 0 as i32;
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     ret = unsafe { vprintf(fmt, ap.clone()) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     ret
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     unsafe { __printf_chk(1 as i32, c"hello".as_ptr() as *const i8) };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe { __printf_chk(1 as i32, c"hello".as_ptr() as *const i8) }) != (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe { __printf_chk(1 as i32, c"hello\n".as_ptr() as *const i8) };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe { __printf_chk(1 as i32, c"hello\n".as_ptr() as *const i8) }) != (6 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe { __printf_chk(1 as i32, c"a".as_ptr() as *const i8) };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe { __printf_chk(1 as i32, c"a".as_ptr() as *const i8) }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe { __printf_chk(1 as i32, c"".as_ptr() as *const i8) };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe { __printf_chk(1 as i32, c"".as_ptr() as *const i8) }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:     unsafe { __printf_chk(1 as i32, c"%c".as_ptr() as *const i8, 120 as i32) };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe { __printf_chk(1 as i32, c"%c".as_ptr() as *const i8, 120 as i32) }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 1 as i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:         __printf_chk(
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
// CHECK-NEXT:     unsafe { __printf_chk(1 as i32, c"%d\n".as_ptr() as *const i8, 0 as i32) };
// CHECK-NEXT:     if !((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(should_optimize)) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(should_optimize), 0 as i32) };
// CHECK-NEXT:     if (unsafe { __printf_chk(1 as i32, c"%d\n".as_ptr() as *const i8, 0 as i32) }) != (2 as i32) {
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
