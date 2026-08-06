#include <stdio.h>
#include <threads.h>

extern _Thread_local int shared_value;

int read_shared_value(void);

static int worker(void *argument) {
  int next     = *(int *)argument;
  int before   = shared_value;
  shared_value = next;
  return before * 100 + read_shared_value();
}

int main(void) {
  thrd_t thread;
  int    next          = 29;
  int    worker_result = 0;
  shared_value         = 17;
  int created          = thrd_create(&thread, worker, &next);
  int joined = created == thrd_success ? thrd_join(thread, &worker_result) : -1;
  printf("%d %d %d %d\n", worker_result, shared_value, created, joined);
  return 0;
}
