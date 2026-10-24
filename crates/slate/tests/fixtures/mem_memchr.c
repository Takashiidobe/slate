#include <stdio.h>

int main(void) {
  unsigned char       buf[8]    = {10, 20, 30, 40, 50, 60, 70, 80};
  const unsigned char cbuf[4]   = {9, 8, 7, 6};
  char                word[]    = "abc";
  int                 needle    = (int)buf[3];
  size_t volatile partial_count = 4;
  unsigned char *hit        = (unsigned char *)__builtin_memchr(buf, needle, 8);
  unsigned char *miss       = (unsigned char *)__builtin_memchr(buf, 99, 8);
  unsigned char *zero       = (unsigned char *)__builtin_memchr(buf, 10, 0);
  char          *nul_after  = (char *)__builtin_memchr(word, 0, sizeof word);
  char          *nul_equal  = (char *)__builtin_memchr(word, 0, 3);
  char          *nul_before = (char *)__builtin_memchr(word, 0, 2);
  unsigned char *partial =
      (unsigned char *)__builtin_memchr(buf, needle, partial_count);
  unsigned char *offset_base = buf + 2;
  unsigned char *offset =
      (unsigned char *)__builtin_memchr(offset_base, needle, 2);
  const unsigned char *const_hit =
      (const unsigned char *)__builtin_memchr(cbuf, 7, sizeof cbuf);
  printf("%ld %d %d %ld %d %d %ld %ld %ld\n", (long)(hit - buf), miss == 0,
         zero == 0, (long)(nul_after - word), nul_equal == 0, nul_before == 0,
         (long)(partial - buf), (long)(offset - offset_base),
         (long)(const_hit - cbuf));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "memchr"]
// CHECK-NEXT:     fn __builtin_memchr([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [u8; 8] = [10, 20, 30, 40, 50, 60, 70, 80];
// CHECK-NEXT:     let mut cbuf: [u8; 4] = [9, 8, 7, 6];
// CHECK-NEXT:     let mut word: [i8; 4] = [97, 98, 99, 0];
// CHECK-NEXT:     let needle: i32 = (buf[3] as u32) as i32;
// CHECK-NEXT:     let partial_count: u64 = 4;
// CHECK-NEXT:     let hit: *mut u8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(buf.as_mut_ptr() as *const (), needle, 8u64)
// CHECK-NEXT:     }) as *mut u8;
// CHECK-NEXT:     let miss: *mut u8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(buf.as_mut_ptr() as *const (), 99i32, 8u64)
// CHECK-NEXT:     }) as *mut u8;
// CHECK-NEXT:     let zero: *mut u8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(buf.as_mut_ptr() as *const (), 10i32, 0u64)
// CHECK-NEXT:     }) as *mut u8;
// CHECK-NEXT:     let nul_after: *mut i8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(word.as_mut_ptr() as *const (), 0i32, 4u64)
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     let nul_equal: *mut i8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(word.as_mut_ptr() as *const (), 0i32, 3u64)
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     let nul_before: *mut i8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(word.as_mut_ptr() as *const (), 0i32, 2u64)
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     let partial: *mut u8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(
// CHECK-NEXT:             buf.as_mut_ptr() as *const (),
// CHECK-NEXT:             needle,
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(partial_count)),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as *mut u8;
// CHECK-NEXT:     let offset_base: *mut u8 = unsafe { (buf.as_mut_ptr() as *mut u8).add(2) };
// CHECK-NEXT:     let offset: *mut u8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(offset_base as *const (), needle, 2u64)
// CHECK-NEXT:     }) as *mut u8;
// CHECK-NEXT:     let const_hit: *const u8 = (unsafe {
// CHECK-NEXT:         __builtin_memchr(cbuf.as_ptr() as *const (), 7i32, 4u64)
// CHECK-NEXT:     }) as *const u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld %d %d %ld %d %d %ld %ld %ld\n".as_ptr(),
// CHECK-NEXT:             hit.offset_from(buf.as_mut_ptr() as *mut u8) as i64,
// CHECK-NEXT:             miss.is_null() as i32,
// CHECK-NEXT:             zero.is_null() as i32,
// CHECK-NEXT:             nul_after.offset_from(word.as_mut_ptr() as *mut i8) as i64,
// CHECK-NEXT:             nul_equal.is_null() as i32,
// CHECK-NEXT:             nul_before.is_null() as i32,
// CHECK-NEXT:             partial.offset_from(buf.as_mut_ptr() as *mut u8) as i64,
// CHECK-NEXT:             offset.offset_from(offset_base as *mut u8) as i64,
// CHECK-NEXT:             const_hit.offset_from(cbuf.as_ptr() as *const u8) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
