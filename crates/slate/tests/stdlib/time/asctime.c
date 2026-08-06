#include <stdio.h>
#include <time.h>
int main(void) {
  time_t     t = 1000000000;
  struct tm *g = gmtime(&t);
  printf("%s", asctime(g));
  return 0;
}
