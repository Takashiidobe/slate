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
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: *mut u64,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *const slate_support::pthread_attr_t,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: Option<unsafe extern "C-unwind" fn(*mut ()) -> *mut ()>,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *mut (),
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut counter: i32 = 0;
// CHECK-NEXT: use slate_support::pthread_join;
// CHECK-NEXT: extern "C-unwind" fn worker(arg: *mut ()) -> *mut () {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: for i in 0i32..100000i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]] = 1i32;
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     std::ptr::addr_of_mut!(counter) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut threads: __SlateAlign16<[u64; 4]> = __SlateAlign16([0u64; 4]);
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             pthread_create(
// CHECK-NEXT:                 (threads.0.as_mut_ptr() as *mut u64).offset(i as isize),
// CHECK-NEXT:                 std::ptr::null::<slate_support::pthread_attr_t>(),
// CHECK-NEXT:                 Some(worker),
// CHECK-NEXT:                 std::ptr::null_mut::<()>(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             pthread_join(
// CHECK-NEXT:                 threads.0[i as usize] as u64,
// CHECK-NEXT:                 std::ptr::null_mut::<*mut ()>() as *mut *mut core::ffi::c_void,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         };
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
