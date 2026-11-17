/* { dg-options "-fsanitize=thread -pthread" } */
#include <pthread.h>
#include <stdio.h>

static int counter;

static void *bump(void *arg) {
  (void)arg;
  counter++;
  return NULL;
}

int main(void) {
  pthread_t thread;
  puts("start");
  fflush(stdout);
  pthread_create(&thread, NULL, bump, NULL);
  counter++;
  pthread_join(thread, NULL);
  printf("%d\n", counter);
  return 0;
}
