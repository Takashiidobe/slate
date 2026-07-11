#include <time.h>
#include <stdio.h>
int main(void) {
  time_t a = 2000;
  time_t b = 500;
  printf("%.1f\n", difftime(a, b));
  return 0;
}
