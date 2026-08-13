#include "test.h"
#include <pthread.h>

_Thread_local int v1;
_Thread_local int v2 = 5;
int               v3 = 7;

struct tls_values {
  int initial_v1;
  int initial_v2;
  int initial_v3;
  int updated_v1;
  int updated_v2;
  int updated_v3;
};

void *thread_main(void *unused) {
  struct tls_values *values = unused;

  values->initial_v1 = v1;
  values->initial_v2 = v2;
  values->initial_v3 = v3;

  v1 = 1;
  v2 = 2;
  v3 = 3;

  values->updated_v1 = v1;
  values->updated_v2 = v2;
  values->updated_v3 = v3;

  return NULL;
}

int main() {
  pthread_t         thr;
  struct tls_values values;

  ASSERT(0, v1);
  ASSERT(5, v2);
  ASSERT(7, v3);

  ASSERT(0, pthread_create(&thr, NULL, thread_main, &values));
  ASSERT(0, pthread_join(thr, NULL));

  ASSERT(0, values.initial_v1);
  ASSERT(5, values.initial_v2);
  ASSERT(7, values.initial_v3);
  ASSERT(1, values.updated_v1);
  ASSERT(2, values.updated_v2);
  ASSERT(3, values.updated_v3);

  ASSERT(0, v1);
  ASSERT(5, v2);
  ASSERT(3, v3);

  printf("OK\n");
  return 0;
}
