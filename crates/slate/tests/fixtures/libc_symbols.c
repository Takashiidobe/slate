#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

static int compare(const void *left, const void *right) {
  int a = *(const int *)left;
  int b = *(const int *)right;
  return (a > b) - (a < b);
}

static rlim_t limit_value(rlim_t value) {
  return value + 1;
}

int main(void) {
  int values[] = {3, 1, 2};
  qsort(values, 3, sizeof(values[0]), compare);
  pid_t (*process_id)(void) = getpid;
  optind = 7;
  char byte = 'x';
  int descriptors[2];
  if (pipe(descriptors) != 0) {
    return 1;
  }
  ssize_t written = write(descriptors[1], &byte, 1);
  char result = 0;
  ssize_t received = read(descriptors[0], &result, 1);
  close(descriptors[0]);
  close(descriptors[1]);
  printf("%d%d%d %d %d %d %ld %ld %c %lu\n", values[0], values[1],
         values[2], process_id() == getpid(), optind, EINVAL != O_RDONLY,
         (long)written, (long)received, result, (unsigned long)limit_value(4));
  printf("%d\n", memcmp(&in6addr_any, &in6addr_loopback, sizeof(in6addr_any)) != 0);
  printf("%d %d %d %d\n", EOF, -EOF, EOF + 1, EOF == -1);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut optind: i32;
// CHECK-NEXT:     static mut in6addr_any: in6_addr;
// CHECK-NEXT:     static mut in6addr_loopback: in6_addr;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct in6_addr {
// CHECK-NEXT:     __in6_u: __SlateRecord_in_h_225,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_in_h_225 {
// CHECK-NEXT:     __u6_addr8: [u8; 16],
// CHECK-NEXT:     __u6_addr16: [u16; 8],
// CHECK-NEXT:     __u6_addr32: [u32; 4],
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::qsort;
// CHECK-NEXT: use slate_support::memcmp;
// CHECK-NEXT: use slate_support::close;
// CHECK-NEXT: use slate_support::read;
// CHECK-NEXT: use slate_support::write;
// CHECK-NEXT: use slate_support::pipe;
// CHECK-NEXT: use slate_support::getpid;
// CHECK-NEXT: extern "C-unwind" fn compare(left: *const (), right: *const ()) -> i32 {
// CHECK-NEXT:     let a: i32 = unsafe { *(left as *const i32) };
// CHECK-NEXT:     let b: i32 = unsafe { *(right as *const i32) };
// CHECK-NEXT:     ((a > b) as i32) - ((a < b) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn limit_value(value: u64) -> u64 {
// CHECK-NEXT:     value.wrapping_add(1)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: [i32; 3] = [3, 1, 2];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         qsort(
// CHECK-NEXT:             (values.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void,
// CHECK-NEXT:             3u64 as usize,
// CHECK-NEXT:             (std::mem::size_of::<i32>() as u64) as usize,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT:                     Option<
// CHECK-NEXT:                         unsafe extern "C" fn(
// CHECK-NEXT:                             *const core::ffi::c_void,
// CHECK-NEXT:                             *const core::ffi::c_void,
// CHECK-NEXT:                         ) -> i32,
// CHECK-NEXT:                     >,
// CHECK-NEXT:                 >(Some(compare))
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let process_id: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:         >(getpid as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         optind = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut byte: i8 = 120;
// CHECK-NEXT:     let mut descriptors: [i32; 2] = [0i32; 2];
// CHECK-NEXT:     if (unsafe { pipe((descriptors.as_mut_ptr() as *mut i32) as *mut i32) as i32 }) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let written: i64 = unsafe {
// CHECK-NEXT:         write(
// CHECK-NEXT:             descriptors[1] as i32,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(byte) as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:             1u64 as usize,
// CHECK-NEXT:         ) as i64
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result: i8 = 0;
// CHECK-NEXT:     let received: i64 = unsafe {
// CHECK-NEXT:         read(
// CHECK-NEXT:             descriptors[0] as i32,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(result) as *mut ()) as *mut core::ffi::c_void,
// CHECK-NEXT:             1u64 as usize,
// CHECK-NEXT:         ) as i64
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { close(descriptors[0] as i32) as i32 };
// CHECK-NEXT:     unsafe { close(descriptors[1] as i32) as i32 };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d%d%d %d %d %d %ld %ld %c %lu\n".as_ptr(),
// CHECK-NEXT:             values[0],
// CHECK-NEXT:             values[1],
// CHECK-NEXT:             values[2],
// CHECK-NEXT:             (process_id.unwrap()() == (getpid() as i32)) as i32,
// CHECK-NEXT:             optind,
// CHECK-NEXT:             ((slate_support::EINVAL as i32) != (slate_support::O_RDONLY as i32)) as i32,
// CHECK-NEXT:             written,
// CHECK-NEXT:             received,
// CHECK-NEXT:             result as i32,
// CHECK-NEXT:             limit_value(4u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", ((unsafe { memcmp((std::ptr::addr_of!(in6addr_any) as * const ()) as *
// CHECK-NEXT:         const core::ffi::c_void, (std::ptr::addr_of!(in6addr_loopback) as * const ()) as
// CHECK-NEXT:         * const core::ffi::c_void, (std::mem::size_of::< in6_addr > () as u64) as usize)
// CHECK-NEXT:         as i32 }) != 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", slate_support::EOF as i32, - (slate_support::EOF as i32),
// CHECK-NEXT:         (slate_support::EOF as i32) + 1, ((slate_support::EOF as i32) == - 1) as i32
// CHECK-NEXT:     );
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
