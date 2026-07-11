#include <time.h>
#include <stdio.h>
int main(void) {
  time_t t = 1000000000;
  struct tm *g = gmtime(&t);
  char buf[64];
  size_t n = strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", g);
  printf("%zu %s\n", n, buf);
  return 0;
}
