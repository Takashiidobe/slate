#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

static int once_total;

static int thread_worker(void *argument) { return *(int *)argument + 1; }

static void once_handler(void) { once_total += 1; }

static void tss_destructor(void *value) { once_total += value != NULL; }

static void quick_handler(void) { once_total += 100; }

// @check-fn-begin
int main(void) {
  thrd_t thread;
  tss_t  key;
  int    argument       = 40;
  int    thread_result  = 0;
  int    thread_created = thrd_create(&thread, thread_worker, &argument);
  int    thread_joined =
      thread_created == thrd_success ? thrd_join(thread, &thread_result) : -1;

  once_flag control = ONCE_FLAG_INIT;
  call_once(&control, once_handler);
  call_once(&control, once_handler);

  int key_created = tss_create(&key, tss_destructor);
  if (key_created == thrd_success) {
    tss_delete(key);
  }

  int quick_registered = at_quick_exit(quick_handler);
  printf("%d %d %d %d %d %d\n", thread_created, thread_joined, thread_result,
         once_total, key_created, quick_registered);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut thread: u64 = 0;
// CHECK-NEXT:     let mut key: u32 = 0;
// CHECK-NEXT:     let mut argument: i32 = 40;
// CHECK-NEXT:     let mut thread_result: i32 = 0;
// CHECK-NEXT:     let thread_created: i32 = unsafe {
// CHECK-NEXT:         thrd_create(
// CHECK-NEXT:             std::ptr::addr_of_mut!(thread),
// CHECK-NEXT:             Some(thread_worker),
// CHECK-NEXT:             std::ptr::addr_of_mut!(argument) as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut thread_joined: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = if thread_created == 0 {
// CHECK-NEXT:         unsafe { thrd_join(thread, std::ptr::addr_of_mut!(thread_result)) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         -1
// CHECK-NEXT:     };
// CHECK-NEXT:     thread_joined = [[V_0]];
// CHECK-NEXT:     let mut control: __SlateRecord_thread_shared_types_h_112 = __SlateRecord_thread_shared_types_h_112 {
// CHECK-NEXT:         __data: 0,
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { call_once(std::ptr::addr_of_mut!(control), Some(once_handler)) };
// CHECK-NEXT:     unsafe { call_once(std::ptr::addr_of_mut!(control), Some(once_handler)) };
// CHECK-NEXT:     let key_created: i32 = unsafe {
// CHECK-NEXT:         tss_create(std::ptr::addr_of_mut!(key), Some(tss_destructor))
// CHECK-NEXT:     };
// CHECK-NEXT:     if key_created == 0 {
// CHECK-NEXT:         unsafe { tss_delete(key) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let quick_registered: i32 = unsafe { at_quick_exit(Some(quick_handler)) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", thread_created, thread_joined, thread_result, unsafe {
// CHECK-NEXT:         once_total }, key_created, quick_registered
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
