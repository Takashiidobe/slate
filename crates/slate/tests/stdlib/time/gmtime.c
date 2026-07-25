#include <stdio.h>
#include <time.h>
int main(void) {
  time_t t = 1000000000;
  struct tm *g = gmtime(&t);
  printf("%d-%02d-%02d %02d:%02d:%02d wday=%d yday=%d\n", g->tm_year + 1900,
         g->tm_mon + 1, g->tm_mday, g->tm_hour, g->tm_min, g->tm_sec,
         g->tm_wday, g->tm_yday);
  return 0;
}
