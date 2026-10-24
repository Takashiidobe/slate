#include <pthread.h>
#include <stdio.h>
#include <string.h>

extern __attribute__((visibility("default"))) const char *malloc_conf;
const char *malloc_conf = "background_thread:true";

__attribute__((visibility("default"))) int exported_count = 3;

__thread char *reusable_buffer = NULL;
__thread int   reusable_used;
static __thread long thread_index = -1;

static int detect(void) {
  static __thread int supported = -1;
  if (supported == -1) {
    supported = (int)strlen(malloc_conf);
  }
  return supported;
}

static void *worker(void *argument) {
  long *out = argument;
  out[0]    = thread_index;
  thread_index = 42;
  out[1]    = reusable_buffer == NULL;
  out[2]    = reusable_used;
  out[3]    = detect();
  return NULL;
}

int main(void) {
  static char storage[8];
  thread_index    = 7;
  reusable_buffer = storage;
  reusable_used   = 5;
  long      seen[4];
  pthread_t thread;
  pthread_create(&thread, NULL, worker, seen);
  pthread_join(thread, NULL);
  printf("%ld %ld %ld %ld %ld %d %d %d\n", seen[0], seen[1], seen[2], seen[3],
         thread_index, reusable_used, detect(), exported_count);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(thread_local)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn pthread_create(
// CHECK-NEXT:         __newthread: *mut u64,
// CHECK-NEXT:         __attr: *const pthread_attr_t,
// CHECK-NEXT:         __start_routine: Option<unsafe extern "C-unwind" fn(*mut ()) -> *mut ()>,
// CHECK-NEXT:         __arg: *mut (),
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn pthread_join(__th: u64, __thread_return: *mut *mut ()) -> i32;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union pthread_attr_t {
// CHECK-NEXT:     __size: [i8; 56],
// CHECK-NEXT:     __align: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut malloc_conf: *const i8 = (b"background_thread:true\0".as_ptr() as *mut i8)
// CHECK-NEXT:     as *const i8;
// CHECK-NEXT: static mut exported_count: i32 = 3;
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut reusable_buffer: *mut i8 = std::ptr::null_mut::<i8>();
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut reusable_used: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut thread_index: i64 = -1;
// CHECK-NEXT: fn detect() -> i32 {
// CHECK-NEXT:     if (unsafe { supported }) == -1 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             supported = (strlen(malloc_conf) as u32) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { supported }
// CHECK-NEXT: }
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut supported: i32 = -1;
// CHECK-NEXT: extern "C-unwind" fn worker(argument: *mut ()) -> *mut () {
// CHECK-NEXT:     let out: *mut i64 = argument as *mut i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *out.add(0) = thread_index;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         thread_index = 42;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *out.add(1) = unsafe { reusable_buffer }.is_null() as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *out.add(2) = reusable_used as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *out.add(3) = detect() as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         thread_index = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         reusable_buffer = std::ptr::addr_of_mut!(storage) as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         reusable_used = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut seen: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:     let mut thread: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         pthread_create(
// CHECK-NEXT:             std::ptr::addr_of_mut!(thread),
// CHECK-NEXT:             std::ptr::null::<pthread_attr_t>(),
// CHECK-NEXT:             Some(worker),
// CHECK-NEXT:             seen.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { pthread_join(thread, std::ptr::null_mut::<*mut ()>()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld %ld %ld %ld %ld %d %d %d\n".as_ptr(),
// CHECK-NEXT:             seen.0[0],
// CHECK-NEXT:             seen.0[1],
// CHECK-NEXT:             seen.0[2],
// CHECK-NEXT:             seen.0[3],
// CHECK-NEXT:             thread_index,
// CHECK-NEXT:             reusable_used,
// CHECK-NEXT:             detect(),
// CHECK-NEXT:             exported_count,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut storage: [i8; 8] = unsafe { std::mem::zeroed() };
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
