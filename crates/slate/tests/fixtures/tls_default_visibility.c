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
