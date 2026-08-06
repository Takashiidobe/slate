#include <stdio.h>
#include <time.h>
int main(void) {
  clock_t       start = clock();
  volatile long acc   = 0;
  for (long i = 0; i < 100000; i++)
    acc += i;
  clock_t end = clock();
  printf("%d\n", end >= start && start != (clock_t)-1);
  return 0;
}
