#include <stdio.h>
#include <time.h>

int main(void) {
  struct timespec value = {0};
  int result = timespec_get(&value, TIME_UTC);
  int nanoseconds_in_range =
      value.tv_nsec >= 0 && value.tv_nsec < 1000000000L;
  printf("%d %d\n", result == TIME_UTC, nanoseconds_in_range);
  return 0;
}
