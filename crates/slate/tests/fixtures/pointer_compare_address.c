#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>

int main(void) {
  time_t timestamp = 0;
  struct tm utc = {};
  struct tm local = {};
  int utc_result = gmtime_r(&timestamp, &utc) == &utc;
  int local_result = localtime_r(&timestamp, &local) == &local;
  printf("%d %d\n", utc_result, local_result);
  return 0;
}
