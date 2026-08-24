#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

static int once_total;

static int thread_worker(void *argument) { return *(int *)argument + 1; }

static void once_handler(void) { once_total += 1; }

static void tss_destructor(void *value) { once_total += value != NULL; }

static void quick_handler(void) { once_total += 100; }

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
// LOWERING-DAG: Option<unsafe extern "C" fn
// LOWERING-DAG: extern "C" fn thread_worker
// LOWERING-DAG: extern "C" fn once_handler
// LOWERING-DAG: extern "C" fn tss_destructor
// LOWERING-DAG: extern "C" fn quick_handler
// LOWERING-DAG: let mut control: i32 = 0;
// LOWERING-DAG: call_once(std::ptr::addr_of_mut!(control) as *mut i32
// LOWERING-NOT: Option<fn(
