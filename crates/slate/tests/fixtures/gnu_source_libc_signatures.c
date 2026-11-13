#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

int main(void) {
  char           buffer[128];
  char          *message = strerror_r(ENOENT, buffer, sizeof buffer);
  struct itimerval timer = {{0, 0}, {0, 0}};
  struct itimerval old;
  struct rlimit  limit;
  struct rusage  usage;

  printf("strerror_r: %s\n", message);
  printf("setitimer: %d\n", setitimer(ITIMER_REAL, &timer, &old));
  printf("getitimer: %d %ld\n", getitimer(ITIMER_REAL, &old), (long)old.it_value.tv_sec);
  printf("getrlimit: %d\n", getrlimit(RLIMIT_NOFILE, &limit));
  printf("setrlimit: %d\n", setrlimit(RLIMIT_NOFILE, &limit));
  printf("getrusage: %d\n", getrusage(RUSAGE_SELF, &usage));
  errno = 0;
  getpriority(PRIO_PROCESS, 0);
  printf("getpriority errno: %d\n", errno);
  printf("setpriority: %d\n", setpriority(PRIO_PROCESS, 0, getpriority(PRIO_PROCESS, 0)));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn strerror_r([[ARG_0:arg[0-9]+]]: i32, [[ARG_1:arg[0-9]+]]: *mut i8, [[ARG_2:arg[0-9]+]]: u64) -> *mut i8;
// CHECK-NEXT:     fn getrlimit([[ARG_0]]: u32, [[ARG_1]]: *mut slate_support::rlimit) -> i32;
// CHECK-NEXT:     fn setrlimit([[ARG_0]]: u32, [[ARG_1]]: *const slate_support::rlimit) -> i32;
// CHECK-NEXT:     fn getrusage([[ARG_0]]: i32, [[ARG_1]]: *mut rusage) -> i32;
// CHECK-NEXT:     fn getpriority([[ARG_0]]: u32, [[ARG_1]]: u32) -> i32;
// CHECK-NEXT:     fn setpriority([[ARG_0]]: u32, [[ARG_1]]: u32, [[ARG_2]]: i32) -> i32;
// CHECK-NEXT:     fn getitimer([[ARG_0]]: u32, [[ARG_1]]: *mut slate_support::itimerval) -> i32;
// CHECK-NEXT:     fn setitimer(
// CHECK-NEXT:         [[ARG_0]]: u32,
// CHECK-NEXT:         [[ARG_1]]: *const slate_support::itimerval,
// CHECK-NEXT:         [[ARG_2]]: *mut slate_support::itimerval,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct rusage {
// CHECK-NEXT:     ru_utime: slate_support::timeval,
// CHECK-NEXT:     ru_stime: slate_support::timeval,
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_40,
// CHECK-NEXT:     [[SLATE_ANON_1:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_47,
// CHECK-NEXT:     [[SLATE_ANON_2:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_53,
// CHECK-NEXT:     [[SLATE_ANON_3:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_59,
// CHECK-NEXT:     [[SLATE_ANON_4:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_66,
// CHECK-NEXT:     [[SLATE_ANON_5:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_72,
// CHECK-NEXT:     [[SLATE_ANON_6:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_78,
// CHECK-NEXT:     [[SLATE_ANON_7:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_85,
// CHECK-NEXT:     [[SLATE_ANON_8:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_91,
// CHECK-NEXT:     [[SLATE_ANON_9:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_97,
// CHECK-NEXT:     [[SLATE_ANON_10:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_103,
// CHECK-NEXT:     [[SLATE_ANON_11:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_109,
// CHECK-NEXT:     [[SLATE_ANON_12:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_117,
// CHECK-NEXT:     [[SLATE_ANON_13:__slate_anon_[0-9]+]]: __SlateRecord_struct_rusage_h_124,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_40 {
// CHECK-NEXT:     ru_maxrss: i64,
// CHECK-NEXT:     __ru_maxrss_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_47 {
// CHECK-NEXT:     ru_ixrss: i64,
// CHECK-NEXT:     __ru_ixrss_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_53 {
// CHECK-NEXT:     ru_idrss: i64,
// CHECK-NEXT:     __ru_idrss_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_59 {
// CHECK-NEXT:     ru_isrss: i64,
// CHECK-NEXT:     __ru_isrss_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_66 {
// CHECK-NEXT:     ru_minflt: i64,
// CHECK-NEXT:     __ru_minflt_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_72 {
// CHECK-NEXT:     ru_majflt: i64,
// CHECK-NEXT:     __ru_majflt_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_78 {
// CHECK-NEXT:     ru_nswap: i64,
// CHECK-NEXT:     __ru_nswap_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_85 {
// CHECK-NEXT:     ru_inblock: i64,
// CHECK-NEXT:     __ru_inblock_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_91 {
// CHECK-NEXT:     ru_oublock: i64,
// CHECK-NEXT:     __ru_oublock_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_97 {
// CHECK-NEXT:     ru_msgsnd: i64,
// CHECK-NEXT:     __ru_msgsnd_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_103 {
// CHECK-NEXT:     ru_msgrcv: i64,
// CHECK-NEXT:     __ru_msgrcv_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_109 {
// CHECK-NEXT:     ru_nsignals: i64,
// CHECK-NEXT:     __ru_nsignals_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_117 {
// CHECK-NEXT:     ru_nvcsw: i64,
// CHECK-NEXT:     __ru_nvcsw_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_struct_rusage_h_124 {
// CHECK-NEXT:     ru_nivcsw: i64,
// CHECK-NEXT:     __ru_nivcsw_word: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[i8; 128]> = __SlateAlign16([0i8; 128]);
// CHECK-NEXT:     let message: *mut i8 = unsafe {
// CHECK-NEXT:         strerror_r(
// CHECK-NEXT:             slate_support::ENOENT as i32,
// CHECK-NEXT:             buffer.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<[i8; 128]>() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut timer: slate_support::itimerval = slate_support::itimerval {
// CHECK-NEXT:         it_interval: slate_support::timeval {
// CHECK-NEXT:             tv_sec: 0,
// CHECK-NEXT:             tv_usec: 0,
// CHECK-NEXT:         },
// CHECK-NEXT:         it_value: slate_support::timeval {
// CHECK-NEXT:             tv_sec: 0,
// CHECK-NEXT:             tv_usec: 0,
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut old: slate_support::itimerval = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut limit: slate_support::rlimit = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut usage: rusage = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"strerror_r: %s\n".as_ptr(), message) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "setitimer: {}", unsafe { setitimer((slate_support::ITIMER_REAL as i32) as u32,
// CHECK-NEXT:         std::ptr::addr_of_mut!(timer) as * const slate_support::itimerval,
// CHECK-NEXT:         std::ptr::addr_of_mut!(old)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"getitimer: %d %ld\n".as_ptr(),
// CHECK-NEXT:             getitimer(
// CHECK-NEXT:                 (slate_support::ITIMER_REAL as i32) as u32,
// CHECK-NEXT:                 std::ptr::addr_of_mut!(old),
// CHECK-NEXT:             ),
// CHECK-NEXT:             old.it_value.tv_sec,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "getrlimit: {}", unsafe { getrlimit((slate_support::RLIMIT_NOFILE as i32) as u32,
// CHECK-NEXT:         std::ptr::addr_of_mut!(limit)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "setrlimit: {}", unsafe { setrlimit((slate_support::RLIMIT_NOFILE as i32) as u32,
// CHECK-NEXT:         std::ptr::addr_of_mut!(limit) as * const slate_support::rlimit) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "getrusage: {}", unsafe { getrusage(slate_support::RUSAGE_SELF as i32,
// CHECK-NEXT:         std::ptr::addr_of_mut!(usage)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *slate_support::errno_location() = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { getpriority((slate_support::PRIO_PROCESS as i32) as u32, 0u32) };
// CHECK-NEXT:     println!("getpriority errno: {}", unsafe { * slate_support::errno_location() });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "setpriority: {}", unsafe { setpriority((slate_support::PRIO_PROCESS as i32) as
// CHECK-NEXT:         u32, 0u32, getpriority((slate_support::PRIO_PROCESS as i32) as u32, 0u32)) }
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
