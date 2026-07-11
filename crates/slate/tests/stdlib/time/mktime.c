#include <time.h>
#include <stdio.h>
int main(void) {
  struct tm tm = {0};
  tm.tm_year = 100;
  tm.tm_mon = 0;
  tm.tm_mday = 1;
  tm.tm_hour = 12;
  tm.tm_isdst = -1;
  time_t a = mktime(&tm);
  tm.tm_mday = 2;
  time_t b = mktime(&tm);
  printf("%ld\n", (long)(b - a));
  return 0;
}
