#include <time.h>
#include <stdio.h>
int main(void) {
  time_t t = time(NULL);
  time_t t2;
  time(&t2);
  printf("%d\n", t > 1000000000 && t2 >= t);
  return 0;
}
