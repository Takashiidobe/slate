#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <string.h>
#include <stdio.h>
int main(void) {
  char b[128];
  int r = strerror_r(ENOENT, b, sizeof b);
  printf("%d %d\n", r == 0, strstr(b, "No such") != 0);
  return 0;
}
