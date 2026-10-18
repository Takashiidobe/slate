#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

static atomic_int counter = 0;

static void *worker(void *arg) {
  (void)arg;
  for (int i = 0; i < 100000; i++) {
    atomic_fetch_add(&counter, 1);
  }
  return 0;
}

int main(void) {
  pthread_t threads[4];
  for (int i = 0; i < 4; i++) {
    pthread_create(&threads[i], 0, worker, 0);
  }
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], 0);
  }
  printf("%d\n", (int)counter);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
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
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union pthread_attr_t {
// CHECK-NEXT:     __size: [i8; 56],
// CHECK-NEXT:     __align: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut counter: i32 = 0 as i32;
// CHECK-NEXT: extern "C-unwind" fn worker(arg: *mut ()) -> *mut () {
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < 100000 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]] = 1 as i32;
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     std::ptr::addr_of_mut!(counter) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut threads: __SlateAlign16<[u64; 4]> = __SlateAlign16([0 as u64; 4]);
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             pthread_create(
// CHECK-NEXT:                 (threads.0.as_mut_ptr() as *mut u64).offset(i as isize),
// CHECK-NEXT:                 std::ptr::null::<pthread_attr_t>(),
// CHECK-NEXT:                 Some(worker),
// CHECK-NEXT:                 std::ptr::null_mut::<()>(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         unsafe { pthread_join(threads.0[i as usize], std::ptr::null_mut::<*mut ()>()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of!(counter)
// CHECK-NEXT:         as * mut i32).load(std::sync::atomic::Ordering::SeqCst) }
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
